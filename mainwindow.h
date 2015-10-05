#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QMenu>

#include "databasehandler.h"
#include "weaterhandler.h"
#include "interfaces/iweaterinterface.h"
#include "interfaces/iviewweatherinfo.h"
#include "interfaces/ibuttonclick.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public iViewWeatherInfo
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSourceSetupMenu(QMenu* sourceMenu);

    void setTemperature(QString temperature ) const override;
    void setPressure(QString pressure) const override;
    void setWind(QString Wind) const override;
    void viewCity(QString city) const override;

    void setWeatherHandler(iButtonClick *button);
    QAction *commonSettingAction();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    iButtonClick *mButton;


};

#endif // MAINWINDOW_H
