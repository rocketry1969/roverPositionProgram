#pragma once
#include <vector>
#include <string>
#include "database.h"

class movement
{
    //Angle is measured in radians
    //Displacement is measured in meters
    double x0, y0, x1, y1, angle, disp;
    double initializer[5];
    database db;
        public:
        movement();
        void addMovement(double angle, double disp);
        void displayAllMovements();
        void displayCurrentPosData();
};