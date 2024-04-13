#include "SQLiteDatabase.h"
static int callback(void *data, int argc, char **argv, char **azColName);

SQLiteDatabase::SQLiteDatabase() : db(nullptr), errMsg(nullptr) {}

// SQLite callback function for retrieving data
static int callback(void *data, int argc, char **argv, char **azColName) {
    std::string* result = static_cast<std::string*>(data);
    bool rowsProcessed = false; // Flag to track if any rows have been processed
    *result +="----------------------------------\n";
    for (int i = 0; i < argc; i++) {
        if (argv[i]) {
            *result += azColName[i];
            *result += ": ";
            *result += argv[i];
            *result += "\n";
            rowsProcessed = true; // Set the flag to true if any row is processed
        }
    }

    if (!rowsProcessed) {
        *result += "No results found.\n"; // Append a message if no rows are processed
    }
    *result +="----------------------------------\n";

    return 0;
}
void SQLiteDatabase::openDatabase(const char *dbName) {
    int rc = sqlite3_open(dbName, &db);
    if (rc) {
        std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
}

void SQLiteDatabase::createTable() {
    const char *sql = "CREATE TABLE IF NOT EXISTS Messages("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "sender TEXT NOT NULL,"
                      "receiver TEXT NOT NULL,"
                      "message TEXT NOT NULL,"
                      "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
                      ");";

    int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
}

void SQLiteDatabase::storeMessage(const char *sender, const char *receiver, const char *message) {
    char *sql = sqlite3_mprintf("INSERT INTO Messages (sender, receiver, message) VALUES ('%q', '%q', '%q');",
                                 sender, receiver, message);

    int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

std::string SQLiteDatabase::retrieveSentMessages(std::string sender) {
    //std::string sentMessages;
    // sqlite3_stmt *stmt;
    // const char *sql = "SELECT * FROM Messages WHERE sender = ?;";
    // int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    // if (rc != SQLITE_OK) {
    //     std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    //     return sentMessages;
    // }

    // sqlite3_bind_text(stmt, 1, sender, -1, SQLITE_STATIC);

    // while (sqlite3_step(stmt) == SQLITE_ROW) {
    //     sentMessages += reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)); // Assuming message is stored in column 3
    //     sentMessages += "\n";
    // }

    // sqlite3_finalize(stmt);
    // return sentMessages;
    /****************************************/
    std::string sql = "SELECT * FROM Messages WHERE sender LIKE \""+sender+"\";";
    std::string sentMessages;
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &sentMessages, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // If no messages were found, append a message to the result
    if (sentMessages.empty()) {
        sentMessages = "No messages found for the specified sender.";
    }

    return sentMessages;
}

std::string SQLiteDatabase::retrieveReceivedMessages(std::string receiver) {
    // std::string receivedMessages;
    // sqlite3_stmt *stmt;
    // const char *sql = "SELECT * FROM Messages WHERE receiver = ?;";
    // int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    // if (rc != SQLITE_OK) {
    //     std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    //     return receivedMessages;
    // }

    // sqlite3_bind_text(stmt, 1, receiver, -1, SQLITE_STATIC);

    // while (sqlite3_step(stmt) == SQLITE_ROW) {
    //     receivedMessages += reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)); // Assuming message is stored in column 3
    //     receivedMessages += "\n";
    // }

    // sqlite3_finalize(stmt);
    // return receivedMessages;
    /****************************************/
    std::string sql = "SELECT * FROM Messages WHERE receiver LIKE \""+receiver+"\";";
    std::string receivedMessages;
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &receivedMessages, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // If no messages were found, append a message to the result
    if (receivedMessages.empty()) {
        receivedMessages = "No messages found for the specified receiver.";
    }

    return receivedMessages;
}

void SQLiteDatabase::closeDatabase() {
    sqlite3_close(db);
}
