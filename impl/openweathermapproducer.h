#ifndef OPENWEATHERMAPPRODUCER
#define OPENWEATHERMAPPRODUCER

#include "../interfaces/iweaterinterface.h"
#include "../interfaces/igetweater.h"
#include <QVector>

class OpenWeatherMapProduser : public IWeaterInterface
{
public:
    OpenWeatherMapProduser();

signals:

public slots:

public:
    bool showSettingDialog(QWidget *parent) override;
    QString sourceDescription() const override; //имя источника для меню
    void updateDatabase() override;
    QString weatherProducerId() const override; // вернуть mId
    QVector<QString> returnAllCityId()const;
    QVector<IGetWeater*> vectorCityWeather() override;
    //override-указывает, что элемент типа переопределяет член базового класса или базового интерфейса
private:
    QString mId;
    QVector<IGetWeater*> mCityWeather;
};

#endif // OPENWEATHERMAPPRODUCER
