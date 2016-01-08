#ifndef SWM_MOON_H
#define SWM_MOON_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>

/*
 * This class makes moons for a planet.
 * Note that most of the names of the members are self-explanatory.
 */

class Moon
{
private:
	float distanceFromPlanet;
	float orbitTime;
	float rotationTime;
	float radius;
	GLuint textureHandle;
	float position[3];
	float rotation;
public:
	Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void calculatePosition(float time);
	void render(void);
	void renderOrbit(void);
	void getPosition(float* vec);
	float getRadius(void);
};

#endif