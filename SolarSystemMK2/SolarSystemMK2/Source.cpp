#include <iostream>
#include <time.h> //For randomisation
#include <vector> //For asteroid belt
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Asteroid.h"
#include "Celestial.h"
#include <glut.h> //OpenGL(GLUT)
#include <windows.h> //For music
#include "CameraHelper.h"

//CONSTANTS
#define farClipPlane 100000.0 //Max render distance variable
#define sunRadius 348
#define au 1490 //Astronomical Unit (distance from Sun to Earth)
#define numberOfAsteroids 2000

using namespace std;

typedef vector< vector <GLfloat> > glVector2d;

//GLOBAL TIMEWARP INTEGER
//USED TO CHANGE THE OVERALL ROTATION OF THE SOLAR SYSTEM
//ACCESSED IN SPINSPEED - ALTERED BY KEYPRESS
int timeWarp = 1;
//GLOBAL PRESSED BOOLAN
//USED DO DETERMINE IF THE CAMERA HAS TO SNAP TO SOMETHING OR NOT
//ACCESSED IN DRAW - ALTERED IN KEYBOARD
bool pressed = false;
//GLOBAL PLANNET NUMBER INTEGER
//USED TO DETERMINE WHICH PLANET TO SNAP THE CAMERA TO
//ACCESSED IN DRAW - ALTERED IN KEYBOARD
int plannetNum = 0;

//CameraHelper object. Stores all the camera variablesL: the lookX/Y and camera position.
CameraHelper cameraHelper;

//Changes made for easier viewing.
//Sun Radius is aprox. 1/2000 from real life.
//Planet Radius is aprox. 1/200 from real life.
//Distance Between Planets and Sun are aprox. 1/100000 from real life.
//Distance Between Planets and Sun After Jupiter has been reduced a further 1/4.
//Plutos radius has been increesed 10x so it is visible.

Celestial Sun("Sun", 0, sunRadius, 0.88, 0.81, 0.13, 1.0);

//INNER PlANNETS
Celestial Mercury("Mercury", sunRadius + 580, 12, 0.89, 0.58, 0.23, 0.3);

Celestial Venus("Venu", sunRadius + 1080, 30, 0.88, 0.82, 0.4, 0.3);

Celestial Earth("Earth", sunRadius + au, 31, 0.2, 0.69, 0.81, 0.7);
Celestial Moon("Moon", 31 + 9, 8, 0.43, 0.43, 0.43, 0.3);

Celestial Mars("Mars", sunRadius + 2280, 16, 0.79, 0.29, 0.11, 0.2);
Celestial Fobos("Fobos", 16 + 9, 5, 0.43, 0.43, 0.43, 0.3);
Celestial Deimos("Deimos", 16 + 12, 3, 0.43, 0.43, 0.43, 0.3);

//========================ASTEROID BELT=======================//
vector <Asteroid> beltOne; //Holds all the asteroid objects

//OUTER PLANETS
Celestial Jupiter("Jupiter", sunRadius + 7783, 349, 0.69, 0.47, 0.21, 0.2);

Celestial Saturn("Saturn", sunRadius + 14294 - 14294 / 4, 291, 0.84, 0.65, 0.43, 0.2);
glVector2d saturnRings;

Celestial Uranus("Uranus", sunRadius + 28709 - 28709 / 4, 126, 0.43, 0.81, 0.84, 0.2);

Celestial Neptune("Neptune", sunRadius + 45043 - 45043 / 4, 123, 0.1, 0.21, 0.76, 0.2);

Celestial Pluto("Pluto", sunRadius + 59135 - 59135 / 4, 60, 0.76, 0.68, 0.1, 0.2);

//Commet
Asteroid commet(0.0);

//List of names
string names[] = { Mercury.name, Venus.name, Earth.name, Mars.name, Jupiter.name, Saturn.name, Uranus.name, Neptune.name, Pluto.name };

//=========================UTILITY FUNCTIONS==============================//

