#include <iostream>
#include <FL/Fl.H>
#include "eController.h"


// extrusion main entry point
int main (int argc, char * const argv[]) {
	Controller* c = new Controller();		// All functionality is linked through a single controller class
	Fl::run();
	
    return 0;
}
