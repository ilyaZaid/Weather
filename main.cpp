#include "mainwindow.h"
#include <QApplication>

#include "weaterhandler.h"
#include "impl/yandexweaterproducer.h"
#include "impl/yandexweathercreator.h"
#include "impl/openweathermapproducer.h"
#include "impl/openweathermapcreator.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    WeaterHandler h;

    YandexWeaterProducer yandex;
    OpenWeatherMapProduser openMap;


    h.setMainWindow(&w);
    h.setCommonSettingAction(w.commonSettingAction());
    h.registerProducer(&yandex);
    h.registerProducer(&openMap);
    w.setWeatherHandler(&h);

    h.registerViewWeather(&w);
    w.setSourceSetupMenu(h.sourceSettingMenu());
    w.show();


    return a.exec();
}
