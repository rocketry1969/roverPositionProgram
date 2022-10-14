#include "functionality.h"
#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i < argc; i++){
    printf("%s\t", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{

    //Main program variables for user interaction
    sqlite3* DB;
    char*  zErrMsg;
    const char* databaseFileName = "movements.db";
    connectToDatabase(&DB, databaseFileName);
    if(argc == 1)
    {
        printf("No Argument Given\nShutting Down...\n");
    }
    else if(strcmp(argv[1], "-l") == 0)
    {
        printf("\nID\tRADIUS\tANGLE\n");
        listALlEntries(DB);
    }else if(strcmp(argv[1], "-cp") == 0){
        printf("\nID\tRADIUS\tANGLE\n");
        getCurrentPosition(DB);
    }else if(strcmp(argv[1], "-a") == 0){ //Some major problem with implementation here; look at later
        double values[2] = {atof(argv[2]), atof(argv[3])};
        addMovementEntry(DB, values);
    }else{
        printf("Invalid Argument\nShutting Down...\n");
    }
    // Archetype for listAllEntries, WORKS!!!!
    

 
    //Disconnect from databse and end program
    sqlite3_close(DB);
    return 0;
} 
