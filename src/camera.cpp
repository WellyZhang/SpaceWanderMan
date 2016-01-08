#ifdef _WIN32
#include <Windows.h>
#endif
#include <glut.h>
#include <cmath>
#include <cstdio>
#include "camera.h"

// set vec to (x,y,z)
void vectorSet(float* vec, float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

// add v2 to v1
void vectorAdd(float* v1, float* v2)
{
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

// copies v2 into v1
void vectorCopy(float* v1, float* v2)
{
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

// multiply a vec by a scalar
void vectorMul(float* vec, float scalar)
{
	vec[0] *= scalar;
	vec[1] *= scalar;
	vec[2] *= scalar;
}

// find the magnitude of a vec
float lengthOfVec(float* vec)
{
	return sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// normalize a vector to be of magnitude 1
void normalizeVec(float* vec)
{
	vectorMul(vec, 1 / lengthOfVec(vec));
}

// makes a 3x3 rotation matrix from the given angle and axis
void rotationMatrix(float* matrix, float* axis, float angle)
{
	float cos1 = cos(angle);
	float cos2 = 1 - cos1;
	float sin1 = sin(angle);

	matrix[0] = cos1 + axis[0]*axis[0]*cos2;
	matrix[1] = axis[0] * axis[1] * cos2 - axis[2]*sin1;
	matrix[2] = axis[0] * axis[2] * cos2 + axis[1]*sin1;

	matrix[3] = axis[1] * axis[0] * cos2 + axis[2]*sin1;
	matrix[4] = cos1 + axis[1] * axis[1] * cos2;
	matrix[5] = axis[1] * axis[2] * cos2 - axis[0] * sin1;
	
	matrix[6] = axis[2] * axis[0] * cos2 - axis[1] * sin1;
	matrix[7] = axis[2] * axis[1] * cos2 + axis[0] * sin1;
	matrix[8] = cos1 + axis[2] * axis[2] * cos2;
}

// multiply a vector v1 by a matrix and put the results into vector v2
void mulVecBy(float* v1, float* matrix, float* v2)
{
	v2[0] = v1[0] * matrix[0] + v1[1] * matrix[1] + v1[2] * matrix[2];
	v2[1] = v1[0] * matrix[3] + v1[1] * matrix[4] + v1[2] * matrix[5];
	v2[2] = v1[0] * matrix[6] + v1[1] * matrix[7] + v1[2] * matrix[8];
}

// rotate a vector v1 around the axis v2 by angle and put the result into v3
void rotateAroundVec(float* v1, float* v2, float angle, float* v3)
{
	float matrix[16];
	rotationMatrix(matrix, v2, angle);
	mulVecBy(v1, matrix, v3);
}

Camera::Camera(void)
{
	// initial values are set by testing
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;
	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770);
	vectorSet(forwardVec,-0.398769796f, 0.763009906f, -0.508720219f);
	vectorSet(rightVec, 0.886262059f, 0.463184059f, 0.000000000f);
	vectorSet(upVec, -0.235630989f, 0.450859368f, 0.860931039f);
}

void Camera::reset(void){
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;
	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770);
	vectorSet(forwardVec, -0.398769796f, 0.763009906f, -0.508720219f);
	vectorSet(rightVec, 0.886262059f, 0.463184059f, 0.000000000f);
	vectorSet(upVec, -0.235630989f, 0.450859368f, 0.860931039f);
}

void Camera::transformOrientation(void)
{
	float tempForward[3], tempUp[3], tempRight[3];
	transformWithMouse(mouseLeftRight, mouseUpDown, tempForward, tempUp, tempRight);

	// look at the direction of the orientation vectors
	gluLookAt(0, 0, 0, tempForward[0], tempForward[1], tempForward[2], tempUp[0], tempUp[1], tempUp[2]);
}

void Camera::transformTranslation(void)
{
	// translate to emulate camera position
	glTranslatef(-position[0], -position[1], -position[2]);
}

// points the camera at the given point in 3d space
void Camera::pointAt(float* targetVec)
{
	float tempVec[3];
	float up[3] = { 0.0f, 0.0f, 1.0f };
	forwardVec[0] = targetVec[0] - position[0];
	forwardVec[1] = targetVec[1] - position[1];
	forwardVec[2] = targetVec[2] - position[2];
	normalizeVec(forwardVec);
	rotateAroundVec(forwardVec, up, -1.57079632679f, tempVec);
	tempVec[2] = 0;
	normalizeVec(tempVec);
	vectorCopy(rightVec, tempVec);
	rotateAroundVec(forwardVec, rightVec, 1.57079632679f, tempVec);
	vectorCopy(upVec, tempVec);
}

void Camera::speedUp(void)
{
	if (cameraSpeed < 1.0f)
		cameraSpeed *= 2;
}

void Camera::slowDown(void)
{
	if (cameraSpeed > 0.000001f)
		cameraSpeed /= 2;
}

void Camera::forward(void)
{
	float tempForward[3], tempUp[3], tempRight[3];
	transformWithMouse(mouseLeftRight, mouseUpDown, tempForward, tempUp, tempRight);
	float vec[3];
	vectorCopy(vec, tempForward);
	vectorMul(vec, cameraSpeed);
	vectorAdd(position, vec);
}

void Camera::backward(void)
{
	float tempForward[3], tempUp[3], tempRight[3];
	transformWithMouse(mouseLeftRight, mouseUpDown, tempForward, tempUp, tempRight);
	float vec[3];
	vectorCopy(vec, tempForward);
	vectorMul(vec, -cameraSpeed);
	vectorAdd(position, vec);
}

void Camera::left(void)
{
	float tempForward[3], tempUp[3], tempRight[3];
	transformWithMouse(mouseLeftRight, mouseUpDown, tempForward, tempUp, tempRight);
	float vec[3];
	vectorCopy(vec, tempRight);
	vectorMul(vec, -cameraSpeed);
	vectorAdd(position, vec);
}

void Camera::right(void)
{
	float tempForward[3], tempUp[3], tempRight[3];
	transformWithMouse(mouseLeftRight, mouseUpDown, tempForward, tempUp, tempRight);
	float vec[3];
	vectorCopy(vec, tempRight);
	vectorMul(vec, cameraSpeed);
	vectorAdd(position, vec);
}

void Camera::yawLeft(void)
{
	float tempVec[3];
	rotateAroundVec(forwardVec, upVec, cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);
	rotateAroundVec(rightVec, upVec, cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

void Camera::yawRight(void)
{
	float tempVec[3];
	rotateAroundVec(forwardVec, upVec, -cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);
	rotateAroundVec(rightVec, upVec, -cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

void Camera::setMouse(float x, float y)
{
	float z;
	x = -(x - 600);
	y = -(y - 350);
	float r = sqrt(pow(x, 2) + pow(y, 2));
	if (r > 350) 
	{
		x = x * 350 / r;
		y = y * 350 / r;
		z = 0;
	}
	else
	{
		z = sqrt(350 * 350 - pow(x, 2) - pow(y, 2));
	}
	mouseLeftRight = atan(x / z);
	mouseUpDown = asin(y / 350);
}

void Camera::transformWithMouse(float theta, float phi, float *forward, float *up, float *right)
{
	float tempForward[3], tempUp[3], tempRight[3], tempVec[3];
	rotateAroundVec(forwardVec, upVec, theta, tempVec);
	rotateAroundVec(rightVec, upVec, theta, tempRight);
	rotateAroundVec(tempVec, tempRight, phi, tempForward);
	rotateAroundVec(upVec, tempRight, phi, tempUp);
	vectorCopy(forward, tempForward);
	vectorCopy(right, tempRight);
	vectorCopy(up, tempUp);
}

void Camera::saveImage(void)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int width = viewport[2];
	int height = viewport[3];

	GLubyte* temp = new GLubyte[width * height * 4];
	memset(temp, 0, width * height * 4);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, temp);

	GLubyte* pdata = new GLubyte[width * height * 3];
	memset(pdata, 0, width * height * 3);

	for (int i = 0; i < width * height * 3; i++){
		pdata[i] = temp[i];
	}

	BITMAPFILEHEADER Header;
	memset(&Header, 0, sizeof(BITMAPFILEHEADER));
	Header.bfType = 0x4D42;
	Header.bfReserved1 = 0;
	Header.bfReserved2 = 0;
	Header.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	Header.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 3 * width * height);

	BITMAPINFOHEADER HeaderInfo;
	memset(&HeaderInfo, 0, sizeof(BITMAPINFOHEADER));
	HeaderInfo.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	HeaderInfo.biWidth = width;
	HeaderInfo.biHeight = height;
	HeaderInfo.biPlanes = 1;
	HeaderInfo.biBitCount = 24;
	HeaderInfo.biCompression = 0;
	HeaderInfo.biSizeImage = 3 * width * height;
	HeaderInfo.biXPelsPerMeter = 0;
	HeaderInfo.biYPelsPerMeter = 0;
	HeaderInfo.biClrUsed = 0;
	HeaderInfo.biClrImportant = 0;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char filename[64];
	sprintf(filename, "Snapshot_%4d%02d%02d_%02d%02d%02d.bmp", sys.wYear, sys.wMonth, 
		sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	FILE *pfile = fopen(filename, "wb+");
	fwrite(&Header, 1, sizeof(BITMAPFILEHEADER), pfile);
	fwrite(&HeaderInfo, 1, sizeof(BITMAPINFOHEADER), pfile);
	for (int i = 0; i < width * height * 3; i++)
	{
		fwrite(&pdata[i], 1, sizeof(char), pfile);
	}
	fclose(pfile);
	delete[] pdata;
}