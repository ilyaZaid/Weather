#ifndef IWEATERINTERFACE_H
#define IWEATERINTERFACE_H

#include <QObject>
#include "iweaterinterface.h"
#include "igetweater.h"

class IWeaterInterface : public QObject
{
    Q_OBJECT
public:
    explicit IWeaterInterface(QObject *parent = 0) : QObject(parent) {}
    virtual ~IWeaterInterface() {}
    virtual bool showSettingDialog(QWidget *parent = nullptr) = 0;
    virtual QString sourceDescription() const = 0;
    virtual void updateDatabase() = 0;
    virtual QString weatherProducerId() const = 0;
    virtual QVector<IGetWeater*> vectorCityWeather() = 0;

public slots:
};

#endif // IWEATERINTERFACE_H
