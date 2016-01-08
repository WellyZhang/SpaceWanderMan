#ifndef SWM_SOLARSYSTEM_H
#define SWM_SOLARSYSTEM_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <vector>

#include "planet.h"
#include "camera.h"
#include "wormhole.h"

/*
 * This class makes a solar system for the main program.
 * Note that most of the names of the members are self-explanatory.
 */

class SolarSystem
{
private:
	std::vector<Planet> planets;
	std::vector<Wormhole> wormholes;

public:
	SolarSystem();
	SolarSystem(int mode);
	void calculatePositions(float time);
	void addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void addWormhole(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void render();
	void renderOrbits();
	void getPlanetPosition(int index, float* vec);
	float getRadiusOfPlanet(int index);

	// check the minimum distance with all planets
	float testDistancewithPlanet(Camera camera);

	// check the minimum distance with all wormholes
	float testDistancewithWormhole(Camera camera);
	bool hasPlanet(unsigned char index);
};

#endif