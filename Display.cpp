#include "Display.h"
#include "eController.h"

Display::Display(int x, int y, int w, int h, Controller* c):Fl_Window(x, y, w, h, "Extrudor"){
		this->c = c;
		win = new GLWindow(400, 0, 400, 400);
		browser = new Fl_Hold_Browser(250, 0, 150, 400);
		browser->callback((void(*)(Fl_Widget*, void*))selectCB, this);
		makeMesh = new Fl_Button(0, 60, 75, 30, "Create");
		makeMesh->callback((Fl_Callback*)makeCB, this);
		randomMesh = new Fl_Button(75, 60, 150, 30, "Random Mesh");
		randomMesh->callback((Fl_Callback*)randomCB, this);
		rows = new Fl_Value_Input(40, 100, 70, 30, "Rows");
		rows->when(FL_WHEN_ENTER_KEY);
		rows->callback((Fl_Callback*)rowsCB, (void*)c);
		columns = new Fl_Value_Input(180, 100, 70, 30, "Columns");
		columns->when(FL_WHEN_ENTER_KEY);
		columns->callback((Fl_Callback*)columnsCB, (void*)c);
		meshWidth = new Fl_Value_Input(40, 150, 80, 30, "Width");
		meshWidth->when(FL_WHEN_ENTER_KEY);
		meshWidth->callback((Fl_Callback*)widthCB, (void*)c);
		meshHeight = new Fl_Value_Input(170, 150, 80, 30, "Height");
		meshHeight->when(FL_WHEN_ENTER_KEY);
		meshHeight->callback((Fl_Callback*)heightCB, (void*)c);
		meshColors = new Fl_Color_Chooser(50, 200, 200, 100, "Colors");
		meshColors->callback((Fl_Callback*)colorsCB, this);

		//sliders to change position
		slideX = new Fl_Value_Slider(50, 400, 50, 100, "X slider");
		slideX->minimum(-10);
		slideX->maximum(10);
		slideX->hide(); 
		slideX->callback((Fl_Callback*)slidexCB, this);
		slideY = new Fl_Value_Slider(100, 400, 50, 100, "Y slider");
		slideY->hide();
		slideY->callback((Fl_Callback*)slideyCB, this);
		slideZ = new Fl_Value_Slider(150, 400, 50, 100, "Z slider");
		slideZ->minimum(-10);
		slideZ->maximum(10);
		slideZ->hide();
		slideZ->callback((Fl_Callback*)slidezCB, this);

		//buttons for draw style
		wire = new Fl_Round_Button(10, 325, 30, 30, "Wireframe");
		solid = new Fl_Round_Button(95, 325, 30, 30, "Solid");
		both = new Fl_Round_Button(150, 325, 30, 30, "Both");

		wire->callback((Fl_Callback*)wireCB, this);
		solid->callback((Fl_Callback*)solidCB, this);
		both->callback((Fl_Callback*)bothCB, this);

		//buttons for different viewing angles
		sideView = new Fl_Button(0, 0, 75, 30, "Side");
		sideView->callback((Fl_Callback*)sideCB, this);
		topView = new Fl_Button(0, 30, 75, 30, "Top");
		topView->callback((Fl_Callback*)topCB, this);

		//widgets for subdivision/fractalization
		divideAmount = new Fl_Value_Slider(250, 425, 50, 100, "Divide");
		divideAmount->minimum(0);
		divideAmount->maximum(5);
		divideAmount->step(1);
		divideAmount->hide();
		//divideAmount->callback((Fl_Callback*)divideCB, this);
		subdivide = new Fl_Button(325, 425, 75, 30, "Subdivide");
		subdivide->hide();
		subdivide->callback((Fl_Callback*)subdivideCB, this);
		fractal = new Fl_Button(400, 425, 75, 30, "Fractal");
		fractal->hide();
		fractal->callback((Fl_Callback*)fractalCB, this);

		this->end();
}

void Display::colorsCB(Fl_Color_Chooser* colors, void* data)
{
	Display* d = (Display*)data;
	//sets c's colors values to current rgb values
	d->c->red = colors->r();
	d->c->green = colors->g();
	d->c->blue = colors->b();
	d->win->red = d->c->red;
	d->win->green = d->c->green;
	d->win->blue = d->c->blue;
	d->win->redraw();
}

