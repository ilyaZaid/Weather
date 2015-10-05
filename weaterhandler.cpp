#include "weaterhandler.h"
#include "impl/yandexweaterproducer.h"

#include "commonsettingsdialog.h"

#include <QMenu>
#include <QTimer>
#include <QMessageBox>
#include <QCoreApplication>


WeaterHandler::WeaterHandler(QObject *parent) :
    QObject(parent),
    mMainWindow(nullptr),
    mSourceMenu(new QMenu())
    //nullptr - пустой указатель
{
    if (!mDbHandler.openDb()) {
        QMessageBox::critical(mMainWindow, tr("Ошибка!"), tr("Невозможно подключиться к базе данных"));
        exit(-1);
    }
    mDbHandler.updateDatabase();// обновляем базу данных

    if (mDbHandler.statusTableLocations()) {
        QMessageBox *msb= new QMessageBox;
        msb->setText("В БД нет ни одного города");
        msb->exec();
        delete msb;
    }
    mCeateId=0;
    connect(&mTimer, SIGNAL(timeout()), SLOT(nextCity()));
    mTimeShowDelay = mDbHandler.showDelay();
}

void WeaterHandler::setMainWindow(QWidget *mainWindow)
{
    mMainWindow = mainWindow;
}

void WeaterHandler::setCommonSettingAction(QAction *action)
{
    connect(action, &QAction::triggered,
            this, &WeaterHandler::onCommonSettings);/* когда открываем окно для настройки
                                                   времени */
}

void WeaterHandler::buttonBackClick()
{
  if (!mVector.empty())
  {
    if (mCeateId==0)
          mCeateId = mVector.size()-1;
    else
          mCeateId -=1;

    viewWeatherInfo(mVector[mCeateId]);
   }
}

void WeaterHandler::buttonNextClick()
{
  if (!mVector.empty())
  {
    if (mCeateId==mVector.size()-1)
         mCeateId = 0;
    else
         mCeateId +=1;

    viewWeatherInfo(mVector[mCeateId]);
  }
}



void WeaterHandler::registerProducer(IWeaterInterface *producer) //добавляем источник
{
    producer->updateDatabase();
    mWeatherSetting[producer->weatherProducerId()] = producer; //в хэш таблицу добавляем указатель на клас по ключу mId
    QAction *action = mSourceMenu->addAction(producer->sourceDescription()); // в подменю добавляем активный элемент который
    //указан в prodeuser'е
    action->setData(producer->weatherProducerId()); //в экшен добавляем mId
    connect(action, &QAction::triggered, this, &WeaterHandler::onSourceActionTriggerd); //когда открываем окно настройки источника

    mVector += producer->vectorCityWeather();

    for (int i=0;i<=mVector.size()-1;i++)
    {
        mVector[i]->setRefreshPeriod(mDbHandler.refreshTime());
        connect(mVector[i],SIGNAL(endRead(IGetWeater*)),this,SLOT(viewWeatherInfo(IGetWeater*)));
    }
    mCeateId=0;
}


void WeaterHandler::registerViewWeather(iViewWeatherInfo *weatherInfoWindow)
{
mViewWeatherInfo = weatherInfoWindow;
}

void WeaterHandler::viewWeatherInfo(IGetWeater* weatherInfo)
{
// IGetWeater *weatherInfo = qobject_cast<IGetWeater*>(sender());
// if (weatherInfo)


    if (mVector[mCeateId]==weatherInfo)
       {
        mViewWeatherInfo->viewCity(weatherInfo->cityName());
        mViewWeatherInfo->setPressure(weatherInfo->pressure());
        mViewWeatherInfo->setTemperature(weatherInfo->temperature());
        mViewWeatherInfo->setWind(weatherInfo->wind());
        mTimer.start(mTimeShowDelay*60*1000);
       }

 }


void WeaterHandler::nextCity()
{
  if (!mVector.empty())
  {
    if (mCeateId==mVector.size()-1)
         mCeateId = 0;
    else
         mCeateId +=1;

  viewWeatherInfo(mVector[mCeateId]);
  }
}



QMenu *WeaterHandler::sourceSettingMenu()
{
    return mSourceMenu; // вернуть подменю
}

void WeaterHandler::onCommonSettings() //общие настройки
{
    CommonSettingsDialog dlg(mMainWindow);
    dlg.setRefreshPreiod(mDbHandler.refreshTime());// выводим время обновления
    dlg.setShowDelay(mDbHandler.showDelay()); // выводим время смены города
    if (dlg.exec() == QDialog::Accepted) // если был сигнал "Ок"
    {
        mDbHandler.setRefreshTimeAndShowDelay(dlg.refreshPeriod(), dlg.showDelay());/* обновляем
                                                                       данные в БД  */
        // меняем время обновления информации
        for (int i=0;i<=mVector.size()-1;i++)
        {
            mVector[i]->setRefreshPeriod(dlg.refreshPeriod());
        }
        mTimeShowDelay = dlg.showDelay();
        mTimer.start(mTimeShowDelay*60*1000);

    }
}

void WeaterHandler::onSourceActionTriggerd()
{
    QAction *action = qobject_cast<QAction*>(sender()); //sender() возвращает указатель на объект, выславший сигнал
    //вернет тот экшен который был испускателем
    if (action) // вернулся ли экшен, если нет то action будет равен 0
    {
        QString id = action->data().toString(); // получаем из полученного экшена id
        IWeaterInterface *interface = mWeatherSetting.value(id, nullptr); //по id полученного из экшена возвращаем указатель на класс
        if (interface)
        {
            //очищаем вектор от старых данных
            QVector<IGetWeater*> vectornull = interface->vectorCityWeather();
            for (int i=0; i<=vectornull.size()-1;i++)
            {
              for (int j=0; j<=mVector.size()-1;j++)
                  if (mVector[j] == vectornull[i])
                   mVector.remove(j);
            }

            interface->showSettingDialog(mMainWindow); //вызов окна
              //после закрытия окна добавления города
                mVector += interface->vectorCityWeather();

        }


       }

}

