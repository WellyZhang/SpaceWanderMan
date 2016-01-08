#include <cmath>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <glut.h>

#include <fstream>
#include "tga.h"
#include "solarsystem.h"
#include "camera.h"
#include "globals.h"
#include "wormhole.h"

// screen size
int screenWidth, screenHeight;

// The TGA texture containing the help dialogue, the starfield, planet texture and spaceship texture.
TGA *window, *stars;
TGA *sun, *mercury, *venus, *earth, *mars, *jupiter, *saturn, *uranus, *neptune, *pluto, *wormhole_pic;
TGA *other_planets[11], *sunPic[3];
TGA *moon, *topSafe, *topFrame, *topDanger, *crashed, *vertical, *horizontal, 
	*black, *control, *mirror, *mirrorMid;

// starship view
bool starshipView = true;

// orbits are drawn
bool showOrbits = true;

// set the initial focus of the spaceship to be the Sun
int planetSelected = 1;

// indicator of whether the spaceship crashes
bool fellDown = false;

// these control the elapse of time
double time;
double timeSpeed;

// state of the controls for the camera
struct ControlStates
{
	bool forward, backward, left, right, yawLeft, yawRight;
} controls;

Camera camera;
SolarSystem *galaxy;

// save the galaxy and the camera, saving the current status
void saveModel(void)
{
	std::ofstream outfile("status.dat", std::ios::binary | std::ios::out);
	outfile.write((char *)galaxy, sizeof(SolarSystem));
	outfile.write((char *)&camera, sizeof(camera));
	outfile.close();
}

// load the galaxy and the camera, restoring the status
void loadModel(void)
{
	std::ifstream infile("status.dat", std::ios::binary | std::ios::in);
	if (infile.is_open())
	{
		infile.read((char *)galaxy, sizeof(SolarSystem));
		infile.read((char *)&camera, sizeof(camera));
	}
	infile.close();
}

// timer function called every 10ms or more
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

