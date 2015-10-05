#ifndef YANDEXWEATHERCREATOR
#define YANDEXWEATHERCREATOR

#include <QTimer>
#include <QNetworkReply>
#include <QCache>
#include <QList>
#include "../interfaces/igetweater.h"

class YandexWeaterCreator: public IGetWeater
{
    Q_OBJECT
public:
    YandexWeaterCreator();
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
    int mLocalId;
    int retryDelaySec; // время повтора запроса
    QNetworkAccessManager *m_NetworkMngr;
    QString mTemperature;
    QString mPressure;
    QString mCity;
    QString mWind_speed;
    int mRefreshPeriod;
};

#endif // YANDEXWEATHERCREATOR
