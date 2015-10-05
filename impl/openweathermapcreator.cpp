#include "openweathermapcreator.h"
#include <QXmlStreamReader>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

OpenWeatherMapCreator :: OpenWeatherMapCreator()
{
retryDelaySec = 1;
m_NetworkMngr = new QNetworkAccessManager(this);
connect(m_NetworkMngr,SIGNAL(finished(QNetworkReply*)),
         this, SLOT(readXml(QNetworkReply*)));

mTemperature = "Данные не загружены";
mPressure = "Данные не загружены";
mCity = "Данные не загружены";
mWind_speed = "Данные не загружены";

QTimer::singleShot(0, this, SLOT(requestXml())); //можно поставить ограничение если не задан город

}


void OpenWeatherMapCreator::setLocationId(const QString locationId)
{
    mLocalId = locationId;
    QTimer::singleShot(0, this, SLOT(requestXml()));
}

void OpenWeatherMapCreator::setRefreshPeriod(const int refreshPeriod)
{
  mRefreshPeriod =  refreshPeriod;
}



void OpenWeatherMapCreator::requestXml()
{
    QString urlForXML = QString("http://api.openweathermap.org/data/2.5/weather?q=%0&mode=xml").arg(mLocalId);
    m_NetworkMngr->get(QNetworkRequest(urlForXML));

}

void OpenWeatherMapCreator::readXml(QNetworkReply *netReply)

{
        if (netReply->error() != QNetworkReply::NoError)
        {
          QTimer::singleShot(retryDelaySec * 1000, this, SLOT (requestXml()));
          retryDelaySec <<=1;
          if (retryDelaySec > 60*60)
           retryDelaySec = 1;
          return;
        }
        else{
            retryDelaySec = 1;
            QXmlStreamReader xmlReader;
            xmlReader.addData(netReply->readAll());


            while (!xmlReader.atEnd())
                {
                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
                    {

                        if (xmlReader.name() == "city")
                        {
                            QXmlStreamAttributes streamAttributes = xmlReader.attributes();
                            mCity = streamAttributes.value("name").toString();
                        }
                        if (xmlReader.name() == "temperature")
                        {
                            QXmlStreamAttributes streamAttributes = xmlReader.attributes();
                            QString FtoC = streamAttributes.value("value").toString();
                            int intFtoC=FtoC.toFloat()-273;
                            mTemperature = QString("%1").arg(intFtoC);
                        }

                        if (xmlReader.name() == "pressure")
                        {
                            QXmlStreamAttributes streamAttributes = xmlReader.attributes();
                            mPressure = streamAttributes.value("value").toString();
                        }

                        if (xmlReader.name() == "speed")
                        {
                            QXmlStreamAttributes streamAttributes = xmlReader.attributes();
                            mWind_speed = streamAttributes.value("value").toString();
                        }

                    }
                    xmlReader.readNext();
                }
            QTimer::singleShot(mRefreshPeriod * 60 * 1000, this, SLOT(requestXml())); //обновление погоды
            emit endRead(this);

        }
         delete netReply;
}

QString OpenWeatherMapCreator::returnLicalId() const
{
    return mLocalId;
}

QString OpenWeatherMapCreator::cityName() const
{
    return mCity;
}

QString OpenWeatherMapCreator::temperature()const
{
    return mTemperature;
}

QString OpenWeatherMapCreator::wind()const
{
    return mWind_speed;
}

QString OpenWeatherMapCreator::pressure()const
{
    return mPressure;
}
