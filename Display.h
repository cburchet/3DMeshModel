#ifndef DISPLAY_H
#define DISPLAY_H
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Hold_Browser.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Round_Button.H>
#include "GLWindow.h"

class Controller;	// forward declaration

class Display: public Fl_Window{
private:
	static void colorsCB(Fl_Color_Chooser* colors, void* data);
	static void makeCB(Fl_Button* button, void* data);
	static void randomCB(Fl_Button* button, void* data);
	static void rowsCB(Fl_Value_Input* rows, void* data);
	static void columnsCB(Fl_Value_Input* cols, void* data);
	static void heightCB(Fl_Value_Input* height, void* data);
	static void widthCB(Fl_Value_Input* width, void* data);
	static void topCB(Fl_Button* width, void* data);
	static void sideCB(Fl_Button* width, void* data);
	static void selectCB(Fl_Hold_Browser* w, void* data);
	static void wireCB(Fl_Round_Button* wire, void* data);
	static void solidCB(Fl_Round_Button* solid, void* data);
	static void bothCB(Fl_Round_Button* both, void* data);
	static void slidexCB(Fl_Value_Slider* X, void* data);
	static void slideyCB(Fl_Value_Slider* Y, void* data);
	static void slidezCB(Fl_Value_Slider* Z, void* data);
	//static void divideCB(Fl_Value_Slider* div, void* data);
	static void subdivideCB(Fl_Button* sub, void* data);
	static void fractalCB(Fl_Button* frac, void* data);

	GLWindow* win;				// the OpenGL window
	Fl_Button* makeMesh;
	Fl_Button* randomMesh;
	Fl_Button* sideView;
	Fl_Button* topView;
	Fl_Round_Button* wire;
	Fl_Round_Button* solid;
	Fl_Round_Button* both;
	Fl_Hold_Browser* browser;
	Fl_Value_Input* rows;
	Fl_Value_Input* columns;
	Fl_Value_Input* meshWidth;
	Fl_Value_Input* meshHeight;
	Fl_Color_Chooser* meshColors;
	Fl_Value_Slider* slideX;
	Fl_Value_Slider* slideY;
	Fl_Value_Slider* slideZ;
	Fl_Value_Slider* divideAmount;
	Fl_Button* subdivide;
	Fl_Button* fractal;
public:
	Display(int x, int y, int w, int h, Controller* c);				// constructor
	Controller* c;				// pointer back to the controller
	void updateDrawing(){ win->redraw(); }
};


#endif
