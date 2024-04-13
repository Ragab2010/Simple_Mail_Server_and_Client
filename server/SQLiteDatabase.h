#ifndef SQLITE_DATABASE_H
#define SQLITE_DATABASE_H

#include <iostream>
#include <sqlite3.h>

class SQLiteDatabase {
private:
    sqlite3 *db;
    char *errMsg;

public:
    SQLiteDatabase();
    void openDatabase(const char *dbName);
    void createTable();
    void storeMessage(const char *sender, const char *receiver, const char *message);
    std::string retrieveSentMessages(std::string sender);
    std::string retrieveReceivedMessages(std::string receiver);
    void closeDatabase();
};

#endif // SQLITE_DATABASE_H
