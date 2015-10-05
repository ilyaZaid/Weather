#include "yandexweathercreator.h"
#include <QXmlStreamReader>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

YandexWeaterCreator :: YandexWeaterCreator()
{
mLocalId = 27113;
retryDelaySec = 1;
m_NetworkMngr = new QNetworkAccessManager(this);
connect(m_NetworkMngr,SIGNAL(finished(QNetworkReply*)),
         this, SLOT(readXml(QNetworkReply*)));

mTemperature = "Данные не загружены";
mPressure = "Данные не загружены";
mCity = "Данные не загружены";
mWind_speed = "Данные не загружены";

//
QTimer::singleShot(0, this, SLOT(requestXml())); //можно поставить ограничение если не задан город

}


void YandexWeaterCreator::setLocationId(const QString locationId)
{
    mLocalId = locationId.toInt();
    QTimer::singleShot(0, this, SLOT(requestXml()));
}

void YandexWeaterCreator::setRefreshPeriod(const int refreshPeriod)
{
  mRefreshPeriod =  refreshPeriod;
}



void YandexWeaterCreator::requestXml()
{
    QString urlForXML = QString("http://export.yandex.ru/weather-ng/forecasts/%0.xml").arg(mLocalId);
    m_NetworkMngr->get(QNetworkRequest(urlForXML));

}

void YandexWeaterCreator::readXml(QNetworkReply *netReply)

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

            while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement
                     && xmlReader.name() == "fact"))
                {
                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
                    {

                        if (xmlReader.name() == "forecast")
                        {
                            QXmlStreamAttributes streamAttributes = xmlReader.attributes();
                            mCity = streamAttributes.value("city").toString();
                        }
                        if (xmlReader.name() == "temperature")
                        {
                            mTemperature = xmlReader.readElementText();
                        }
                        if (xmlReader.name() == "pressure")
                        {
                            mPressure = xmlReader.readElementText();
                        }
                        if (xmlReader.name() == "wind_speed")
                        {
                            mWind_speed = xmlReader.readElementText();
                        }
                    }
                    xmlReader.readNext();
                }
            QTimer::singleShot(mRefreshPeriod * 60 * 1000, this, SLOT(requestXml())); //обновление погоды
            emit endRead(this);

        }
         delete netReply;
}

QString YandexWeaterCreator::returnLicalId() const
{
    QString MyString = QString("%1").arg(mLocalId);
    return MyString;
}

QString YandexWeaterCreator::cityName() const
{
    return mCity;
}

QString YandexWeaterCreator::temperature()const
{
    return mTemperature;
}

QString YandexWeaterCreator::wind()const
{
    return mWind_speed;
}

QString YandexWeaterCreator::pressure()const
{
    return mPressure;
}
