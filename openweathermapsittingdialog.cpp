#include "openweathermapsittingdialog.h"
#include "ui_openweathermapsittingdialog.h"

#include <QUuid>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QMessageBox>

OpenWeatherMapSittingDialog::OpenWeatherMapSittingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenWeatherMapSittingDialog),
    mModel(new QStandardItemModel(this))  //создаем новую модель данных
{
    ui->setupUi(this);
    readSettings();// прочитать настройки в БД
    connect(ui->pushButtonAdd, &QPushButton::clicked,
            this, &OpenWeatherMapSittingDialog::onAddButtonPushed);
    connect(ui->pushButtonRemove, &QPushButton::clicked,
            this, &OpenWeatherMapSittingDialog::onRemoveButtonPushed);
    mModel->setHorizontalHeaderLabels({tr("Идентификатор"), tr("Название")});
    connect(mModel, &QStandardItemModel::itemChanged,
            this, &OpenWeatherMapSittingDialog::onItemChanged);
    // если данные в модели были изменены то переходим к слоту onItemChanged
    ui->treeView->setModel(mModel); //задаем treeView модель вывода
}

OpenWeatherMapSittingDialog::~OpenWeatherMapSittingDialog()
{
    delete ui;
}


void OpenWeatherMapSittingDialog::readSettings()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.exec("select openmap_info.id, locations.id, openmap_info.openmap_loc_id, locations.descriptions from openmap_info "
           "inner join locations on (locations.id == openmap_info.location_id)");
    //задаем выборку из двух таблиц
    while (q.next())
    {
        int openmapInfoId = q.value(0).toInt();
        int locationId = q.value(1).toInt();
        QString openmapLocationId = q.value(2).toString();
        QString locationDescription = q.value(3).toString();
        QStandardItem *locationIdItem = new QStandardItem(openmapLocationId); //новый Item для QStandardItemModel
        QStandardItem *locationDescriptionItem = new QStandardItem(locationDescription);
        locationDescriptionItem->setData(locationId, locationIdRole);
        locationDescriptionItem->setData(openmapInfoId, openmapInfoIdRole);
        locationIdItem->setData(locationId, locationIdRole);
        locationIdItem->setData(openmapInfoId, openmapInfoIdRole);
        mModel->appendRow({locationIdItem, locationDescriptionItem}); //добавить item'мы в mModel
    }
}

void OpenWeatherMapSittingDialog::addLocation(const QString &locationId, const QString &locationDescription)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.prepare("INSERT into locations (descriptions) values (?)"); //prepare-готовит запрос для исполнения
    q.addBindValue(locationDescription); //addBindValue- добавляет значение в место ?
    if (q.exec()) //exec() - выполняет ранее заданный звапрос и возвращает истоину если запрос успешно выполнен
    {
        q.exec("select last_insert_rowid();"); //возвращаем id последней записи добавленой в таблицу
        if (q.next()) // next-переходит к следующей записи
        {
            int id = q.value(0).toInt();
            q.prepare("INSERT into openmap_info (location_id, openmap_loc_id) values (?, ?)");
            q.addBindValue(id);
            q.addBindValue(locationId);
            q.exec();
            q.exec("select last_insert_rowid();");
            if (q.next()){
                int openmap_info_id = q.value(0).toInt();
                QStandardItem *locationIdItem = new QStandardItem(locationId);
                QStandardItem *locationDescriptionItem = new QStandardItem(locationDescription);
                locationDescriptionItem->setData(id, locationIdRole);
                locationDescriptionItem->setData(openmap_info_id, openmapInfoIdRole);
                locationIdItem->setData(id, locationIdRole);
                locationIdItem->setData(openmap_info_id, openmapInfoIdRole);
                mModel->appendRow({locationIdItem, locationDescriptionItem});
            }
        }
    }
}

void OpenWeatherMapSittingDialog::removeLocation(const QModelIndex &index) //удаляем город
{
    if (index.isValid()) {
        int id = index.data(locationIdRole).toInt();
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery q(db);
        q.prepare("delete from locations WHERE id = ?");
        q.addBindValue(id);
        q.exec();
        mModel->removeRow(index.row());
    }
}

void OpenWeatherMapSittingDialog::onAddButtonPushed() //нажатие кнопки добавить (создаются рандомные значения)
{
    QString decr = QString("loc_%0").arg(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex().right(6)));
    QString locId = QString("%0").arg(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex().right(6)));
    addLocation(locId, decr); // добавляем город в модель
    //  в %0 будет подставлено значение, arg(вот это значение);
}

void OpenWeatherMapSittingDialog::onRemoveButtonPushed() //нажатие кнопки удаление
{
    removeLocation(ui->treeView->currentIndex());
}

void OpenWeatherMapSittingDialog::onItemChanged(const QStandardItem *item) //редактирование данных
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    switch (item->column()) {
    case 0:
        // id
        q.prepare("update openmap_info SET openmap_loc_id = ? where id = ?");
        q.addBindValue(item->text());
        q.addBindValue(item->data(openmapInfoIdRole));
        break;
    case 1:
        // descr
        q.prepare("UPDATE locations set descriptions = ? WHERE id = ?");
        q.addBindValue(item->text());
        q.addBindValue(item->data(locationIdRole));
        break;
    default:
        return;
    }
    q.exec();

}
