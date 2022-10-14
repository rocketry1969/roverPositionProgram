#include "functionality.h"
#include <math.h>

// Table Information within database 'movements.db'
// - Name: MOVEMENTS
// - Column Names: ID (Primary Key/INT), RADIUS (DOUBLE), ANGLE (DOUBLE)

// -------------------------- //
// Callback Functions Section //
// -------------------------- //

static int returnRows(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i < argc; i++)
    {
        printf("%s\t", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int updateRow(void *data, int argc, char **argv, char **azColName) {
    double* values = (double*)data;
    //retrieved current position row data
    double newRadiusX = atof(argv[1])*cos(atof(argv[2])) + *(values + 0) * cos( *(values + 1) );
    double newRadiusY = atof(argv[1])*sin(atof(argv[2])) + *(values + 0) * sin( *(values + 1) );
    double newRadius = sqrt(pow(newRadiusX, 2) + pow(newRadiusY, 2));
    
    //Add check to make sure angle output of atan is correct
    double newAngle;

    if(newRadiusX > 0)
        newAngle = atan(newRadiusY/newRadiusX);
    else if(newRadiusX < 0)
    {
        if(newRadiusY > 0)
            newAngle = M_PI - atan(newRadiusY/newRadiusX);
        else if(newRadiusY < 0)
            newAngle = M_PI + atan(newRadiusY/newRadiusX);
    }
    else if(newRadiusX = 0)
    {
        if(newRadiusY > 0)
            newAngle = M_PI;
        else if(newRadiusY < 0)
            newAngle = 3 * M_PI_2;
        else if(newRadiusY = 0)
            newAngle = 0;
    }


    values[0] = newRadius;
    values[1] = newAngle;

    return 0;
}

//Connect to Databse and so State Success of Connection Attempt
void connectToDatabase(sqlite3** DB, const char* filename)
{
    //Open SQLITE3 Database Connection
    int connectionStatus = sqlite3_open(filename, DB);
    if(connectionStatus){
        fprintf(stderr, "Database could not be opened: %s\n", sqlite3_errmsg(*DB));
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
}

//Retrieve and Print all Movement Entries Apart from Current Position (ID=1)
//THIS WORKS!!!!
void listALlEntries(sqlite3* DB)
{
    //Error Message Variable in Event of Problem(s)
    char* zErrMsg;

    //Execute SQLite command and determine success or failure
    if(sqlite3_exec(DB, "SELECT * FROM MOVEMENTS WHERE ID>1", returnRows, 0, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
}

//Return Current Position According to Database (ID=1)
void getCurrentPosition(sqlite3* DB)
{
    //Error Message Variable in Event of Problem(s)
    char* zErrMsg;

    //Execute SQLite command and determine success or failure
    if(sqlite3_exec(DB, "SELECT * FROM MOVEMENTS WHERE ID=1", returnRows, 0, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
}

void addMovementEntry(sqlite3* DB, double* polarValues)
{
    //Error Message Variable in Event of Problem(s)
    //Command for sql commands
    char *zErrMsg, *command;

    //Set command to add entry to MOVEMENTS data table
    sprintf(command, "INSERT INTO MOVEMENTS (RADIUS, ANGLE) VALUES (%lf, %lf)", *(polarValues + 0), *(polarValues + 1));
  
    //Insert Values into Table and determine success or failure
    if(sqlite3_exec(DB, command, returnRows, 0, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
    else printf("[SYSTEM] Entry added to table\n");



    //Retrieve current position data entry
    if(sqlite3_exec(DB, "SELECT * FROM MOVEMENTS WHERE ID=1", updateRow, polarValues, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
    else printf("[SYSTEM] Current position retrieved\n");



    //Set command to update the current position data entry to new radius and angle values
    sprintf(command, "UPDATE MOVEMENTS SET RADIUS=%lf, ANGLE=%lf WHERE ID=1", *(polarValues + 0), *(polarValues+1));
    
    //Update current position with new values
    if(sqlite3_exec(DB, command, 0, 0, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
    else{
        printf("[SYSTEM] Current Position Updated\n");
        printf("[SYSTEM] New Current Coordinates: (%lf, %lf)\n", *(polarValues + 0), *(polarValues+1));
    }
}