#include <iostream>
#include "database.h"
#include "movement.h"

int main()
{
    database db("movements.db");
    std::string command;
    movement move;
    int menuRes;
    std::cout << "Menu\nView Current Positional Data (1)\nDatabase Interface(2)\nEnter Choice: ";
    std::cin >> menuRes;
    switch(menuRes)
    {
        case 1:
            move.displayCurrentPosData();
            break;
        case 2:
            while(true)
            {
                std::cout << "Enter SQLITE3 Command: ";
                std::getline(std::cin, command);
                if(command == "exit") break;
                db.execCommand(command.c_str());
            }
            break; 

    }
    return 0;
}