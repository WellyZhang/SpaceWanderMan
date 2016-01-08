#ifndef SWM_CAMERA_H
#define SWM_CAMERA_H

/*
 * This class implements functions needed for camera manipulation.
 * It could be used to modify views from the spaceship. Though in fact the view
 * of the user is changed, it still simulates the motion of the spaceship.
 * Note that most of the names of the members are self-explanatory.
 */

class Camera
{
friend class SolarSystem;
private:
	// a vector pointing in the direction you're facing
	float forwardVec[3];

	// a vector pointing to the right of the front 
	float rightVec[3];

	// a vector pointing upwards from the front
	float upVec[3];

	// a vector describing the position of the camera
	float position[3];

	// the camera speed
	float cameraSpeed;
	float cameraTurnSpeed;

	// mouse position
	float mouseUpDown;
	float mouseLeftRight;

public:
	Camera(void);
	void reset(void);

	// transform the OpenGL view matrix for the orientation
	void transformOrientation(void);

	// transform the OpenGL view matrix for the translation
	void transformTranslation(void);
	void pointAt(float* targetVec);
	void speedUp(void);
	void slowDown(void);

	// move the camera forward
	void forward(void);

	// move the camera backward
	void backward(void);

	// steer left
	void left(void);

	// steer right
	void right(void);

	// yaw left
	void yawLeft(void);

	// yaw right
	void yawRight(void);

	// set mouse control
	void setMouse(float x, float y);

	// transform the OpenGL view matrix for the mouse contorl
	void transformWithMouse(float theta, float phi, float *forward, float *up, float *right);

	// save the snapshot
	void saveImage(void);
};

#endif