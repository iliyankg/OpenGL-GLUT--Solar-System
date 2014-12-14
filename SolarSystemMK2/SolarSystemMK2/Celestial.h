#pragma once
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include<glut.h>


class Celestial
{
public:
	Celestial(std::string name, int dist, int rad, GLfloat R, GLfloat G, GLfloat B, GLfloat specGloss);
	~Celestial();
	
	std::string name;

	GLdouble spin;

	//Creator values
	int distance;
	int radius;

	//Deffuse
	GLfloat red;
	GLfloat green;
	GLfloat blue;

	//Spec and Gloss
	GLfloat spec[4];
	GLfloat gloss;

	//Celestial Objects
	void createStar(GLdouble spinSpeed);
	void createPlanet(GLdouble speedSpin);
	void createPlanetMoon(GLdouble speedSpin);
	void createMultiMoon(GLdouble spinSpeed);
	void createMoon(GLdouble speedSpin);
};

