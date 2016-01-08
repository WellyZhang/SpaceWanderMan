#ifndef SWM_WORMHOLE_H
#define SWM_WORMHOLE_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <vector>
#include "moon.h"
#include "camera.h"

/*
 * This class makes wormholes for a solar system.
 * It allows the spaceship to enter a wormhole and appears in another solar system.
 * Note that most of the names of the members are self-explanatory.
 */

class Wormhole
{
private:
	float distanceFromSun;
	float orbitTime;
	float rotationTime;
	float radius;
	GLuint textureHandle;
	float position[3];
	float rotation;
public:
	Wormhole(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void calculatePosition(float time);
	void render(void);
	void renderOrbit(void);
	void getPosition(float* vec);
	float getRadius(void);
};

#endif
