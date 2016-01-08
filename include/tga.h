#ifndef SWM_TGA_H
#define SWM_TGA_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>

/*
 * This class loads a TGA image from the disk for texture mapping.
 * Note that most of the names of the members are self-explanatory.
 */

class TGA
{
private:
	GLuint textureHandle;
public:
	TGA(char* imagePath);
	GLuint getTextureHandle(void);
};

#endif