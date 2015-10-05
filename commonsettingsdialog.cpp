#include "commonsettingsdialog.h"
#include "ui_commonsettingsdialog.h"

CommonSettingsDialog::CommonSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommonSettingsDialog)
{
    ui->setupUi(this);

}

CommonSettingsDialog::~CommonSettingsDialog()
{
    delete ui;
}

void CommonSettingsDialog::setRefreshPreiod(const int value)
{
    ui->spinBoxRefresh->setValue(value);
}

int CommonSettingsDialog::refreshPeriod() const
{
    return ui->spinBoxRefresh->value();
}

void CommonSettingsDialog::setShowDelay(const int delay)
{
    ui->spinBoxShowDelay->setValue(delay);
}

int CommonSettingsDialog::showDelay() const
{
    return ui->spinBoxShowDelay->value();
}
