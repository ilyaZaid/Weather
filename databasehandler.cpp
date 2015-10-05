#include "databasehandler.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

DatabaseHandler::DatabaseHandler()
{
  nullTableLocations=true;
}

bool DatabaseHandler::openDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("settings.db");
    bool b = db.open();
    if (b)
    {
        QSqlQuery q(db);
        q.exec("PRAGMA foreign_keys=ON"); //включаем связность таблиц
    }

    return b;
}

bool DatabaseHandler::statusTableLocations()
{
    return nullTableLocations;
}

void DatabaseHandler::updateDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    auto record = db.record("common_settings");
    QSqlQuery q(db);
    if (record.isEmpty()) {


        q.exec("CREATE TABLE common_settings ( "
               "refresh_time INTEGER, "
               "show_delay   INTEGER "
               ");");

    }

    record = db.record("locations");
    if (record.isEmpty()) {


        q.exec("CREATE TABLE locations ( "
               "id           INTEGER PRIMARY KEY AUTOINCREMENT, "
               "descriptions VARCHAR);");
    }
    else{

    // если таблица была создана, выводим сообщение о том что город не указан
        nullTableLocations=false;
    }


    q.exec("select count (*) from common_settings");
    q.next();
    if (q.value(0).toInt() == 0) {
        q.exec("insert into common_settings (refresh_time, show_delay) values (600, 2)");
        // если в таблице нет настроек то добавляем стандартные
    }
}

int DatabaseHandler::refreshTime() const
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.exec("select refresh_time,show_delay FROM common_settings");
    if (q.next()) {
        return q.value(0).toInt();
    }
    return 600;
}

int DatabaseHandler::showDelay() const
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.exec("select refresh_time, show_delay FROM common_settings");
    if (q.next()) {
        return q.value(1).toInt();
    }
    return 2;
}

void DatabaseHandler::setRefreshTimeAndShowDelay(const int refreshTime, const int showDelay)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.prepare("UPDATE common_settings set refresh_time = ?, show_delay = ?");
    q.addBindValue(refreshTime);
    q.addBindValue(showDelay);
    q.exec();
}

