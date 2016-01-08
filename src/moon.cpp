#include "moon.h"
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <glut.h>
#include "globals.h"

Moon::Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	this->distanceFromPlanet = distanceFromPlanet;
	this->orbitTime = orbitTime;
	this->rotationTime = rotationTime;
	this->radius = radius;
	this->textureHandle = textureHandle;
}

void Moon::calculatePosition(float time)
{
	// find the angle of orientation of the orbit around the planet
	float angle = time * 3.1419f / orbitTime;
	
	// trigonometric function for position calculation
	position[0] = sin(angle) * distanceFromPlanet;
	position[1] = cos(angle) * distanceFromPlanet;
	position[2] = 0;

	// find the rotation of the moon around its axis
	rotation = time * 360 / rotationTime;
}

void Moon::render(void)
{
	glPushMatrix();

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// translate to the right positon and rotate for the moons spinning
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);
	glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
	
	// GLU sphere quadric object for the moon
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, radius * planetSizeScale, 30, 30);
	glPopMatrix();
}

void Moon::renderOrbit(void)
{
	glBegin(GL_LINE_STRIP);
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.1f)
	{
		glVertex3f(sin(angle) * distanceFromPlanet * distanceScale, cos(angle) * distanceFromPlanet * distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromPlanet * distanceScale, 0.0f);

	glEnd();
}

void Moon::getPosition(float* vec)
{
	vec[0] = position[0] * distanceScale;
	vec[1] = position[1] * distanceScale;
	vec[2] = position[2] * distanceScale;
}

float Moon::getRadius(void)
{
	return radius;
}