#include "eController.h"
#include <iostream>

Controller::Controller(){
	//6 quads for the cube

	row = 0;
	col = 0;
	width = 0;
	height = 0;
	selected = 0;
	red = 0;
	green = 0;
	blue = 1;
	square = new mesh();
	d = new Display(10, 10, 800, 600, this);	// Create a new display window
	d->show();									// Show the window
}

Controller::~Controller(){};

const int Controller::numPoints(){							// Get the number of vertices
	return row * col;
}

const int Controller::getSelected(){						// Get which face has been selected
	return selected;
}
void Controller::select(int i){								// Set the selected face
	selected = i; d->updateDrawing();
}