void  Display::randomCB(Fl_Button* button, void* data)
{
	Display* d = (Display*)data;
	//get random numbers for mesh size and color
	int rows = ((rand() / (float)(RAND_MAX)) * 6) + 2;
	int cols = ((rand() / (float)(RAND_MAX))* 6) + 2;
	int height = ((rand() / (float)(RAND_MAX))* 6) + 2;
	int width = ((rand() / (float)(RAND_MAX)) * 6) + 2;
	float red = ((float)rand() / (float)(RAND_MAX));
	float green = ((float)rand() / (float)(RAND_MAX));
	float blue = ((float)rand() / (float)(RAND_MAX));

	d->c->square = new mesh(rows, cols, width, height);
	//sets glwindows square to controllers square
	d->win->square = d->c->square;
	//set height and width
	d->meshHeight->value(height);
	d->c->height = height;
	d->meshWidth->value(width);
	d->c->width = width;

	//sets color values
	d->meshColors->rgb(red, green, blue);
	d->c->red = red;
	d->win->red = red;
	d->c->green = green;
	d->win->green = green;
	d->c->blue = blue;
	d->win->blue = blue;
	//sets number of rows and cols
	d->rows->value(rows);
	d->c->row = rows;
	d->win->rows = rows;
	d->columns->value(cols);
	d->win->cols = cols;
	d->c->col = cols;

	d->divideAmount->show();
	d->subdivide->show();
	d->fractal->show();

	//clears browser and then adds each vertice to browser
	d->browser->clear();
	for (int i = 0; i < d->win->rows * d->win->cols; i++)
	{
		d->browser->add("Vertice");
	}
	d->win->redraw();
}

void  Display::makeCB(Fl_Button* button, void* data)
{
	Display* d = (Display*)data;
	//sets square in c to new mesh of rows, columns, width, and height
	d->c->square = new mesh(d->c->row, d->c->col, d->c->width, d->c->height);
	//sets glwindows square to controllers square
	d->win->square = d->c->square;
	//sets color values
	d->win->red = d->c->red;
	d->win->green = d->c->green;
	d->win->blue = d->c->blue;
	//sets number of rows and cols
	d->win->rows = d->c->row;
	d->win->cols = d->c->col;

	d->divideAmount->show();
	d->subdivide->show();
	d->fractal->show();

	//clears browser and then adds each vertice to browser
	d->browser->clear();
	for (int i = 0; i < d->win->rows * d->win->cols; i++)
	{
		d->browser->add("Vertice");
	}
	d->win->redraw();
}

void Display::rowsCB(Fl_Value_Input* rows, void* data)
{
	//sets number of rows of mesh in c to value
	Controller* c = (Controller*)data;
	c->row = rows->value();
}

void Display::columnsCB(Fl_Value_Input* cols, void* data)
{
	//sets number of columns of mesh in c to value
	Controller* c = (Controller*)data;
	c->col = cols->value();
}

void Display::heightCB(Fl_Value_Input* height, void* data)
{
	//sets height of mesh in c to value
	Controller* c = (Controller*)data;
	c->height = height->value();
}

void Display::widthCB(Fl_Value_Input* width, void* data)
{
	//sets width of mesh in c to value
	Controller* c = (Controller*)data;
	c->width = width->value();
}

//top view
void Display::topCB(Fl_Button* width, void* data)
{
	Display* d = (Display*)data;
	//gets current theta and phi values
	float currentPhi = d->win->c->getPhi();
	float currentTheta = d->win->c->getTheta();
	if (currentPhi != 0)
	{
		//increase phi by -phi to result with 0
		d->win->c->increasePhi((currentPhi * (180 / 3.14159265)) * -1);
	}
	if (currentTheta != -90 * (3.14159265 / 180))
	{
		//increases theta by -90 - currenttheta to end up with top view
		float nextTheta = ((-90 * (3.14159265 / 180)) - currentTheta) * (180 / 3.14159265);
		d->win->c->increaseTheta(nextTheta);
	}
	d->win->redraw();
}

