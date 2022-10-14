# Positioning Program
Determine the position of a moving vehicle by logged polar coordinates

# Commands
In order to use the program there are three commands that you can use

Commands
1. -cp
2. -l 
3. -a (radius) (angle)

The first command will print out the current position of the rover relative to the origin of before it moved at all, according to the data logged in the table.

The second command will show all the movements taken by the object from when it first powered on. In the database table, the current position is held in the first data entry, so the table ids printed to the console will be numbered two or greater.

The third command will take two arguments along with it, radius and angle. These will represent the angle and magnitude of the displacement vector of the object from its previous position. It will add this entry to the database and update the current position of the vehicle relative to its starting point accordingly.


# Compilation and Execution
In order to compile and execute the program, run the following commands (when using gcc)

```console
user@device:~/Program/Folder/$ gcc main.c functionality.c -lm -lsqlite3 -o Output
user@device:~/Program/Folder/$ ./Output [-cp | -l | -a radius angle]
```