#ifndef YANDEXWEATERPRODUCER_H
#define YANDEXWEATERPRODUCER_H

#include "../interfaces/iweaterinterface.h"
#include "../interfaces/igetweater.h"
#include <QVector>

class YandexWeaterProducer : public IWeaterInterface
{
public:
    YandexWeaterProducer();

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

#endif // YANDEXWEATERPRODUCER_H
