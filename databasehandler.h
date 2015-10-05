#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>

class DatabaseHandler
{
public:
    DatabaseHandler();
    bool openDb();
    bool statusTableLocations();
    void updateDatabase();

    int refreshTime() const;
    int showDelay() const;
    void setRefreshTimeAndShowDelay(const int refreshTime, const int showDelay);

private:
    bool nullTableLocations;

};

#endif // DATABASEHANDLER_H