//side view
void Display::sideCB(Fl_Button* width, void* data)
{
	Display* d = (Display*)data;
	//gets current phi and theta
	float currentPhi = d->win->c->getPhi();
	float currentTheta = d->win->c->getTheta();
	if (currentPhi != 90 * (3.14159265 / 180))
	{
		//increases phi by 90 - currentphi to end up with side view
		float nextPhi = (90 * (3.14159265 / 180) - currentPhi) * (180 / 3.14159265);
		d->win->c->increasePhi(nextPhi);
	}
	if (currentTheta != 0)
	{
		//increases theta by -theta to result in 0
		d->win->c->increaseTheta((currentTheta * (180 / 3.14159265)) * -1);
	}
	d->win->redraw();
}

void Display::selectCB(Fl_Hold_Browser* w, void* data){
	Display* d = (Display*)data;
	d->c->select(w->value() - 1);
	d->win->selected = d->c->getSelected();
	
	//current row and column
	int currentRow = d->c->getSelected() / d->c->col;
	int currentCol = d->c->getSelected() % d->c->col;
	//sets values of sliders to values at selected array and shows sliders
	d->slideX->value((d->c->square->square[currentRow][currentCol])[0]);
	d->slideX->show();
	d->slideY->value((d->c->square->square[currentRow][currentCol])[1]);
	d->slideY->show();
	d->slideZ->value((d->c->square->square[currentRow][currentCol])[2]);
	d->slideZ->show();
	d->win->redraw();
}

void Display::slidexCB(Fl_Value_Slider* X, void* data)
{
	Display* d = (Display*)data;
	int currentRow = d->c->getSelected() / d->c->col;
	int currentCol = d->c->getSelected() % d->c->col;
	//sets value of x pos at current array position to value
	(d->c->square->square[currentRow][currentCol])[0] = X->value();
	d->win->square = d->c->square;
	d->win->redraw();
}

void Display::slideyCB(Fl_Value_Slider* Y, void* data)
{
	Display* d = (Display*)data;
	int currentRow = d->c->getSelected() / d->c->col;
	int currentCol = d->c->getSelected() % d->c->col;
	//sets value of Y pos at current array position to value
	(d->c->square->square[currentRow][currentCol])[1] = Y->value();
	d->win->square = d->c->square;
	d->win->redraw();
}

void Display::slidezCB(Fl_Value_Slider* Z, void* data)
{
	Display* d = (Display*)data;
	int currentRow = d->c->getSelected() / d->c->col;
	int currentCol = d->c->getSelected() % d->c->col;
	//sets value of z pos at current array position to value
	(d->c->square->square[currentRow][currentCol])[2] = Z->value();
	d->win->square = d->c->square;
	d->win->redraw();
}

void Display::wireCB(Fl_Round_Button* wire, void* data)
{
	Display* d = (Display*)data;
	//sets booleans for wire and solid drawing
	d->win->wire = true;
	d->win->solid = false;
	//clears solid and both buttons
	d->solid->clear();
	d->both->clear();
	d->win->redraw();
}

void Display::solidCB(Fl_Round_Button* solid, void* data)
{
	Display* d = (Display*)data;
	//sets booleans for wire and solid drawing
	d->win->wire = false;
	d->win->solid = true;
	//clears wire and both buttons
	d->wire->clear();
	d->both->clear();
	d->win->redraw();
}

void Display::bothCB(Fl_Round_Button* both, void* data)
{
	Display* d = (Display*)data;
	//sets booleans for wire and solid drawing
	d->win->wire = true;
	d->win->solid = true;
	//clears wire and solid buttons
	d->wire->clear();
	d->solid->clear();
	d->win->redraw();
}

