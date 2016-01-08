#include "solarsystem.h"
#include "tga.h"

extern double time;

extern float planetSizeScale;

extern TGA* sun, *mercury, *venus, *earth, *mars, *jupiter, *saturn, *uranus, *neptune, *pluto, *wormhole_pic, *moon, *other_planets[11], *sunPic[3];

SolarSystem::SolarSystem()
{
	this->planets.clear();
	this->wormholes.clear();

	// add all the planets with accurate data. Distance measured in km, time measured in earth days
	
	// Sum
	this->addPlanet(0, 1, 500, 695500, sun->getTextureHandle());

	// Mercury
	this->addPlanet(57910000, 88, 58.6, 2440, mercury->getTextureHandle());

	// Venus
	this->addPlanet(108200000, 224.65, 243, 6052, venus->getTextureHandle());

	// Earth
	this->addPlanet(149600000, 365, 1, 6371, earth->getTextureHandle());

	// Mars
	this->addPlanet(227939100, 686, 1.03f, 3389, mars->getTextureHandle());

	// Jupiter
	this->addPlanet(778500000, 4332, 0.4139, 69911, jupiter->getTextureHandle());

	// Saturn
	this->addPlanet(1433000000, 10759, 0.44375, 58232, saturn->getTextureHandle());

	// Uranus
	this->addPlanet(2877000000, 30685, 0.718056, 25362, uranus->getTextureHandle());

	// Neptune
	this->addPlanet(4503000000, 60188, 0.6713, 24622, neptune->getTextureHandle());

	// Pluto
	this->addPlanet(5906380000, 90616, 6.39, 1137, pluto->getTextureHandle());

	// Moon for the Earth
	this->addMoon(3, 7000000, 27.3, 27.3, 1738, moon->getTextureHandle());

	// Wormhole
	this->addWormhole(130000000, 13000000000.0, 0.0130, 13000, wormhole_pic->getTextureHandle());
}

// a new constructor that generates a random solar system based on time
SolarSystem::SolarSystem(int mode)
{
	this->planets.clear();
	this->wormholes.clear();

	if (mode != 0)
	{
		return;
	}
	bool flag[11];
	for (int i = 0; i < 11; i++)
	{
		flag[i] = false;
	}
	flag[0] = true;

	// set the random number generator
	srand((int)time);
	int sun_index = rand() % 3;

	// set up a new solar system based on random numbers
	this->addPlanet(0, 1, 500, 695500, sunPic[sun_index]->getTextureHandle());
	int solar_size = rand() % 5 + 5;
	float distancefromSun = 0;

	// loop over all planets
	while (solar_size--)
	{
		int index = rand() % 12;
		while (flag[index])
		{
			index = rand() % 12;
		}
		
		// if this is exactly the Sun
		if (distancefromSun < 0.001f)
		{
			float distance = (rand() + 1000) * (rand() + 1000);
			while (distance < 57910000 || distance > 120000000)
			{
				distance = (rand() + 1000) * (rand() + 1000);
			}
			distancefromSun = distancefromSun + distance;
			this->addPlanet(distancefromSun, rand() % 1000 + 100, (float)(rand() + 1000) / 8000, 
				rand() % 1500 + 3000, other_planets[index]->getTextureHandle());
		}
		
		// this is a normal planet
		else{
			float distance = (rand() + 1000) * (rand() + 1000);
			while (distance < 57910000 || distance > 120000000)
			{
				distance = (rand() + 1000) * (rand() + 1000);
			}
			distancefromSun = distancefromSun + distance;
			this->addPlanet(distancefromSun, rand() % 1000 + 100, (float)(rand() + 1000) / 8000, 
				rand() % 20000 + 5000, other_planets[index]->getTextureHandle());
		}
		flag[index] = true;
	}

	// add a new wormhole
	this->addWormhole(130000000, 13000000000.0, 0.0130, 13000, wormhole_pic->getTextureHandle());
}

void SolarSystem::calculatePositions(float time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].calculatePosition(time);
	}
	for (int i = 0; i < wormholes.size(); i++)
	{
		wormholes[i].calculatePosition(time);
	}
}

void SolarSystem::addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets.push_back(Planet(distanceFromSun, orbitTime, rotationTime, radius, textureHandle));
}

void SolarSystem::addWormhole(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	wormholes.push_back(Wormhole(distanceFromSun, orbitTime, rotationTime, radius, textureHandle));
}

void SolarSystem::addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	planets[planetIndex].addMoon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle);
}

void SolarSystem::render()
{
	for (int i = 0; i < wormholes.size(); i++)
	{
		wormholes[i].render();
	}
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].render();
	}
}

void SolarSystem::renderOrbits()
{
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].renderOrbit();
	}
	glEnable(GL_TEXTURE_2D);
}

void SolarSystem::getPlanetPosition(int index, float* vec)
{
	planets[index].getPosition(vec);
}

float SolarSystem::getRadiusOfPlanet(int index)
{
	return planets[index].getRadius();
}

// check the minimum distance with all planets
float SolarSystem::testDistancewithPlanet(Camera camera)
{
	float position[3];
	for (int i = 0; i < 3; i++)
	{
		position[i] = camera.position[i];
	}
	float min_distance = 10000.0f;

	// loop over all planets
	for (int i = 0; i < planets.size(); i++)
	{
		float pos[3] = {1, 1, 1};
		planets[i].getPosition(pos);
		float distance = 0;

		// calculate the distance from each planet's center
		for (int j = 0; j < 3; j++)
		{
			distance = distance + (pos[j] - position[j]) * (pos[j] - position[j]);
		}
		distance = sqrt(distance);
		float radiusScale = planets[i].getRadius() * planetSizeScale;
		if (radiusScale > 0.5f)
		{
			radiusScale = 0.5f;
		}

		// find the global minimum
		if (min_distance > distance - radiusScale)
		{
			min_distance = distance - radiusScale;
		}
	}
	return min_distance;
}

// check the minimum distance with all wormholes
float SolarSystem::testDistancewithWormhole(Camera camera)
{
	float position[3];
	for (int i = 0; i < 3; i++)
	{
		position[i] = camera.position[i];
	}
	float min_distance = 10000.0f;

	// loop over all wormholes
	for (int i = 0; i < wormholes.size(); i++)
	{
		float pos[3] = {1, 1, 1};
		wormholes[i].getPosition(pos);
		float distance = 0;

		// calculate the distance from each wormhole's center
		for (int j = 0; j < 3; j++)
		{
			distance = distance + (pos[j] - position[j]) * (pos[j] - position[j]);
		}
		distance = sqrt(distance);
		float radiusScale = wormholes[i].getRadius() * planetSizeScale;

		// find the global minimum 
		if (min_distance > distance - radiusScale)
		{
			min_distance = distance - radiusScale;
		}
	}
	return min_distance;
}

bool SolarSystem::hasPlanet(unsigned char index)
{
	int x = index - '0';
	if (x < planets.size())
		return true;
	return false;
}