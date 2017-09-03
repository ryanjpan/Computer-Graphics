Use make to create the executable, it should be named hw3.

UI:

Right click in the bottom right area of the window for a menu.

I/O has 2 functions
-Remove all clears all objects from the screen and internally deletes them.

-Load file will prompt for a filename i.e. "test.txt" It will then load in the test file and apply phong with the default parameters.
Note that load will first remove all objects from screen then load the file.

A test file phongtest.txt has been provided.

*****DEFAULT VALUES*****
float fx = 12; // from point
float fy = 12;
float fz = 12;
float kar = .8; // k ambient
float kag = .8;
float kab = .8;
float ksr = .8; // k specular
float ksg = .8;
float ksb = .8;
float kdr = .8; // k diffuse
float kdg = .8;
float kdb = .8;
float lx = 10; // light location
float ly = 10;
float lz = 10;
float Iar = .8; // intensity ambient
float Iag = .8;
float Iab = .8;
float ILr = .9; // intensity light
float ILg = .9;
float ILb = .9;
int PN = 3; // phong constant
**************************

Phong Submenu has 5 options
-Change Intensity allows the user to change I_a and I_L. Enter both as r g b i.e. .5 .5 .5

-Change From point allows user to change the from point. Enter the point in world coordinates as x y z i.e. 10 10 10

-Change K values prompts the user for Ka then Ks then Kd as r g b triples i.e. .2 .2 .2

-Change Light location prompts user for light location in world coordinates x y z i.e. 10 10 10

-Change Phong Constant prompts user for integer value i.e. 2

***Note colors are normalized after Phong is calculated.


Halftone Submenu

-Halftone On will halftone all 3 buffers

-Halftone Off will project the scene with previous phong values


ALGORITHMS (line numbers)

-Halftone Helper.cpp 111 - 323

-Phong Calculation Poly.cpp 296 - 345

-Painters main.cpp 181 - 224

-Painters cont. Poly.cpp (sorting faces within polygon) 38 - 132
 
-Linear Interpolation (Gouraud shading) Line.cpp 438 - 554
