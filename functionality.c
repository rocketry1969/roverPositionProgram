#include "functionality.h"
#include <math.h>

// Table Information within database 'movements.db'
// - Name: MOVEMENTS
// - Column Names: ID (Primary Key/INT), RADIUS (DOUBLE), ANGLE (DOUBLE)

// -------------------------- //
// Callback Functions Section //
// -------------------------- //

//Callback function for retrieving data from database rows and printing them out to terminal
static int returnRows(void *NotUsed, int argc, char **argv, char **azColName) {
    //Print out value in each column in table, separated by tabs, with this loop running each time another row is retrieved from database table
    for(int i = 0; i < argc; i++)
    {
        printf("%s\t", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

//Callback function for retrieving current position data and updating it using arguments passed via terminal
static int updateRow(void *data, int argc, char **argv, char **azColName) {
    //Gain access to angle and radius values entered into terminal with program execution
    double* values = (double*)data;

    //Calculate new current position in terms of cartesian coordinates by finding components of overall displacement vector
    double newRadiusX = atof(argv[1])*cos(atof(argv[2])) + *(values + 0) * cos( *(values + 1) );
    double newRadiusY = atof(argv[1])*sin(atof(argv[2])) + *(values + 0) * sin( *(values + 1) );

    //Determine magnitude of new displacement vector (new current position)
    double newRadius = sqrt(pow(newRadiusX, 2) + pow(newRadiusY, 2));
    

    //Declare variable to store new angle of displacement vector
    double newAngle;

    //Code block for determining positive angle between displacement vector and positive x-axis of theoretical cartesian plane
    //Needed owing to innate limitations of the arctangent function
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

    //Update values list to have updated values for current position radius and angle values
    values[0] = newRadius;
    values[1] = newAngle;

    return 0;
}

// --------------------------- //
// Function Definition Section //
// --------------------------- //

//Connect to database and so state success of connection attempt
void connectToDatabase(sqlite3** DB, const char* filename)
{
    //Open SQLITE3 database connection
    int connectionStatus = sqlite3_open(filename, DB);

    //Check status of connection attempt
    if(connectionStatus){
        fprintf(stderr, "Database could not be opened: %s\n", sqlite3_errmsg(*DB));
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
}

//Retrieve and print all movement entries apart from current position (ID=1)
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

//Return current position according to database (ID=1)
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

//Add movement entry from arguments passed in terminal and update current position using this information
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


    //Retrieve now outdated current position data entry
    if(sqlite3_exec(DB, "SELECT * FROM MOVEMENTS WHERE ID=1", updateRow, polarValues, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
    else printf("[SYSTEM] Current position retrieved\n");


    //Set command to update the current position data entry to new radius and angle values calculated in previous command's callback 'updateRow'
    sprintf(command, "UPDATE MOVEMENTS SET RADIUS=%lf, ANGLE=%lf WHERE ID=1", *(polarValues + 0), *(polarValues+1));
    //Update current position with new values and determine success or failure
    if(sqlite3_exec(DB, command, 0, 0, &zErrMsg))
    { 
        printf("Command Failed\n");
        fprintf(stderr, "[ERROR] %s\n", sqlite3_errmsg(DB));
    }
    else{
        //State position update success and print out new current position
        printf("[SYSTEM] Current Position Updated\n");
        printf("[SYSTEM] New Current Coordinates: (%lf, %lf)\n", *(polarValues + 0), *(polarValues+1));
    }
}