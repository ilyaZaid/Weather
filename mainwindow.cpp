#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QCoreApplication>

#include "commonsettingsdialog.h"
#include "impl/yandexweaterproducer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSourceSetupMenu(QMenu *sourceMenu)
{
    ui->actionSource->setMenu(sourceMenu); //в кнопке меню "Настройка источников" вывести подменю
}

QAction *MainWindow::commonSettingAction()
{
    return ui->actionCommon; //вернуть кнопку меню "Общие настройки"
}

void MainWindow::setWeatherHandler(iButtonClick *button)
{
 mButton=button;
}

void MainWindow::viewCity(QString city) const
{
   ui->labelCity_2->setText(city);
}

void MainWindow::setTemperature(QString temperature) const
{
   ui->labelTemp->setText(temperature);
}

void MainWindow::setPressure(QString pressure) const
{
   ui->labelPressure->setText(pressure);
}

void MainWindow::setWind(QString Wind) const
{
   ui->labelWind->setText(Wind);
}

void MainWindow::on_pushButton_clicked()
{
    mButton->buttonBackClick();
}


void MainWindow::on_pushButton_2_clicked()
{
    mButton->buttonNextClick();
}
