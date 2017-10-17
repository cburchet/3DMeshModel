#include "camera.h"

Camera::Camera(float r, float phi, float theta)
{
			// converts phi and theta to radians
	phi = phi * (3.14159265 / 180);
	theta = theta * (3.14159265 / 180);
	this->r = r;
	this->phi = phi;
	this->theta = theta;
	//computes the x, y, z position of the camera
	eye = vec3(r * cos(theta) * cos(phi), r * sin(theta) * cos(phi), r * sin(phi));
	//computes the up vector for the camera
	up = vec3(0, 1, 0);
	rot = rotation3D(vec3(0, 1, 0), 0);
}

void Camera::increasePhi(float inc)
{
	//changes inc to radians and adds to phi
	inc = inc * (3.14159265 / 180);
	phi += inc;
	//computes position of camera and up vector
	eye = vec3(r * cos(theta) * cos(phi), r * sin(theta) * cos(phi), r * sin(phi));
	mat4 rotMatrix = rotation3D(up, inc);
	this->rot = rotMatrix * this->rot;
}

void Camera::increaseTheta(float inc)
{
	//changes inc to radians and adds to theta
	inc = inc * (3.14159265 / 180);
	theta += inc;
	float ypre = eye[1];
	//computes position of camera and up vector
	eye = vec3(r * cos(theta) * cos(phi), r * sin(theta) * cos(phi), r * sin(phi));
	mat4 rotMatrix = rotation3D(eye ^ up, inc);
	this->rot = rotMatrix * this->rot;
	if (ypre - eye[1] > 0)
	{
		up[1] = -1;
	}
	else
	{
		up[1] = 1;
	}
} 

void Camera::increaseRad(float inc)
{
	//adds inc to r and computes the new position
	r += inc;
	eye = vec3(r * cos(theta) * cos(phi), r * sin(theta) * cos(phi), r * sin(phi));
}

const vec3 Camera::getEye()
{
	return eye;
}

void Camera::setEye(float x, float y, float z)
{
	eye = vec3(x, y, z);
}

const vec3 Camera::getUp()
{
	return up;
}

//returns r, phi, theta if ever needed
const float Camera::getR()
{
	return r;
}


const float Camera::getPhi()
{
	return phi;
}

const float Camera::getTheta()
{
	return theta;
}

void Camera::setPhi(float p)
{
	phi = p;
}

void Camera::setTheta(float t)
{
	theta = t;
}