void  Display::subdivideCB(Fl_Button* button, void* data)
{
	Display* d = (Display*)data;
	mesh* tempMesh;
	for (int i = 0; i < d->divideAmount->value(); i++)
	{
		tempMesh = new mesh((d->c->row * 2) - 1, (d->c->col * 2) - 1, d->c->width, d->c->height);
		for (int j = 0; j < d->c->row; j++)
		{
			for (int k = 0; k < d->c->col; k++)
			{
				//place each original vertex in new temp position
				tempMesh->square[j * 2][k * 2] = d->c->square->square[j][k];
				//point at center of face
				if (k != d->c->col - 1 && j != d->c->row - 1)
				{
					//center of face
					vec3 centerFace = vec3(0, 0, 0);
					centerFace = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + d->c->square->square[j + 1][k + 1] + d->c->square->square[j + 1][k]) / 4;
					tempMesh->square[j * 2 + 1][k * 2 + 1] = centerFace;
				}
				//get each edge midpoint 
				//prevent out of bounds exception if k is at final column
				if (k != d->c->col - 1)
				{
					//horizontal edge
					if (j == 0)
					{
						tempMesh->square[j * 2][k * 2 + 1] = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + tempMesh->square[j * 2 + 1][k * 2 + 1]) / 3;
					}
					else if (j != 0 && j != d->c->row - 1)
					{
						tempMesh->square[j * 2][k * 2 + 1] = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + tempMesh->square[j * 2 + 1][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2 + 1]) / 4;
					}
					else if (j == d->c->row - 1)
					{
						tempMesh->square[j * 2][k * 2 + 1] = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + tempMesh->square[j * 2 - 1][k * 2 + 1]) / 3;
					}
				}
				//prevent out of bounds exception if j is at final row
				if (j != d->c->row - 1)
				{
					//vertical edge
					if (k == 0)
					{
						tempMesh->square[j * 2 + 1][k * 2] = (d->c->square->square[j][k] + d->c->square->square[j + 1][k] + tempMesh->square[j*2 + 1][k*2+1]) / 3;
					}
					else if (k != 0 && k != d->c->col - 1)
					{
						tempMesh->square[j * 2 + 1][k * 2] = (d->c->square->square[j][k] + d->c->square->square[j + 1][k] + tempMesh->square[j * 2 + 1][k * 2 + 1] + tempMesh->square[j * 2 + 1][k * 2 - 1]) / 4;
					}
					else if (k == d->c->col - 1)
					{
						tempMesh->square[j * 2 + 1][k * 2] = (d->c->square->square[j][k] + d->c->square->square[j + 1][k] + tempMesh->square[j * 2 + 1][k * 2 - 1]) / 3;
					}
				}
				//replace original points
				//replaces original non edge points
				if (k != 0 && k != d->c->col - 1 && j != 0 && j != d->c->row - 1)
				{
					//average of faces for current point
					vec3 faceAv = (tempMesh->square[j * 2 - 1][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2 - 1] + tempMesh->square[j * 2 + 1][k * 2 - 1] + tempMesh->square[j * 2 + 1][k * 2 + 1]) / 4;
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2-1][k * 2] + tempMesh->square[j * 2][k * 2 - 1] + tempMesh->square[j * 2 + 1][k * 2]) / 4;
					tempMesh->square[j * 2][k * 2] = (faceAv/9) + (edgeAv/4.5) + (tempMesh->square[j * 2][k * 2]/ 1.5);
				}
				//top row minus corners
				else if (j == 0 && k != 0 && k != d->c->col - 1)
				{
					//average of faces for current point
					vec3 faceAv = (tempMesh->square[j * 2 + 1][k * 2 + 1] + tempMesh->square[j * 2 + 1][k * 2 - 1]) / 2;
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2 + 1][k * 2] + tempMesh->square[j * 2][k * 2 - 1]) / 3;
					tempMesh->square[j * 2][k * 2] = (faceAv/6) + (edgeAv/3) + (tempMesh->square[j * 2][k * 2]/2);
				}
				////bottom row minus corners
				else if (j == d->c->row - 1 && k != 0 && k != d->c->col - 1)
				{
					//average of faces for current point
					vec3 faceAv = (tempMesh->square[j * 2 - 1][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2 - 1]) / 2;
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2] + tempMesh->square[j * 2][k * 2 - 1]) / 3;
					tempMesh->square[j * 2][k * 2] = (faceAv/6) + (edgeAv/3) + (tempMesh->square[j * 2][k * 2]/2);
				}
				////leftmost column minus corners
				else if (k == 0 && j != 0 && j != d->c->row -1)
				{
					//average of faces for current point
					vec3 faceAv = (tempMesh->square[j * 2 - 1][k * 2 + 1] + tempMesh->square[j * 2 + 1][k * 2 + 1]) / 2;
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2] + tempMesh->square[j * 2 + 1][k * 2]) / 3;
					tempMesh->square[j * 2][k * 2] = (faceAv/6) + (edgeAv/3) + (tempMesh->square[j * 2][k * 2]/2);
				}
				//rightmost column minus corners
				else if (k == d->c->row-1 && j != 0 && j != d->c->row - 1)
				{
					//average of faces for current point
					vec3 faceAv = (tempMesh->square[j * 2 - 1][k * 2 - 1] + tempMesh->square[j * 2 + 1][k * 2 - 1]) / 2;
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2] + tempMesh->square[j * 2][k * 2 - 1]) / 3;
					tempMesh->square[j * 2][k * 2] = (faceAv/6) + (edgeAv/3) + (tempMesh->square[j * 2][k * 2]/2);
				}
				//top left corner
				else if (k == 0 && j == 0)
				{
					//average of faces for current point
					vec3 faceAv = tempMesh->square[j * 2 + 1][k * 2 + 1];
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2 + 1][k * 2]) / 2;
					tempMesh->square[j * 2][k * 2] = (faceAv / 4) + (edgeAv / 2) + (tempMesh->square[j * 2][k * 2] / 4);
				}
				//top right corner
				else if (j == 0 && k == d->c->col - 1)
				{
					//average of faces for current point
					vec3 faceAv = tempMesh->square[j * 2 + 1][k * 2 - 1];
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 - 1] + tempMesh->square[j * 2 + 1][k * 2]) / 2;
					tempMesh->square[j * 2][k * 2] = (faceAv / 4) + (edgeAv / 2) + (tempMesh->square[j * 2][k * 2] / 4);
				}
				//bottom left corner
				else if (j == d->c->row - 1 && k == 0)
				{
					//average of faces for current point
					vec3 faceAv = tempMesh->square[j * 2 - 1][k * 2 + 1];
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2]) / 2;
					tempMesh->square[j * 2][k * 2] = (faceAv / 4) + (edgeAv / 2) + (tempMesh->square[j * 2][k * 2] / 4);
				}
				//bottom right corner
				else if (j == d->c->row - 1 && k == d->c->col - 1)
				{
					//average of faces for current point
					vec3 faceAv = tempMesh->square[j * 2 - 1][k * 2 - 1];
					//average of edges for current point
					vec3 edgeAv = (tempMesh->square[j * 2][k * 2 - 1] + tempMesh->square[j * 2 - 1][k * 2]) / 2;
					tempMesh->square[j * 2][k * 2] = (faceAv / 4) + (edgeAv / 2) + (tempMesh->square[j * 2][k * 2] / 4);
				}
				//add in corner points cases
			}
		}
		delete(d->c->square);
		d->c->row = (d->c->row * 2) - 1;
		d->c->col = (d->c->col * 2) - 1;
		d->win->rows = d->c->row;
		d->win->cols = d->c->col;
		d->c->square = tempMesh;
	}

	d->browser->clear();
	for (int i = 0; i < d->win->rows * d->win->cols; i++)
	{
		d->browser->add("Vertice");
	}

	d->win->square = d->c->square;
	d->win->redraw();
}