//Incrememnts the spin variables and insures a infinite loop.
void spinScene()
{
	static int lastFrameTime = 0.0; //Integer to hold the time of the current frame
	static int totalTime = 0.0;	//Integer to hold the time since the current frame

	//if statement used only during the first call of this function
	if (lastFrameTime == 0)
		lastFrameTime = glutGet(GLUT_ELAPSED_TIME);

	/*================ WHAT I WANT IT TO DO ===================*/

	//Iterates the spin speeds
	Sun.spin += 0.2 * timeWarp;
	Mercury.spin += 0.5 * timeWarp;
	Venus.spin += 0.2 * timeWarp;
	Earth.spin += 0.6 * timeWarp;
	Mars.spin += 0.1 * timeWarp;
	beltOne[0].spin += 0.001 * timeWarp;
	Jupiter.spin += 0.06 * timeWarp;
	Saturn.spin += 0.03 * timeWarp;
	Uranus.spin += 0.02 * timeWarp;
	Neptune.spin += 0.05 * timeWarp;
	Pluto.spin += 0.1 * timeWarp;
	commet.spin += 0.001 * timeWarp;
	
	//Makes the spin infinite
	if (Mercury.spin > 360.0)
		Mercury.spin = Mercury.spin - 360.0;
	if (Venus.spin > 360.0)
		Venus.spin = Venus.spin - 360.0;
	if (Earth.spin > 360.0)
		Earth.spin = Earth.spin - 360.0;
	if (Mars.spin > 360.0)
		Mars.spin = Mars.spin - 360.0;
	if (beltOne[0].spin > 360.0)
		beltOne[0].spin = beltOne[0].spin - 360.0;
	if (Jupiter.spin > 360.0)
		Jupiter.spin = Jupiter.spin - 360.0;
	if (Saturn.spin > 360.0)
		Saturn.spin = Saturn.spin - 360.0;
	if (Uranus.spin > 360.0)
		Uranus.spin = Uranus.spin - 360.0;
	if (Neptune.spin > 360.0)
		Neptune.spin = Neptune.spin - 360.0;
	if (Pluto.spin > 360.0)
		Pluto.spin = Pluto.spin - 360.0;
	if (commet.spin > 360.0)
		commet.spin = commet.spin - 360.0;

	//Gets the current time
	totalTime = glutGet(GLUT_ELAPSED_TIME);

	//Replaces old time with new time untill the desired milisecond delay is reached (16ms = 60fps)
	while (totalTime - lastFrameTime < 16)
	{
		totalTime = glutGet(GLUT_ELAPSED_TIME);
	}

	//Resets the lastFrameTime to the current time so it is ready for the next call by the glutIdleFunc();
	lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
}

//Creates the series of points needed to create a flat donut shape and pushes them in a vector.
//Uses the same technique as the Asteroid::generateRandomPoints method.
void saturnRing()
{
	//Making sure I am building the ring on a clean modelview matrix.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vector<GLfloat> xyz; //Holds the coords before they are pushed in the main vector

	int x;
	int y = 0.0;
	int z;

	int increment = 360 / 18; //Angle by which 
	
	//Inner and outer radius for the Rings (To scale with the planet)
	double innerR = 291 + 35;
	double outerR = 291 + 300;

	for (int iterator = 0; iterator <= 18; iterator++)
	{
		
		double angle = increment * iterator;
		
		if (angle == 360)
			angle = 0;

		//Inner points
		x = cos(angle * 3.14 / 180.0) * innerR; 
		z = sin(angle * 3.14 / 180.0) * innerR;
		xyz.push_back(x);
		xyz.push_back(y);
		xyz.push_back(z);
		saturnRings.push_back(xyz);
		xyz.clear();

		//Outer points
		x = cos(angle * 3.14 / 180.0) * outerR;
		z = sin(angle * 3.14 / 180.0) * outerR;
		xyz.push_back(x);
		xyz.push_back(y);
		xyz.push_back(z);
		saturnRings.push_back(xyz);
		xyz.clear();
	}
}

//Equivalent of the Celestial::createPlanet method.
void createSaturnRings(GLfloat spinSpeed)
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);
	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);

	glTranslated(0.0, 0.0, sunRadius + 14294 - 14294 / 4);

	glBegin(GL_QUAD_STRIP);
	for (int iterator = 0; iterator <= 37; iterator++)
	{
		glVertex3f(saturnRings[iterator][0], saturnRings[iterator][1], saturnRings[iterator][2]);
	}
	glEnd();

	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();

	
}

