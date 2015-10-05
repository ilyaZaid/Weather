#ifndef WEATERHANDLER_H
#define WEATERHANDLER_H

#include <QObject>
#include <QDialog>
#include <QTimer>

#include "databasehandler.h"
#include "interfaces/iweaterinterface.h"
#include "interfaces/igetweater.h"
#include "interfaces/iviewweatherinfo.h"
#include "interfaces/ibuttonclick.h"

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class WeaterHandler : public QObject, public iButtonClick
{
    Q_OBJECT
public:
    explicit WeaterHandler(QObject *parent = 0);
    void setMainWindow(QWidget *mainWindow);
    void setCommonSettingAction(QAction *action); //вызов окна настроек времени
    void registerProducer(IWeaterInterface *producer); //вызов окна настроек для некого источника
    void registerViewWeather(iViewWeatherInfo *weatherInfoWindow);
    void buttonBackClick() override;
    void buttonNextClick() override;
    QMenu *sourceSettingMenu();

private:
    void onCommonSettings();
    void onSourceActionTriggerd();


private slots:
    void viewWeatherInfo(IGetWeater* weatherInfo);
    void nextCity();

private:
    QWidget *mMainWindow;
    DatabaseHandler mDbHandler;
    QHash<QString, IWeaterInterface*> mWeatherSetting; //хэш таблица
    QHash<QString,  QVector<IGetWeater*>> mVectorWeather;
    QMenu *mSourceMenu;
    iViewWeatherInfo *mViewWeatherInfo;
    QVector<IGetWeater*> mVector;
    QTimer mTimer;
    int mCeateId;
    int mTimeShowDelay;

};

#endif // WEATERHANDLER_H