void  Display::fractalCB(Fl_Button* button, void* data)
{
	Display* d = (Display*)data;
 	mesh* tempMesh;
	for (int i = 0; i < d->divideAmount->value(); i++)
	{
		tempMesh = new mesh((d->c->row * 2) - 1, (d->c->col * 2) - 1, d->c->width, d->c->height);
		for (int j = 0; j < d->c->row; j++)
		{
			for (int k = 0; k < d->c->col; k++)
			{
				//place each original vertex in new temp position
				tempMesh->square[j * 2][k * 2] = d->c->square->square[j][k];
				if (k != d->c->col - 1 && j != d->c->row - 1)
				{
					//center of face
					vec3 centerFace = vec3(0, 0, 0);
					centerFace[0] = (((d->c->square->square[j][k])[0] + (d->c->square->square[j][k + 1])[0] + (d->c->square->square[j + 1][k + 1])[0] + (d->c->square->square[j + 1][k])[0])) / 4;
					centerFace[1] = (((d->c->square->square[j][k])[1] + (d->c->square->square[j][k + 1])[1] + (d->c->square->square[j + 1][k + 1])[1] + (d->c->square->square[j + 1][k])[1])) / 4;
					centerFace[2] = (((d->c->square->square[j][k])[2] + (d->c->square->square[j][k + 1])[2] + (d->c->square->square[j + 1][k + 1])[2] + (d->c->square->square[j + 1][k])[2])) / 4;
					tempMesh->square[j * 2 + 1][k * 2 + 1] = centerFace;
					//get the vector from current vertice to vertice diagonal of it
					vec3 sub = d->c->square->square[j][k] - d->c->square->square[j + 1][k + 1];
					//get length of diagonal vector
					float len = sub.length();
					//find random float from -len/2 and len/2 and add to y value
					double random = ((double)rand() / (double)(RAND_MAX / len)) - len/2;
					tempMesh->square[j * 2 + 1][k * 2 + 1][1] += random;
				}
				//get each edge midpoint 
				//prevent out of bounds exception if k is at final column
				if (k != d->c->col - 1)
				{
					//horizontal edge
					if (j == 0)
					{
						tempMesh->square[j * 2][k * 2 + 1] = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + tempMesh->square[j * 2 + 1][k * 2 + 1]) / 3;
					}
					else if (j != 0 && j != d->c->row - 1)
					{
						tempMesh->square[j * 2][k * 2 + 1] = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + tempMesh->square[j * 2 + 1][k * 2 + 1] + tempMesh->square[j * 2 - 1][k * 2 + 1]) / 4;
					}
					else if (j == d->c->row - 1)
					{
						tempMesh->square[j * 2][k * 2 + 1] = (d->c->square->square[j][k] + d->c->square->square[j][k + 1] + tempMesh->square[j * 2 - 1][k * 2 + 1]) / 3;
					}
					vec3 sub = d->c->square->square[j][k] - d->c->square->square[j][k + 1];
					float len = sub.length();
					double random = ((double)rand() / (double)(RAND_MAX / len)) - len / 2;
					tempMesh->square[j * 2][k * 2 + 1][1] += random;
				}
				//prevent out of bounds exception if j is at final row
				if (j != d->c->row - 1)
				{
					if (k == 0)
					{
						tempMesh->square[j * 2 + 1][k * 2] = (d->c->square->square[j][k] + d->c->square->square[j + 1][k] + tempMesh->square[j * 2 + 1][k * 2 + 1]) / 3;
					}
					else if (k != 0 && k != d->c->col - 1)
					{
						tempMesh->square[j * 2 + 1][k * 2] = (d->c->square->square[j][k] + d->c->square->square[j + 1][k] + tempMesh->square[j * 2 + 1][k * 2 + 1] + tempMesh->square[j * 2 + 1][k * 2 - 1]) / 4;
					}
					else if (k == d->c->col - 1)
					{
						tempMesh->square[j * 2 + 1][k * 2] = (d->c->square->square[j][k] + d->c->square->square[j + 1][k] + tempMesh->square[j * 2 + 1][k * 2 - 1]) / 3;
					}
					vec3 sub = d->c->square->square[j][k] - d->c->square->square[j + 1][k];
					float len = sub.length();
					double random = ((double)rand() / (double)(RAND_MAX / len)) - len / 2;
					tempMesh->square[j * 2][k * 2 + 1][1] += random;
				}
			}
		}
		delete(d->c->square);
		d->c->row = (d->c->row * 2) - 1;
		d->c->col = (d->c->col * 2) - 1;
		d->win->rows = d->c->row;
		d->win->cols = d->c->col;
		d->c->square = tempMesh;
	}

	d->browser->clear();
	for (int i = 0; i < d->win->rows * d->win->cols; i++)
	{
		d->browser->add("Vertice");
	}

	d->win->square = d->c->square;
	d->win->redraw();
}