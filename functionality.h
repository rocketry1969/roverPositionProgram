#pragma once
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

//Declaration statements for all the functions used to interact with the database from main()
void connectToDatabase(sqlite3** DB, const char* filename);
void listALlEntries(sqlite3* DB);
void getCurrentPosition(sqlite3* DB);
void addMovementEntry(sqlite3* DB, double* polarValues);