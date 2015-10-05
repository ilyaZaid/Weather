#ifndef YANDEXWEATERSETTINGDIALOG_H
#define YANDEXWEATERSETTINGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui { // пространство имен
class YandexWeaterSettingDialog;
}

class YandexWeaterSettingDialog : public QDialog
{
    Q_OBJECT

    enum {
        locationIdRole = Qt::UserRole + 1,
        yandexInfoIdRole
        //UserRole-задает пользовательскую роль
    }; // задаем две константы

public:
    explicit YandexWeaterSettingDialog(QWidget *parent = 0);
    ~YandexWeaterSettingDialog();

    void readSettings();
    void addLocation(const QString &locationId, const QString &locationDescription);
    void removeLocation(const QModelIndex &index);

    void onAddButtonPushed();
    void onRemoveButtonPushed();

    void onItemChanged(const QStandardItem *item); //редактирование записи

private slots:


private:
    Ui::YandexWeaterSettingDialog *ui;
    QStandardItemModel *mModel; // модель данных таже бд только модель
};

#endif // YANDEXWEATERSETTINGDIALOG_H
