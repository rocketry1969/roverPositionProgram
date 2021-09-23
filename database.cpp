#include "database.h"
#include <iostream>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   for(int i = 0; i<argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
static int tableRowCallback(void* veryUsed, int argc, char** argv, char** azColName)
{
   int i;
   double* vals = (double*)veryUsed;
   for(i = 0; i < argc; i++)
   {
      *(vals + i) = std::atof(*(argv + i));
   }
   return 0;
}

//Open connection to database file
database::database(const char* dbFile){ sqlite3_open(dbFile, &DB);  }

void database::execCommand(const char* command)
{
   //Execute SQLITE3 Command
    rc = sqlite3_exec(DB, command, callback, 0, &zErrMsg);
    
    //Check if the execution had errors
    if(rc) std::cout << "SQLITE ERROR : " << sqlite3_errmsg(DB) << std::endl;
    else   std::cout << "Command Executed Successfully\n";
}
void database::insertTableRow(std::string tableName, double values[])
{

   int i;
   cmd << "INSERT INTO " << tableName;
   if(tableName == "movement") 
      cmd << " (angle, disp, x, y) values (" << values[0] << ", " << values[1] << ", "<< values[2] << ", " << values[3] <<")";
   else return;
   
   //Same execute command and check for errors
   rc = sqlite3_exec(DB, cmd.str().c_str(), callback, 0, &zErrMsg);
   if(rc) std::cout << "SQLITE ERROR : " << sqlite3_errmsg(DB) << std::endl;
   else   std::cout << "Command Executed Successfully\n";
   
   //Empty cmd for next use
   cmd.clear();
}
void database::displayTable(const char* tableName)
{
   //Insert the select all command into cmd
   cmd << "SELECT * FROM " << tableName;

   //Executre that command and check for errors
   rc = sqlite3_exec(DB, cmd.str().c_str(), callback, 0, &zErrMsg);
   if(rc) std::cout << "SQLITE ERROR : " << sqlite3_errmsg(DB) << std::endl;
   
   //Emplty cmd for next use
   cmd.clear();
}

//Retrieve data from the table's row with the largest ID
void database::retrieveLastRowData(const char* tableName, double values[])
{
   cmd << "SELECT * FROM " << tableName << " ORDER BY id DESC LIMIT 1";
   rc = sqlite3_exec(DB, cmd.str().c_str(), tableRowCallback, values, &zErrMsg);
   cmd.clear();
}

//Close connection to databse in destructor
database::~database(){  sqlite3_close(DB);  }