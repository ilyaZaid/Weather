#include "openweathermapcreator.h"
#include "openweathermapproducer.h"
#include "openweathermapsittingdialog.h"
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QUuid>

OpenWeatherMapProduser::OpenWeatherMapProduser() :
    mId(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex()))
{
}

bool OpenWeatherMapProduser::showSettingDialog(QWidget *parent = nullptr)
{
   OpenWeatherMapSittingDialog dlg(parent);
    return (dlg.exec() == QDialog::Accepted);
}

QString OpenWeatherMapProduser::sourceDescription() const
{
    return tr("OpenWeatherMap");
}

QVector<IGetWeater*> OpenWeatherMapProduser::vectorCityWeather()
{
    QVector<QString> vectorWithId=returnAllCityId();
    QVector<IGetWeater*> newCityWeather;
    bool overlap = false;

    for (int i=0; i<=vectorWithId.size()-1;i++)
    {
        for (int j=0; j<=mCityWeather.size()-1;j++)
        {
            if (mCityWeather[j]->returnLicalId() == vectorWithId[i])
            {
                newCityWeather << mCityWeather[j];
                overlap = true;
            }
        }
            if(!overlap)
            {
                newCityWeather <<new OpenWeatherMapCreator();
                newCityWeather[i]->setLocationId(vectorWithId[i]);
            }
         overlap = false;
     }

    mCityWeather = newCityWeather;

    return (mCityWeather);
}




void OpenWeatherMapProduser::updateDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    auto record = db.record("openmap_info");
    QSqlQuery q(db);
    if (record.isEmpty()) {
        q.exec("CREATE TABLE openmap_info ( "
               "id            INTEGER PRIMARY KEY AUTOINCREMENT, "
               "location_id   INTEGER REFERENCES locations (id) ON DELETE CASCADE "
               "ON UPDATE CASCADE,"
               "openmap_loc_id VARCHAR"
               ");");
    }

}

QVector<QString> OpenWeatherMapProduser::returnAllCityId() const
{

    QVector<QString> vectorForId;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.exec("select openmap_info.openmap_loc_id from openmap_info");
    while (q.next())
    {
        vectorForId<<q.value(0).toString();
    }
    return vectorForId;
}


QString OpenWeatherMapProduser::weatherProducerId() const
{
    return mId;
}


