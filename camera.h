#include <math.h>
#include "algebra3.h"
#include <iostream>

// A class for storing camera transforms
class Camera{
	private:
		mat4 rot;			// Current rotation matrix 
		vec3 up;				// Current up vector
		vec3 eye;			// Current eye/camera position
		//easy r, phi, theta values to use for moving camera
		float r;
		float phi;
		float theta;
	public:
		Camera(float r, float phi, float theta); 
		void increasePhi(float inc);		// increase the phi angle. Note a negative value will decrease it. 
		void increaseTheta(float inc);		// increase the theta angle
		void increaseRad(float inc);		// increase the radius
		const vec3 getEye();		// get the current eye position
		void setEye(float x, float y, float z);
		const vec3 getUp();		// get the current up vector
		const float getR();
		const float getPhi();
		const float getTheta();
		void setPhi(float p);
		void setTheta(float t);
};