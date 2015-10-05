#include "yandexweaterproducer.h"
#include "yandexweatersettingdialog.h"
#include "yandexweathercreator.h"
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QUuid>

YandexWeaterProducer::YandexWeaterProducer() :
    mId(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex()))
{
    /*
    1) fromLatin1(str,size)-Возвращает QString, инициализированную
       первыми size символами строки str в кодах Latin-1.
       Если размер size равен -1 (по умолчанию), то длинна
       подсчитывается при помощи qstrlen(str).
    2) Latin-1 кодировка (пример 9=57)
    3) QUuid класс хранит универсальный уникальный идентификатор(UUID);
    4) Rfc4122 стандарт UUID (число в 16 ричной системе);
    5) toHex()- метод класса QByteArray. Возвращает шестнадцатеричную
    копию массива байтов. В виде набора цифр от 0-9 и буквы  от A-F
*/
}

bool YandexWeaterProducer::showSettingDialog(QWidget *parent = nullptr)
{
    YandexWeaterSettingDialog dlg(parent);
    return (dlg.exec() == QDialog::Accepted);
    // если все ок переписываю вектор


}

QString YandexWeaterProducer::sourceDescription() const
{
    return tr("Яндекс погода");
}

QVector<IGetWeater*> YandexWeaterProducer::vectorCityWeather()
{
    QVector<QString> vectorWithId=returnAllCityId();
    QVector<IGetWeater*> newCityWeather;
    bool overlap = false;

    for (int i=0; i<=vectorWithId.size()-1;i++)
    {
        for (int j=0; j<=mCityWeather.size()-1;j++)
        {
            if (mCityWeather[j]->returnLicalId() == vectorWithId[i])
            {
                newCityWeather << mCityWeather[j];
                overlap = true;
            }
        }
            if(!overlap)
            {
                newCityWeather <<new YandexWeaterCreator();
                newCityWeather[i]->setLocationId(vectorWithId[i]);
            }
         overlap = false;
     }

    mCityWeather = newCityWeather;

    return (mCityWeather);
}




void YandexWeaterProducer::updateDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    auto record = db.record("yandex_info");
    QSqlQuery q(db);
    if (record.isEmpty()) {
        q.exec("CREATE TABLE yandex_info ( "
               "id            INTEGER PRIMARY KEY AUTOINCREMENT, "
               "location_id   INTEGER REFERENCES locations (id) ON DELETE CASCADE "
               "ON UPDATE CASCADE,"
               "yandex_loc_id VARCHAR"
               ");");
    }

}

QVector<QString> YandexWeaterProducer::returnAllCityId() const
{

    QVector<QString> vectorForId;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.exec("select yandex_info.yandex_loc_id from yandex_info");
    while (q.next())
    {
        vectorForId<<q.value(0).toString();
    }
    return vectorForId;
}


QString YandexWeaterProducer::weatherProducerId() const
{
    return mId;
}

