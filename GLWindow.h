#ifndef GL_WINDOW_H
#define GL_WINDOW_H 
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Gl.H>
#include <GL/glu.h>
#include <Fl/Fl.H>
#include <iostream>
#include "camera.h"
#include "mesh.h"
#include "algebra3.h"

class GLWindow : public Fl_Gl_Window{
	private:					// a pointer to the camera
		void init();				// initialize the required OpenGL transforms
		void draw();				// draw the faces in the OpenGL window
		int handle(int event);	 // handle keyboard events
	public:
		Camera* c;
		GLWindow(int x, int y, int w, int h); 
		int rows, cols, selected;
		bool wire, solid;
		float red, green, blue;
		mesh* square;
};

#endif