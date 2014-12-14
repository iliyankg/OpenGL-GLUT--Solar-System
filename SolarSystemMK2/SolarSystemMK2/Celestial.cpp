#include "Celestial.h"

Celestial::Celestial(std::string name, int dist, int rad, GLfloat R, GLfloat G, GLfloat B, GLfloat specGloss)
{
	distance = dist;
	radius = rad;

	red = R;
	green = G;
	blue = B;

	spec[0] = red;
	spec[1] = green;
	spec[2] = blue;
	spec[3] = specGloss;

	gloss = spec[3];

	this->name = name;

	spin = 0.0;
}

Celestial::~Celestial()
{
}

void Celestial::createStar(GLdouble spinSpeed)
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);

	glRotated((GLdouble) spinSpeed, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, distance);
	glutSolidSphere(radius, 20.0, 10.0);

	glDisable(GL_COLOR_MATERIAL);
	
	glPopMatrix();
}

/*
The only diffeerence between the following four methods
is where th push_matrix and pop_matrix are called.

All methods have a finished push - pop pair to make them around themselves.

createPlanet: has a push and pop matrix methods making it just orbit the sun.
createPlanetMoon: lacks a pop matrix method allowing moon(s) to be added.
createMultiMoon: lacks push or pop matrix meaning it will rotate around the planet and
allow for more moons to be added.
createMoon: has a pop matrix method finishing the Planet-Moon creation.
*/
void Celestial::createPlanet(GLdouble spinSpeed)
{
	glPushMatrix();

	//Set material properties.
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);
	
	//Makes orbit
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glutSolidTorus(4, distance, 5, 30);
	glPopMatrix();

	//Rotates planet around it self
	glPushMatrix();
	
	//Check for the two planets that rotate counter clockwise.
	if ((this->distance == 348 + 1080) || (this->distance == 348 + 28709 - 28709 / 4))
		glRotated(-(GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	else
		glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glPopMatrix();

	glRotated((GLdouble) spinSpeed, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, distance);
	glutSolidSphere(radius, 20.0, 10.0);
	
	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}

void Celestial::createPlanetMoon(GLdouble spinSpeed)
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);

	//Makes orbit
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glutSolidTorus(4, distance, 5, 30);
	glPopMatrix();

	//Rotates planet around it self
	glPushMatrix();
	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glPopMatrix();

	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, distance);
	glutSolidSphere(radius, 20.0, 10.0);

	glDisable(GL_COLOR_MATERIAL);
}

void Celestial::createMultiMoon(GLdouble spinSpeed)
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);

	//Rotates planet around it self
	glPushMatrix();
	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glPopMatrix();

	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, distance);
	glutSolidSphere(radius, 20.0, 10.0);

	glDisable(GL_COLOR_MATERIAL);
}

void Celestial::createMoon(GLdouble spinSpeed)
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);

	//Rotates planet around it self
	glPushMatrix();
	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glPopMatrix();

	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, distance);
	glutSolidSphere(radius, 20.0, 10.0);

	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}





