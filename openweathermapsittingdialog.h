#ifndef OPENWEATHERMAPSITTINGDIALOG_H
#define OPENWEATHERMAPSITTINGDIALOG_H
#include <QDialog>
#include <QStandardItemModel>


namespace Ui { // пространство имен
class OpenWeatherMapSittingDialog;
}

class OpenWeatherMapSittingDialog : public QDialog
{
    Q_OBJECT

    enum {
        locationIdRole = Qt::UserRole + 1,
        openmapInfoIdRole
        //UserRole-задает пользовательскую роль
    }; // задаем две константы

public:
    explicit OpenWeatherMapSittingDialog(QWidget *parent = 0);
    ~OpenWeatherMapSittingDialog();

    void readSettings();
    void addLocation(const QString &locationId, const QString &locationDescription);
    void removeLocation(const QModelIndex &index);

    void onAddButtonPushed();
    void onRemoveButtonPushed();

    void onItemChanged(const QStandardItem *item); //редактирование записи

private:
    Ui::OpenWeatherMapSittingDialog *ui;
    QStandardItemModel *mModel; // модель данных таже бд только модель
};
#endif // OPENWEATHERMAPSITTINGDIALOG_H

