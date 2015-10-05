#include "yandexweatersettingdialog.h"
#include "ui_yandexweatersettingdialog.h"

#include <QUuid>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QMessageBox>

YandexWeaterSettingDialog::YandexWeaterSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YandexWeaterSettingDialog),
    mModel(new QStandardItemModel(this))  //создаем новую модель данных
{
    ui->setupUi(this);
    readSettings();// прочитать настройки в БД
    connect(ui->pushButtonAdd, &QPushButton::clicked,
            this, &YandexWeaterSettingDialog::onAddButtonPushed);
    connect(ui->pushButtonRemove, &QPushButton::clicked,
            this, &YandexWeaterSettingDialog::onRemoveButtonPushed);
    mModel->setHorizontalHeaderLabels({tr("Идентификатор"), tr("Название")});
    connect(mModel, &QStandardItemModel::itemChanged,
            this, &YandexWeaterSettingDialog::onItemChanged);
    // если данные в модели были изменены то переходим к слоту onItemChanged
    ui->treeView->setModel(mModel); //задаем treeView модель вывода
}

YandexWeaterSettingDialog::~YandexWeaterSettingDialog()
{
    delete ui;
}


void YandexWeaterSettingDialog::readSettings()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.exec("select yandex_info.id, locations.id, yandex_info.yandex_loc_id, locations.descriptions from yandex_info "
           "inner join locations on (locations.id == yandex_info.location_id)");
    //задаем выборку из двух таблиц
    while (q.next())
    {
        int yandexInfoId = q.value(0).toInt();
        int locationId = q.value(1).toInt();
        QString yandexLocationId = q.value(2).toString();
        QString locationDescription = q.value(3).toString();
        QStandardItem *locationIdItem = new QStandardItem(yandexLocationId); //новый Item для QStandardItemModel
        QStandardItem *locationDescriptionItem = new QStandardItem(locationDescription);
        locationDescriptionItem->setData(locationId, locationIdRole);
        locationDescriptionItem->setData(yandexInfoId, yandexInfoIdRole);
        locationIdItem->setData(locationId, locationIdRole);
        locationIdItem->setData(yandexInfoId, yandexInfoIdRole);
        mModel->appendRow({locationIdItem, locationDescriptionItem}); //добавить item'мы в mModel
    }
}

void YandexWeaterSettingDialog::addLocation(const QString &locationId, const QString &locationDescription)
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
            q.prepare("INSERT into yandex_info (location_id, yandex_loc_id) values (?, ?)");
            q.addBindValue(id);
            q.addBindValue(locationId);
            q.exec();
            q.exec("select last_insert_rowid();");
            if (q.next()){
                int yandex_info_id = q.value(0).toInt();
                QStandardItem *locationIdItem = new QStandardItem(locationId);
                QStandardItem *locationDescriptionItem = new QStandardItem(locationDescription);
                locationDescriptionItem->setData(id, locationIdRole);
                locationDescriptionItem->setData(yandex_info_id, yandexInfoIdRole);
                locationIdItem->setData(id, locationIdRole);
                locationIdItem->setData(yandex_info_id, yandexInfoIdRole);
                mModel->appendRow({locationIdItem, locationDescriptionItem});
            }
        }
    }
}

void YandexWeaterSettingDialog::removeLocation(const QModelIndex &index) //удаляем город
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

void YandexWeaterSettingDialog::onAddButtonPushed() //нажатие кнопки добавить (создаются рандомные значения)
{
    QString decr = QString("loc_%0").arg(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex().right(6)));
    QString locId = QString("%0").arg(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex().right(6)));
    addLocation(locId, decr); // добавляем город в модель
    //  в %0 будет подставлено значение, arg(вот это значение);
}

void YandexWeaterSettingDialog::onRemoveButtonPushed() //нажатие кнопки удаление
{
    removeLocation(ui->treeView->currentIndex());
}

void YandexWeaterSettingDialog::onItemChanged(const QStandardItem *item) //редактирование данных
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    switch (item->column()) {
    case 0:
        // id
        q.prepare("update yandex_info SET yandex_loc_id = ? where id = ?");
        q.addBindValue(item->text());
        q.addBindValue(item->data(yandexInfoIdRole));
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

