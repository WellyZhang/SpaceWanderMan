#ifndef SWM_PLANET_H
#define SWM_PLANET_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <vector>
#include "moon.h"

/*
 * This class makes planets in a solar system.
 * Note that most of the names of the members are self-explanatory.
 */

class Planet
{
private:
	float distanceFromSun;
	float orbitTime;
	float rotationTime;
	float radius;
	GLuint textureHandle;
	float position[3];
	float rotation;

	// moons attached to this planet
	std::vector<Moon> moons;
public:
	Planet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void calculatePosition(float time);
	void render(void);
	void renderOrbit(void);
	void getPosition(float* vec);
	float getRadius(void);
	void addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
};

#endif