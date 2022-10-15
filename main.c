#include "functionality.h"
#include <string.h>

int main(int argc, char *argv[])
{
    //Main program variables for user interaction
    sqlite3* DB;
    char*  zErrMsg;
    const char* databaseFileName = "movements.db";

    //Connect to database and check success status
    connectToDatabase(&DB, databaseFileName);

    //Check which command was passed, if any, and execute corresponding function
    if(argc == 1)                           //No argument given, end program
    {
        printf("No Argument Given\nShutting Down...\n");
    }
    else if(strcmp(argv[1], "-l") == 0)     //List all movement entries aside from current position
    {
        printf("\nID\tRADIUS\tANGLE\n"); //table header for aesthetics in terminal
        listALlEntries(DB);
    }else if(strcmp(argv[1], "-cp") == 0)   //Print out current position from database
    {
        printf("\nID\tRADIUS\tANGLE\n"); //same as with before, aesthetics
        getCurrentPosition(DB);
    }else if(strcmp(argv[1], "-a") == 0)    //Add movement entry from radius and angle given and update current position accordingly
    {
        double values[2] = {atof(argv[2]), atof(argv[3])}; //Retrieve values of {radius, angle} from command prompt and add them to a list
        addMovementEntry(DB, values);
    }else
    {
        printf("Invalid Argument\nShutting Down...\n"); //Argument passed doesn't correspond to any previous, must be mistyped or not a command built for this program
    }
    
    //Disconnect from databse and end program
    sqlite3_close(DB);
    return 0;
} 
