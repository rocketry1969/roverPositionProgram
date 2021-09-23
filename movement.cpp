#include "movement.h"
#include <math.h>

movement::movement() : db("movements.db")
{
   db.retrieveLastRowData("movements", initializer);
    angle = initializer[1];
    disp  = initializer[2];
    x1    = initializer[3];
    y1    = initializer[4];
    //If the most recent entry is the only entry,
    //then there is no previous position
    if(initializer[0] == 0)
    {
        x0 = 0;
        y0 = 0;
    }else{
        x0 = x1 - disp * cos(angle);
        y0 = y1 - disp * sin(angle);
    }
}

//Used to insert new positional & movement data into
//databse as well as update object variables
void movement::addMovement(double newAngle, double newDisp)
{
    angle = newAngle;
    disp  = newDisp;
    x0 = x1;
    y0 = y1;
    x1 += disp * cos(angle);
    y1 += disp * sin(angle);
    double values[4] = {angle, disp, x1, y1};
    db.insertTableRow("movements", values);
}

//Displays all logged movements from database table
void movement::displayAllMovements(){   db.displayTable("movements");   }

//Prints out relavant current positional data
void movement::displayCurrentPosData()
{
    printf("\nCurrent Position  : (%f, %f)\nPrevious Position : (%f, %f)\nVector Angle : %f radians\nVector Magnitude : %fm\nInitializer = {%f, %f, %f, %f}\n\n", 
    x1, y1, x0, y0, angle, disp, initializer[1], initializer[2], initializer[3], initializer[4]);
}