// initialize the system
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	// set up lighting, single lighting source as the Sun
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat matAmbience[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat matShininess[] = {20.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lightSpecular[] = {1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// load all image data

	// load the spaceship
	window = new TGA("images/window.tga");
	stars = new TGA("images/stars.tga");
	moon = new TGA("images/moon.tga");
	topSafe = new TGA("images/topSafe.tga");
	topFrame = new TGA("images/topFrame.tga");
	topDanger = new TGA("images/topDanger.tga");
	crashed = new TGA("images/crashed.tga");
	vertical = new TGA("images/vertical.tga");
	horizontal = new TGA("images/horizontal.tga");
	black = new TGA("images/black.tga");
	control = new TGA("images/control.tga");
	mirror = new TGA("images/mirror.tga");
	mirrorMid = new TGA("images/mirrorMid.tga");

	// load planets
	sun = new TGA("images/sun.tga");
	mercury = new TGA("images/mercury.tga");
	venus = new TGA("images/venus.tga");
	earth = new TGA("images/earth.tga");
	mars = new TGA("images/mars.tga");
	jupiter = new TGA("images/jupiter.tga");
	saturn = new TGA("images/saturn.tga");
	uranus = new TGA("images/uranus.tga");
	neptune = new TGA("images/neptune.tga");
	pluto = new TGA("images/pluto.tga");
	wormhole_pic = new TGA("images/black1.tga");
	other_planets[1] = new TGA("images/1.tga");
	other_planets[2] = new TGA("images/2.tga");
	other_planets[3] = new TGA("images/3.tga");
	other_planets[4] = new TGA("images/4.tga");
	other_planets[5] = new TGA("images/5.tga");
	other_planets[6] = new TGA("images/6.tga");
	other_planets[7] = new TGA("images/7.tga");
	other_planets[8] = new TGA("images/8.tga");
	other_planets[9] = new TGA("images/9.tga");
	other_planets[10] = new TGA("images/10.tga");
	other_planets[11] = new TGA("images/11.tga");
	sunPic[1] = new TGA("images/sun1.tga");
	sunPic[2] = new TGA("images/sun2.tga");
	sunPic[0] = new TGA("images/sun3.tga");

	galaxy = new SolarSystem();

	// set up time
	time = 2.552f;
	timeSpeed = 0.1f;

	// set controls
	controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.yawLeft = false;
	controls.yawRight = false;

	timer(0);
}

void drawCube(void)
{
	glBegin(GL_QUADS);
	// new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
}


void display(void)
{
	// update time
	time += timeSpeed;
	galaxy->calculatePositions(time);

	float min_distance = galaxy->testDistancewithPlanet(camera);

	float involve_distance = galaxy->testDistancewithWormhole(camera);

	if (min_distance < 0.001f)
		fellDown = true;

	// draw a new scene to inform the user that the spaceship crashes
	if (fellDown)
	{
		glBindTexture(GL_TEXTURE_2D, crashed->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(1200.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(1200.0f, 700.0f);
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 700.0f);
		glEnd();

		glFlush();
		glutSwapBuffers();

		return;
	}

	// generate a new galaxy when the spaceship is absorbed by the wormhole
	if (involve_distance < 0.001f)
	{
		srand((int)time);
		int xixi = rand();
		if (xixi % 6 == 0)
			galaxy = new SolarSystem();
		else
			galaxy = new SolarSystem(0);
		camera.reset();
	}


	if (controls.forward) camera.forward();		
	if (controls.backward) camera.backward();
	if (controls.left) camera.left();			
	if (controls.right) camera.right();
	if (controls.yawLeft) camera.yawLeft();		
	if (controls.yawRight) camera.yawRight();
	
	// set the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)screenWidth / (float)screenHeight, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.transformOrientation();

	// draw the skybox
	glBindTexture(GL_TEXTURE_2D, stars->getTextureHandle());
	drawCube();
	camera.transformTranslation();

	GLfloat lightPosition[] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// render the solar system
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	galaxy->render();
	glDisable(GL_LIGHTING);
	if (showOrbits)
		galaxy->renderOrbits();
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, (GLdouble)screenHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// draw the spaceship
	if (starshipView)
	{
		double x, y;
		x = 6; y = 150;

		glBindTexture(GL_TEXTURE_2D, vertical->getTextureHandle());
		for (int k = 1; k < 5; k++)
		{
			if (k == 1 || k == 4) 
				y = 200;
			else 
				y = 0;
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f), glVertex2f(screenWidth / 5 * k, y);
			glTexCoord2f(1.0f, 0.0f), glVertex2f(screenWidth / 5 * k + x, y);
			glTexCoord2f(1.0f, 1.0f), glVertex2f(screenWidth / 5 * k + x, screenHeight);
			glTexCoord2f(0.0f, 1.0f), glVertex2f(screenWidth / 5 * k, screenHeight);
			glEnd();
		}

		glBegin(GL_QUADS);
		double k = 100;
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5, y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 + x, y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 + x + k, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 + k, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 * 4, y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * 4 + x, y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 4 + x - k, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * 4 - k, 0.0f);
		glEnd();

		y = 35.6;
		glBindTexture(GL_TEXTURE_2D, topFrame->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth, y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, y);
		glEnd();

		x = 364; y = 137;
		if (min_distance < 0.08f)
		{
			glBindTexture(GL_TEXTURE_2D, topDanger->getTextureHandle());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, topSafe->getTextureHandle());
		}
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f((screenWidth - x) / 2, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex2f((screenWidth - x) / 2 + x, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f((screenWidth - x) / 2 + x, y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f((screenWidth - x) / 2, y);
		glEnd();

		x = 6; y = 100;
		glBindTexture(GL_TEXTURE_2D, horizontal->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, screenHeight);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0f, screenHeight - x);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 + x / 2, screenHeight - y - x);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 + x / 2, screenHeight - y);
								  
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 + x / 2, screenHeight - y - x);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 + x / 2, screenHeight - y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 2 + x / 2, screenHeight - 1.5 * y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * 2 + x / 2, screenHeight - 1.5 * y - x);
								  
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 * 3 + x / 2, screenHeight - 1.5 * y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * 3 + x / 2, screenHeight - 1.5 * y - x);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 4 + x / 2, screenHeight - y - x);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * 4 + x / 2, screenHeight - y);
								  
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 * 4 + x / 2, screenHeight - y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * 4 + x / 2, screenHeight - y - x);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth + x / 2, screenHeight - x);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth + x / 2, screenHeight);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, black->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, screenHeight);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 + x / 2, screenHeight - y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 2 + x / 2, screenHeight - 1.5 * y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * 2 + x / 2, screenHeight);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth, screenHeight);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * 4 + x / 2, screenHeight - y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 3 + x / 2, screenHeight - 1.5 * y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * 3 + x / 2, screenHeight);
		glEnd();

		x = 400; y = 150;
		glBindTexture(GL_TEXTURE_2D, control->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f((screenWidth - x) / 2, screenHeight - y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f((screenWidth - x) / 2 + x, screenHeight - y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f((screenWidth - x) / 2 + x, screenHeight);
		glTexCoord2f(0.0f, 1.0f); glVertex2f((screenWidth - x) / 2, screenHeight);
		glEnd();

		x = 80; k = 1.67; int h = 40;
		glBindTexture(GL_TEXTURE_2D, mirror->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 * k, screenHeight - y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * k + x, screenHeight - y - h);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * k + x, screenHeight - y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * k, screenHeight - y + h);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 * 3, screenHeight - y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * 3 + x, screenHeight - y + h);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 3 + x, screenHeight - y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * 3, screenHeight - y - h);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, mirrorMid->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(screenWidth / 5 * k + x, screenHeight - y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(screenWidth / 5 * 3, screenHeight - y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(screenWidth / 5 * 3, screenHeight - y - h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(screenWidth / 5 * k + x, screenHeight - y - h);
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}

// registered function that handles issues when keys are pressed
void keyDown(unsigned char key, int x, int y)
{
	if (key > '0' && key <= '9')
	{
		// point at the specified planet
		float vec[3];
		if (!galaxy->hasPlanet(key)) return;
		galaxy->getPlanetPosition(key - '0', vec);
		camera.pointAt(vec);
		planetSelected = key - '0';
	}

	switch (key)
	{
	case '-':
		timeSpeed /= 2.0f; 
		break;
	case '=':
		timeSpeed *= 2.0f;
		break;
	case 'u':
		starshipView = !starshipView;
		break;
	case 'o':
		showOrbits = !showOrbits;
		break;
	case ',':
		camera.slowDown();
		break;
	case '.':
		camera.speedUp();
		break;
	case 'w':
		controls.forward = true;
		break;
	case 's':
		controls.backward = true;
		break;
	case 'a':
		controls.yawLeft = true;
		break;
	case 'd':
		controls.yawRight = true;
		break;

	// reset the system when the spaceship crashes
	case 't':
		fellDown = false;
		camera.reset();
		break;
	case 'p':
		camera.saveImage();
		break;
	case 'b':
		saveModel();
		break;
	case 'n':
		loadModel();
		break;
	}

}

// registered function that handles issues when keys are released
void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		controls.forward = false;
		break;
	case 's':
		controls.backward = false;
		break;
	case 'a':
		controls.yawLeft = false;
		break;
	case 'd':
		controls.yawRight = false;
		break;
	}
}

// set mouse-camera control connection
void mouse(int x, int y){
	camera.setMouse(x, y);
}

// called when the shape of the window is changed
void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Space Wander Man");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	return 0;
}