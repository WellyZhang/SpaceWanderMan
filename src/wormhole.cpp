#include "wormhole.h"
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <glut.h>
#include "globals.h"

// planet size scaling factor
extern float planetSizeScale;

Wormhole::Wormhole(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
{
	this->distanceFromSun = distanceFromSun;
	this->orbitTime = orbitTime;
	this->rotationTime = rotationTime;
	this->radius = radius;
	this->textureHandle = textureHandle;
}

void Wormhole::calculatePosition(float time)
{
	// find the angle of orientation of the orbit around the sun
	float angle = time * 3.1419f / orbitTime;
	
	// trigonometric function for position calculation
	position[0] = sin(angle) * distanceFromSun;
	position[1] = cos(angle) * distanceFromSun;
	position[2] = 0;

	// find the rotation of the planet around its axis
	rotation = time * 360 / rotationTime;
}

void Wormhole::render(void)
{
	glPushMatrix();

	// translate to the right positon
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);

	/// rotate for the planet's spin
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	// GLU sphere quadric object for the wormhole
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	// if this is the sun, don't render it too big, and disable lighting
	if (distanceFromSun < 0.001f)
	{
		float radiusScaled = radius * planetSizeScale;
		if (radiusScaled > 0.5f) radiusScaled = 0.5f;

		glDisable(GL_LIGHTING);
		gluSphere(quadric, radiusScaled, 30, 30);
		glEnable(GL_LIGHTING);
	}
	else
	{
		gluSphere(quadric, radius * planetSizeScale, 30, 30);
	}

	glPopMatrix();
}

void Wormhole::renderOrbit(void)
{
}

void Wormhole::getPosition(float* vec)
{
	vec[0] = position[0] * distanceScale;
	vec[1] = position[1] * distanceScale;
	vec[2] = position[2] * distanceScale;
}

float Wormhole::getRadius(void)
{
	return radius;
}
