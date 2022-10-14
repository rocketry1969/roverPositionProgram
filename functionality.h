#pragma once
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

void connectToDatabase(sqlite3** DB, const char* filename);
void listALlEntries(sqlite3* DB);
void getCurrentPosition(sqlite3* DB);
void addMovementEntry(sqlite3* DB, double* polarValues);