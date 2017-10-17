#include <vector>
#include <iostream>
#include "Display.h"
#include "algebra3.h"

class Controller{
	private:
		int selected;				// Which face is currently selected
		Display* d;					// A pointer to the display mechanisms
	public:
		Controller(); 
		~Controller();

		int width, height, row, col;
		float red, green, blue;
		mesh* square;

		const int numPoints();				// Get the number of faces
		const vec3 getPoly(int i);		// Get a certain face
		const int getSelected();			// Get which face has been selected
		void select(int i);					// Set the selected face
		void extrudeSelected(float depth);	// Perform an extrusion
};