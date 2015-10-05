#ifndef COMMONSETTINGSDIALOG_H
#define COMMONSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class CommonSettingsDialog;
}

class CommonSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommonSettingsDialog(QWidget *parent = 0);
    ~CommonSettingsDialog();
    void setRefreshPreiod(const int value); //задать время обнавления инф. источника
    int refreshPeriod() const; //вернуть врямя об. источника

    void setShowDelay(const int delay); //задать время смены города
    int showDelay() const; //вернуть время смены города

private:
    Ui::CommonSettingsDialog *ui;
};

#endif // COMMONSETTINGSDIALOG_H
