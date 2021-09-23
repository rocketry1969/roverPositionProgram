#pragma once
#include <vector>
#include <sstream>
#include <sqlite3.h>



class database
{
    sqlite3* DB;
    std::stringstream cmd;
    char* zErrMsg;
    int rc;

    public:
        database(const char* dbFile);
        void execCommand(const char* command);
        void insertTableRow(std::string tableName, double values[]);
        void displayTable(const char* tableName);
        void retrieveLastRowData(const char* tableName, double values[]);
        ~database();
};