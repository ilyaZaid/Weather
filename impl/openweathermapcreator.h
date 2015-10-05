#ifndef OPENWEATHERMAPCREATOR
#define OPENWEATHERMAPCREATOR

#include <QTimer>
#include <QNetworkReply>
#include <QCache>
#include <QList>
#include "../interfaces/igetweater.h"

class OpenWeatherMapCreator: public IGetWeater
{
    Q_OBJECT
public:
    OpenWeatherMapCreator();
    void setLocationId(const QString locationId) override;
    void setRefreshPeriod(const int refreshPeriod) override;
    QString returnLicalId() const override;
    QString cityName() const override;
    QString temperature() const override;
    QString wind() const override;
    QString pressure() const override;


signals:
    void endRead();
    void endRead(IGetWeater*);
private slots:
    void requestXml();
    void readXml(QNetworkReply *netReply);

private:
    QString mLocalId;
    int retryDelaySec; // время повтора запроса
    QNetworkAccessManager *m_NetworkMngr;
    QString mTemperature;
    QString mPressure;
    QString mCity;
    QString mWind_speed;
    int mRefreshPeriod;
};


#endif // OPENWEATHERMAPCREATOR