//Prints the text for the plannet names. It is centered and abouve each plannet.
void printLabels(int plannetNumber, int yOffset, GLdouble spinValue, int distance)
{

	GLfloat textSpec[4] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat height;
	GLfloat width;

	//Current plannet position
	GLdouble lookX = cos((-spinValue + 90) * 3.14 / 180.0) * distance;
	GLdouble lookZ = sin((-spinValue + 90) * 3.14 / 180.0) * distance;

	GLdouble x, y, z; //2D coords

	GLdouble worldSpace[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, worldSpace);

	GLdouble cameraSpace[16];
	glGetDoublev(GL_PROJECTION_MATRIX, cameraSpace);

	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	gluProject(lookX, yOffset, lookZ, worldSpace, cameraSpace, view, &x, &y, &z);

	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	//Display Text
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0.89, 0.58, 0.23);
	glMaterialfv(GL_FRONT, GL_SPECULAR, textSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.5);

	glDisable(GL_LIGHTING);

	/*Because glRasterPos works both on the projection AND modelview matricies we
	ensure we are working with a clean slate by loading an identity matrix for both
	matrix modes and we create and orthographic projection the size of the screen.
	We pop both matrix stacks after the desired operation is done.*/

	//Porjection mode clear.
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, 1);

	//Modelview mode clear.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Text position on screen in pixels
	//glRasterPos2f(width / 2 - names[plannetNumber].size() * 4.5, height / 2 + yOffset);
	glRasterPos2f(x - names[plannetNumber].size() * 4.5, y);
	
	//Actually prints out the text.
	for (size_t iterator = 0; iterator <= names[plannetNumber].size(); iterator++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, names[plannetNumber][iterator]);
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	//We pop the matricies and return to the state we were before the if statement
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

