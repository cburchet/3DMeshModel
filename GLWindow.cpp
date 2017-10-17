#include "GLWindow.h"
#include "Display.h"


GLWindow::GLWindow(int x, int y, int w, int h) : Fl_Gl_Window(x, y, w, h){
	c= new Camera(4, 0, 0);	// create a camera
	square = new mesh();
	red = 0;
	green = 0;
	blue = 0;
	rows = 0;
	cols = 0;
	selected = -1;
	//bools for whether the mesh is wireframe, filled, or both
	wire = true;
	solid = false;
}

void GLWindow::init(){	// initialize the required OpenGL transforms
	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 10);		// experiment with these settings to see their effects
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
}

void GLWindow::draw(){
	if (!valid()){
		init();				// make sure the OpenGL settings are completed once before drawing
		valid(1);
	}
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	vec4 eye = c->getEye();
	vec4 up = c->getUp();
	//sets the camera at position eye, looking at origin with the up direction
	gluLookAt(eye[0], eye[1], eye[2], 0, 0, 0, up[0], up[1], up[2]); // camera control
	Fl_Group *g = this->parent();	// get a pointer to the FL display window
	Display *d = (Display*)(g);
	int currentRow = 0;
	int currentCol = 0;
	//draws small symbol at selected vertex
	if (selected >= 0)
	{
		currentRow = selected / this->cols;
		currentCol = selected % this->cols;
		glBegin(GL_QUADS);
			glColor3f(green, blue, red);
			glVertex3f((square->square[currentRow][currentCol])[0] + .1, (square->square[currentRow][currentCol])[1], (square->square[currentRow][currentCol])[2]);
			glVertex3f((square->square[currentRow][currentCol])[0] - .1, (square->square[currentRow][currentCol])[1], (square->square[currentRow][currentCol])[2]);
			glVertex3f((square->square[currentRow][currentCol])[0], (square->square[currentRow][currentCol])[1], (square->square[currentRow][currentCol])[2] + .1);
			glVertex3f((square->square[currentRow][currentCol])[0], (square->square[currentRow][currentCol])[1], (square->square[currentRow][currentCol])[2] - .1);
		glEnd();
	}

	//if solid is chosen mesh will be solid
	if (solid)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//if wire is true and solid is false wireframe mesh
	else if (wire && !solid)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			glBegin(GL_TRIANGLES);
			//color chosen by user for mesh
			glColor3f(red, green, blue);
			if (i % 2 == 0 && i <= rows-1 && j < cols - 1)
			{
				//triangles of even rows
				if (i < rows - 1)
				{
					glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
					glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
					glVertex3f((square->square[i + 1][j])[0], (square->square[i + 1][j])[1], (square->square[i + 1][j])[2]);
				}
				if (i != 0)
				{
					glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
					glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
					glVertex3f((square->square[i - 1][j+1])[0], (square->square[i - 1][j+1])[1], (square->square[i - 1][j+1])[2]);
				}
			}
			else if (i % 2 != 0 && j < cols - 1)
			{
				//triangles of odd rows
				glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
				glVertex3f((square->square[i][j+1])[0], (square->square[i][j+1])[1], (square->square[i][j+1])[2]);
				glVertex3f((square->square[i-1][j + 1])[0], (square->square[i-1][j + 1])[1], (square->square[i-1][j + 1])[2]);
				
				if (i != rows - 1)
				{
					glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
					glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
					glVertex3f((square->square[i + 1][j])[0], (square->square[i + 1][j])[1], (square->square[i + 1][j])[2]);
				}
			}
			glEnd();
		}
	}

	//if wire and filled are true mesh will be drawn again as wireframe and offset by 1 
	if (wire && solid)
	{
		//enables and offsets polygon to prevent flickering
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(1, 1);

		//draw wireframe of mesh
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				glBegin(GL_TRIANGLES);
				//switch color values to prevent wireframe blending with filled mesh
				glColor3f(blue, red, green);
				if (i % 2 == 0 && i < rows - 1 && j < cols - 1)
				{
					//triangles for even numbered rows
					glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
					glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
					glVertex3f((square->square[i + 1][j])[0], (square->square[i + 1][j])[1], (square->square[i + 1][j])[2]);
					
					//prevent error if on first row
					if (i != 0)
					{
						glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
						glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
						glVertex3f((square->square[i - 1][j + 1])[0], (square->square[i - 1][j + 1])[1], (square->square[i - 1][j + 1])[2]);
					}
				}
				else if (i % 2 != 0 && j < cols - 1)
				{
					//triangles for odd numbered rows
					glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
					glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
					glVertex3f((square->square[i - 1][j + 1])[0], (square->square[i - 1][j + 1])[1], (square->square[i - 1][j + 1])[2]);

					//prevent error if on last column
					if (i != cols - 1)
					{
						glVertex3f((square->square[i][j])[0], (square->square[i][j])[1], (square->square[i][j])[2]);
						glVertex3f((square->square[i][j + 1])[0], (square->square[i][j + 1])[1], (square->square[i][j + 1])[2]);
						glVertex3f((square->square[i + 1][j])[0], (square->square[i + 1][j])[1], (square->square[i + 1][j])[2]);
					}
				}
				glEnd();
			}
		}
		//disable offset
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
}

int GLWindow::handle(int event){		// handle keyboard events
	if (event == FL_KEYUP){				// when the key is released, check for an event and handle it
		if (Fl::event_key() == 'a'){
			c->increasePhi(-7);
			redraw();
		}
		if (Fl::event_key() == 'd'){
			c->increasePhi(7);
			redraw();
		}
		if (Fl::event_key() == 'w'){
			c->increaseTheta(-7);
			redraw();
		}
		if (Fl::event_key() == 's'){
			c->increaseTheta(7);
			redraw();
		}
		if (Fl::event_key() == 'e'){
			c->increaseRad(-.5);					
			redraw();
		}
		if (Fl::event_key() == 'f'){
			c->increaseRad(.5);					
			redraw();
		}
	}
	return Fl_Gl_Window::handle(event);
}