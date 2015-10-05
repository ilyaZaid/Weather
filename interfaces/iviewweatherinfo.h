#ifndef IVIEWWEATHERINFO
#define IVIEWWEATHERINFO

#include <QMainWindow>

class iViewWeatherInfo
{
public:
    virtual ~iViewWeatherInfo() {}
    virtual void viewCity(QString city) const = 0;
    virtual void setTemperature(QString city) const = 0;
    virtual void setWind(QString city) const = 0;
    virtual void setPressure(QString city) const = 0;


};
#endif // IVIEWWEATHERINFO