//Calculates the position the camera should be at relative to the planet and changes the look at position of the camera.
//Also the text functions are here.
void snapCamera(bool pressed, int plannetNum)
{
	
	/*Position of cameraLookX/Y is calculated form the current spin value and overall distance of the plannet.
	The position of the camera is calculated the same way and the distance between the planet and the Sun is
	multiplied by a value to determine if the camera is behind or in front of the plannet (1 is inside the plannet)*/
	

	//Mercury
	if (pressed && plannetNum == 1)
	{
		cameraHelper.setCameraLookX(cos((-Mercury.spin + 90) * 3.14 / 180.0) * Mercury.distance);
		cameraHelper.setCameraLookZ(sin((-Mercury.spin + 90) * 3.14 / 180.0) * Mercury.distance);


		cameraHelper.setLR(cos((-Mercury.spin + 75.0) * 3.14 / 180.0) * Mercury.distance*0.5);
		cameraHelper.setUD(Mercury.radius * 3.0);
		cameraHelper.setFB(sin((-Mercury.spin + 75.0) * 3.14 / 180.0) * Mercury.distance*0.5);
	}
	//Venus
	if (pressed && plannetNum == 2)
	{
		cameraHelper.setCameraLookX(cos((-Venus.spin + 90.0) * 3.14 / 180.0) * Venus.distance);
		cameraHelper.setCameraLookZ(sin((-Venus.spin + 90.0) * 3.14 / 180.0) * Venus.distance);


		cameraHelper.setLR(cos((-Venus.spin + 75.0) * 3.14 / 180.0) * Venus.distance*0.5);
		cameraHelper.setUD(Venus.radius * 3.0);
		cameraHelper.setFB(sin((-Venus.spin + 75.0) * 3.14 / 180.0) * Venus.distance*0.5);
	}
	//Earth
	if (pressed && plannetNum == 3)
	{
		cameraHelper.setCameraLookX(cos((-Earth.spin + 90.0) * 3.14 / 180.0) * Earth.distance);
		cameraHelper.setCameraLookZ(sin((-Earth.spin + 90.0) * 3.14 / 180.0) * Earth.distance);


		cameraHelper.setLR(cos((-Earth.spin + 75.0) * 3.14 / 180.0) * Earth.distance*0.5);
		cameraHelper.setUD(Earth.radius * 3.0);
		cameraHelper.setFB(sin((-Earth.spin + 75.0) * 3.14 / 180.0) * Earth.distance*0.5);
	}
	//Mars
	if (pressed && plannetNum == 4)
	{
		cameraHelper.setCameraLookX(cos((-Mars.spin + 90.0) * 3.14 / 180.0) * Mars.distance);
		cameraHelper.setCameraLookZ(sin((-Mars.spin + 90.0) * 3.14 / 180.0) * Mars.distance);


		cameraHelper.setLR(cos((-Mars.spin + 75.0) * 3.14 / 180.0) * Mars.distance*0.5);
		cameraHelper.setUD(Mars.radius * 3.0);
		cameraHelper.setFB(sin((-Mars.spin + 75.0) * 3.14 / 180.0) * Mars.distance*0.5);
	}
	//Jupiter
	if (pressed && plannetNum == 5)
	{
		cameraHelper.setCameraLookX(cos((-Jupiter.spin + 90.0) * 3.14 / 180.0) * Jupiter.distance);
		cameraHelper.setCameraLookZ(sin((-Jupiter.spin + 90.0) * 3.14 / 180.0) * Jupiter.distance);


		cameraHelper.setLR(cos((-Jupiter.spin + 75.0) * 3.14 / 180.0) * Jupiter.distance*1.5);
		cameraHelper.setUD(Jupiter.radius * 3.0);
		cameraHelper.setFB(sin((-Jupiter.spin + 75.0) * 3.14 / 180.0) * Jupiter.distance*1.5);
	}
	//Saturn
	if (pressed && plannetNum == 6)
	{
		cameraHelper.setCameraLookX(cos((-Saturn.spin + 90) * 3.14 / 180.0) * Saturn.distance);
		cameraHelper.setCameraLookZ(sin((-Saturn.spin + 90) * 3.14 / 180.0) * Saturn.distance);


		cameraHelper.setLR(cos((-Saturn.spin + 75.0) * 3.14 / 180.0) * Saturn.distance*1.5);
		cameraHelper.setUD(Saturn.radius * 3.0);
		cameraHelper.setFB(sin((-Saturn.spin + 75.0) * 3.14 / 180.0) * Saturn.distance*1.5);
	}
	//Uranus
	if (pressed && plannetNum == 7)
	{
		cameraHelper.setCameraLookX(cos((-Uranus.spin + 90.0) * 3.14 / 180.0) * Uranus.distance);
		cameraHelper.setCameraLookZ(sin((-Uranus.spin + 90.0) * 3.14 / 180.0) * Uranus.distance);


		cameraHelper.setLR(cos((-Uranus.spin + 87.0) * 3.14 / 180.0) * Uranus.distance*1.1);
		cameraHelper.setUD(Uranus.radius * 3.0);
		cameraHelper.setFB(sin((-Uranus.spin + 87.0) * 3.14 / 180.0) * Uranus.distance*1.1);
	}
	//Neptune
	if (pressed && plannetNum == 8)
	{
		cameraHelper.setCameraLookX(cos((-Neptune.spin + 90.0) * 3.14 / 180.0) * Neptune.distance);
		cameraHelper.setCameraLookZ(sin((-Neptune.spin + 90.0) * 3.14 / 180.0) * Neptune.distance);


		cameraHelper.setLR(cos((-Neptune.spin + 86.0) * 3.14 / 180.0) * Neptune.distance*1.1);
		cameraHelper.setUD(Neptune.radius * 3.0);
		cameraHelper.setFB(sin((-Neptune.spin + 86.0) * 3.14 / 180.0) * Neptune.distance*1.1);
	}
	//Pluto
	if (pressed && plannetNum == 9)
	{
		cameraHelper.setCameraLookX(cos((-Pluto.spin + 90.0) * 3.14 / 180.0) * Pluto.distance);
		cameraHelper.setCameraLookZ(sin((-Pluto.spin + 90.0) * 3.14 / 180.0) * Pluto.distance);


		cameraHelper.setLR(cos((-Pluto.spin + 89.0) * 3.14 / 180.0) * Pluto.distance*1.05);
		cameraHelper.setUD(Pluto.radius * 3.0);
		cameraHelper.setFB(sin((-Pluto.spin + 89.0) * 3.14 / 180.0) * Pluto.distance*1.05);
	}
}


