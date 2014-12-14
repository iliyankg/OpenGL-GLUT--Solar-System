#pragma once
/*TVector library from 217CR module*/
#include "TVector.h"
#include<math.h>
#include<vector>
#include<glut.h>


typedef std::vector< std::vector <GLfloat> > glVector2d;

class Asteroid
{

private:
	bool bNorm = true;

	std::vector< std::vector<double> > normals;

	inline void calcCornerNorm(int vOne, int vTwo, int vThree, int vFour);
	inline void calcMainNorm(int vOne, int vTwo, int vThree, int vFour, int vFive);

public:

	GLfloat spin;

	//Holds xyz of the points when creating an asteroid
	glVector2d stripOne;
	GLdouble Xoffset;
	GLdouble Zoffset;

	//Creator values
	int distance;
	int direction;
	int ofset;

	//Deffuse
	GLfloat red;
	GLfloat green;
	GLfloat blue;

	//Spec and Gloss
	GLfloat spec[4];
	GLfloat gloss;

	//Constructor
	Asteroid(int dist);
	~Asteroid();

	//Asteroid methods
	std::vector<GLfloat> generateRandomPoint(int fromX, int toX, int fromY, int toY, int fromZ, int toZ);
	void makeStrip(int width, int height, int depth, int ofset);
	void asteroidDraw();
	void createAsteroid(GLdouble spinSpeed);
	void createCommet(GLdouble spinSpeed, GLdouble radOne, GLdouble ranTwo);
};


