Ryan Pan


There is a Makefile, run make, the executable is named "hw2".

No command line arguments.

To use the UI, right click the bottom right region of the main window.

Bottom left is XY projection. Top left is YZ projection. Top right is XZ projection.

Two Test files are provided: test.txt and test2.txt


UI STUFF:

Clear All removes all objects on screen and internally destroys them.



Each of the transformations will first ask you to enter the index of the polygon to be transformed. The index will be the order in which the polygons were read in, starting at 0. i.e. the first polygon read in is index 0

-Translate will then ask you for a translation vector in WORLD coordinates, enter it in the form: tx ty tz

-Scale will ask for the scaling factor, enter in the form: alpha beta gamma, where alpha is the x scaling factor, beta is the y scaling factor, and gamma is the z scaling factor

-Rotate will ask for the two points in WORLD coordinates then the degrees, enter the points in the form: x y z
The axis displayed after the rotation will display until another operation is selected from the UI.



File IO will first ask for the name of the file to be loaded/saved. Enter a filename without spaces. i.e. "Save.txt"

-Load will remove all the objects internally and on screen before loading in the contents of the file

-Save will not remove all objects on the screen, just save all objects to the specified file



Under the IO menu there is also an option to manually add a Polyhedron. It will first prompt for the number of vertices. Then after completing the reading of the indices it will prompt for the number of adjacencies and read in adjacencies.

-Vertices should be entered as x y z

-Adjacencies should be entered as v1 v2, where v1 is index vertex 1 and v2 is index vertex 2

-Indices of vertices start at 1, not 0 i.e. the adjacency of the first vertex entered to the third vertex entered would be 1 3

Show Coordinates under IO menu will prompt for an index of polyhedron. It will then print the coordinates of each vertex to the screen. Indices begin at 0.


Start animation will begin rotating all objects around the 0 0 0 to 1 1 1 axis. This will permanently modify the objects internal coordinates. Selecting this option multiple times in a row will increase the speed of the animation.

Stop animation will stop rotating it, but the rotations already applied are permanent.



Implementations:
Poly.cpp rotate line 60-136
Poly.cpp scale line 138-153
Poly.cpp translate line 46-57
Poly.cpp projections line 201-275

main.cpp ndc line 169-211
Poly.cpp ndc line 277-285
Poly.cpp ndc line 177-197