//Camera functions
void cameraRight(int value)
{
	cameraHelper.cameraLR += 100;
	glutPostRedisplay();
}
void cameraLeft(int value)
{
	cameraHelper.cameraLR -= 100;
	glutPostRedisplay();
}
void cameraUp(int value)
{
	cameraHelper.cameraUD += 100;
	glutPostRedisplay();
}
void cameraDown(int value)
{
	cameraHelper.cameraUD -= 100;
	glutPostRedisplay();
}
void cameraForward(int value)
{
	cameraHelper.cameraFB -= 100;
	glutPostRedisplay();
}
void cameraBack(int value)
{
	cameraHelper.cameraFB += 100;
	glutPostRedisplay();
}

//===================MAIN FUNCTIONS================================//
void draw(void)
{
	//Sets the positions for the two lights.
	GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat position2[] = { commet.Xoffset, 0.0, 99500+commet.Zoffset, 1.0 };

	//Clears color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	snapCamera(pressed, plannetNum);

	//Sets camera look at based on the six variables
	gluLookAt(cameraHelper.cameraLR, cameraHelper.cameraUD, cameraHelper.cameraFB, cameraHelper.cameraLookX, cameraHelper.cameraLookY, cameraHelper.cameraLookZ, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glLightfv(GL_LIGHT1, GL_POSITION, position2);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, (99500 + commet.Zoffset)/332);
	
	//SUN
	//Lighting disabled and then enabled again to exclude the
	//Sun object from the lighting pass and make it seem like its self eluminated
	glDisable(GL_LIGHTING);
	Sun.createStar(Sun.spin);
	glEnable(GL_LIGHTING);

	//Mercury
	Mercury.createPlanet(Mercury.spin);
	printLabels(0, Mercury.radius, Mercury.spin, Mercury.distance);

	//Venus
	Venus.createPlanet(Venus.spin);
	printLabels(1, Venus.radius, Venus.spin, Venus.distance);

	//Earth
	Earth.createPlanetMoon(Earth.spin);
	Moon.createMoon(Earth.spin);
	printLabels(2, Earth.radius, Earth.spin, Earth.distance);
	

	//Mars
	Mars.createPlanetMoon(Mars.spin);
	Fobos.createMultiMoon(Mars.spin);
	Deimos.createMoon(Mars.spin);
	printLabels(3, Mars.radius, Mars.spin, Mars.distance);

	//Asteroid Belt
	for (int iterator = 0; iterator <= numberOfAsteroids; iterator++)
	{ 
		beltOne[iterator].createAsteroid(beltOne[0].spin);
	}

	//Jupiter
	Jupiter.createPlanet(Jupiter.spin);
	printLabels(4, Jupiter.radius, Jupiter.spin, Jupiter.distance);

	//Saturn
	Saturn.createPlanet(Saturn.spin);
	createSaturnRings(Saturn.spin);
	printLabels(5, Saturn.radius, Saturn.spin, Saturn.distance);

	//Uranus
	Uranus.createPlanet(Uranus.spin);
	printLabels(6, Uranus.radius, Uranus.spin, Uranus.distance);

	//Neptune
	Neptune.createPlanet(Neptune.spin);
	printLabels(7, Neptune.radius, Neptune.spin, Neptune.distance);

	//Pluto
	Pluto.createPlanet(Pluto.spin);
	printLabels(8, Pluto.radius, Pluto.spin, Pluto.distance);

	//Commet
	commet.createCommet(commet.spin, 10000, 100000);


	glPopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

void initialise()
{
	//Values for Light1
	GLfloat defuse[] = { 0.0, 0.0, 100.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1, 1, 1, 1 };
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

	//Light one and settings needed for it to work.
	glLightfv(GL_LIGHT1, GL_DIFFUSE, defuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT1);

	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	//first two are lower left corner of viewport in pixels
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	//Picks the projection matrix stack to apply the following this
	//line statements
	glMatrixMode(GL_PROJECTION);

	//Efectively resets the matrix to a default state
	//ready for next operation
	glLoadIdentity();

	//FOV, aspectRatio, distance to near clipping plane, distance to far clipping plane
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, farClipPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Keyboard entry.
void Keyboard(unsigned char key, int A, int B)
{
	switch (key)
	{
	case 'r':
		glutIdleFunc(spinScene);
		break;
	case 't':
		glutIdleFunc(NULL);
		break;
	case 'z':
		timeWarp++;
		break;
	case 'x':
		timeWarp--;
		break;
	case '1':
		plannetNum = 1;
		pressed = true;
		break;
	case '2':
		plannetNum = 2;
		pressed = true;
		break;
	case '3':
		plannetNum = 3;
		pressed = true;
		break;
	case '4':
		plannetNum = 4;
		pressed = true;
		break;
	case '5':
		plannetNum = 5;
		pressed = true;
		break;
	case '6':
		plannetNum = 6;
		pressed = true;
		break;
	case '7':
		plannetNum = 7;
		pressed = true;
		break;
	case '8':
		plannetNum = 8;
		pressed = true;
		break;
	case '9':
		plannetNum = 9;
		pressed = true;
		break;
	case' ':
		pressed = false;
		cameraHelper.setCameraLookX(0.0);
		cameraHelper.setCameraLookZ(0.0);
		break;
	case 119:
		glutTimerFunc(100, cameraForward, 100);
		break;
	case 115:
		glutTimerFunc(100, cameraBack, 100);
		break;
	case 97:
		glutTimerFunc(100,cameraLeft, 100);
		break;
	case 100:
		glutTimerFunc(100, cameraRight, 100);
		break;
	case 101:
		glutTimerFunc(100,cameraUp, 100);
		break;
	case 113:
		glutTimerFunc(100, cameraDown, 100);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//Displays the menu.
void menu(void)
{
	printf("------------ OPTIONS -------------\n");
	printf("Press r to start rotation\n");
	printf("Press t to start rotation\n");
	printf("WASD Q and E controll basic camera movement\n");
	printf("Numbers from 1-9 correspond to the planets\n");
	printf("Z and X alter spin speed\n");
	printf("Press Esc to terminate the program\n");
	printf("----------------------------------\n");
}

int main(int argc, char **argv)
{
	menu();
	srand(time(0)); //Seeds the rand() function
	
	//Play music.
	PlaySound("The Complex.wav", NULL, SND_LOOP | SND_ASYNC);

	//Creates asteroids number of which is specified by numberOfAsteroids
	//and pushes the created objects in a vector
	for (int iterator = 0; iterator <= numberOfAsteroids; iterator++)
	{
		//Sets the distance to be roughly the width of the asteroid belt.
		int distance = 2*au + (rand() % (3*au - 2*au + 1));
		Asteroid asteroid(distance);
		beltOne.push_back(asteroid);
	}

	//Calls the makeStrip() function for every single object and gives it random
	//with, hight, depth and ofset values
	for (int iterator = 0; iterator <= numberOfAsteroids; iterator++)
	{
		int with = 10 + (rand() % (30 - 10 + 1)); // 10 - 30
		int height = 10 + (rand() % (20 - 10 + 1)); // 10 - 20
		int depth = 10 + (rand() % (20 - 10 + 1)); // 10 - 20
		int ofset = -30 + rand() % 60; // -30 - 30
		beltOne[iterator].makeStrip(with, height, depth, ofset);
	}

	//Makes the Commet
	int with = 10 + (rand() % (30 - 10 + 1)); // 10 - 30
	int height = 10 + (rand() % (20 - 10 + 1)); // 10 - 20
	int depth = 10 + (rand() % (20 - 10 + 1)); // 10 - 20
	int ofset = -30 + rand() % 60; // -30 - 30
	commet.makeStrip(with*3, height*3, depth*3, ofset*3);

	//Make the points for the Rings of Saturn
	saturnRing();

	glutInit(&argc, argv);

	//Double buffer, RGB and depth
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	//Set up the window
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Solar System");

	initialise();

	//Registers the functions to GLUT
	glutDisplayFunc(draw);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(Keyboard);

	glutMainLoop();

	return 0;
}