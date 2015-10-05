#ifndef IGETWEATER_H
#define IGETWEATER_H


#include <QString>
#include <QObject>

class IGetWeater : public QObject
{
    Q_OBJECT
public:
    explicit IGetWeater(QObject *parent = 0) : QObject(parent) {}
    virtual ~IGetWeater() {}
    virtual void setLocationId(const QString locationId) = 0;
    virtual void setRefreshPeriod(const int refreshPeriod) = 0;
    virtual QString returnLicalId() const = 0;
    virtual QString cityName() const = 0;
    virtual QString temperature() const = 0;
    virtual QString wind() const = 0;
    virtual QString pressure() const = 0;

signals:
    void endRead(IGetWeater*);

};

#endif // IGETWEATER_H
