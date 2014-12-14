#include "Asteroid.h"

/*Private functions*/
/*
The four arguments represent:
vOne - the vertex for which the normal is calculated;
vTwo, vThree, vFour - the three verticies connected to the first one.
*/
void Asteroid::calcCornerNorm(int vOne, int vTwo, int vThree, int vFour)
{
	//Store the three verticies in TVectors
	TVector one(stripOne[vOne][0], stripOne[vOne][1], stripOne[vOne][2]);
	TVector two(stripOne[vTwo][0], stripOne[vTwo][1], stripOne[vTwo][2]);
	TVector three(stripOne[vThree][0], stripOne[vThree][1], stripOne[vThree][2]);
	TVector four(stripOne[vFour][0], stripOne[vFour][1], stripOne[vFour][2]);

	//Get the three vectors that are going to be used for the cross product.
	TVector v1 = two - one;
	TVector v2 = three - one;
	TVector v3 = four - one;

	four.~TVector();
	
	//Get the three individual vectors perpendicular to each plane represented 
	//by each two vectors.
	one = crossPrdct(v1, v2);
	two = crossPrdct(v2, v3);
	three = crossPrdct(v3, v1);

	//Calculate the direction of the normal vector
	TVector normal = one + two + three;

	std::vector <double> norm;
	//Populate the temporary normal with the normalized normal vector.
	norm.push_back(normal.getUnit()[0]);
	norm.push_back(normal.getUnit()[1]);
	norm.push_back(normal.getUnit()[2]);
	//Push the final normal on the normals vector
	normals.push_back(norm);


	
	/*Destroying the TVectors used for calculating the normals*/
	one.~TVector();
	two.~TVector();
	three.~TVector();
	

	v1.~TVector();
	v2.~TVector();
	v3.~TVector();

	normal.~TVector();
}
/*
This method calculates the normal for each vertex that is not on the corners.
Each one of them has four other ones connected to it hence a new method is needed. 
*/
void Asteroid::calcMainNorm(int vOne, int vTwo, int vThree, int vFour, int vFive)
{
	//Store the three verticies in TVectors
	TVector one(stripOne[vOne][0], stripOne[vOne][1], stripOne[vOne][2]);
	TVector two(stripOne[vTwo][0], stripOne[vTwo][1], stripOne[vTwo][2]);
	TVector three(stripOne[vThree][0], stripOne[vThree][1], stripOne[vThree][2]);
	TVector four(stripOne[vFour][0], stripOne[vFour][1], stripOne[vFour][2]);
	TVector five(stripOne[vFive][0], stripOne[vFive][1], stripOne[vFive][2]);

	//Get the three vectors that are going to be used for the cross product.
	TVector v1 = two - one;
	TVector v2 = three - one;
	TVector v3 = four - one;
	TVector v4 = five - one;
	
	five.~TVector();

	//Get the three individual vectors perpendicular to each plane represented 
	//by each two vectors.
	one = crossPrdct(v1, v2);
	two = crossPrdct(v2, v3);
	three = crossPrdct(v3, v4);
	four = crossPrdct(v4, v1);

	//Calculate the direction of the normal vector
	TVector normal = one + two + three + four;

	std::vector <double> norm;
	//Populate the temporary normal with the normalized normal vector.
	norm.push_back(normal.getUnit()[0]);
	norm.push_back(normal.getUnit()[1]);
	norm.push_back(normal.getUnit()[2]);
	//Push the final normal on the normals vector
	normals.push_back(norm);


	/*Destroying the TVectors used for calculating the normals*/
	
	one.~TVector();
	two.~TVector();
	three.~TVector();
	four.~TVector();

	v1.~TVector();
	v2.~TVector();
	v3.~TVector();
	v4.~TVector();

	normal.~TVector();
}

Asteroid::Asteroid(int dist)
{
	distance = dist;
	direction = rand() % 360;

	//Deffuse
	red = 0.13;
	green = 0.13;
	blue = 0.13;

	//Spec and Gloss
	spec[0] = red;
	spec[1] = green;
	spec[2] = blue;
	spec[3] = 0.33;
	gloss = 0.1;
}


Asteroid::~Asteroid()
{
}

//Generates random points based on an imaginary grid defined by the imput values.
std::vector<GLfloat> Asteroid::generateRandomPoint(int fromX, int toX, int fromY, int toY, int fromZ, int toZ)
{
	std::vector<GLfloat> xyz;

	int x;
	int y;
	int z;

	//Random Num Generation
	//X
	if (fromX < 0 && toX < 0)
	{
		x = toX + (rand() % (fromX - toX + 1));
	}
	else
	{
		x = fromX + (rand() % (toX - fromX + 1));
	}

	//Y
	if (fromY < 0 && toY < 0)
	{
		y = toY + (rand() % (fromY - toY + 1));
	}
	else
	{
		y = fromY + (rand() % (toY - fromY + 1));
	}

	//Z
	if (fromZ < 0 && toZ < 0)
	{
		z = toZ + (rand() % (fromZ - toZ + 1));
	}
	else
	{
		z = fromZ + (rand() % (toZ - fromZ + 1));
	}

	//Side Checks
	xyz.push_back(x);
	xyz.push_back(y);
	xyz.push_back(z);

	return xyz;
}

//Makes the main asteroid strip
void Asteroid::makeStrip(int width, int height, int depth, int ofSet)
{
	//Integers depicting an imaginary grid for the xyz random number generation
	int fromX = width / 4;
	int toX = width / 2;
	int fromYone = height - height / 4;
	int toYone = height;
	int fromYtwo = height / 8;
	int toYtwo = height / 2 - height / 8;
	int fromZ = width / 4;
	int toZ = width / 2;

	ofset = ofSet;

	/*ORDER OF push_back IS IMPORTANT! DO!NOT!TOUCH!*/

	/*=============TOP===============*/
	//Top Back Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, fromYone, toYone, -fromZ, -toZ));

	//Top Front Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, fromYone, toYone, fromZ, toZ));

	//Top Back Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, fromYone, toYone, -fromZ, -toZ));

	//Top Front Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, fromYone, toYone, fromZ, toZ));


	/*=====================LEFT=====================*/
	//Top Middle Back Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, fromYtwo, toYtwo, -fromZ, -toZ));

	//Top Middle Front Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, fromYtwo, toYtwo, fromZ, toZ));

	//Bot Middle Back Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, -fromYtwo, -toYtwo, -fromZ, -toZ));

	//Bot Middle Front Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, -fromYtwo, -toYtwo, fromZ, toZ));


	/*===============BOT===============*/
	//Bot Back Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, -fromYone, -toYone, -fromZ, -toZ));

	//Bot Front Left quadrent
	stripOne.push_back(generateRandomPoint(-fromX, -toX, -fromYone, -toYone, fromZ, toZ));

	//Bot Back Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, -fromYone, -toYone, -fromZ, -toZ));

	//Bot Front Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, -fromYone, -toYone, fromZ, toZ));

	/*=================RIGHT================*/
	//Bot Middle Back Right
	stripOne.push_back(generateRandomPoint(fromX, toX, -fromYtwo, -toYtwo, -fromZ, -toZ));

	//Bot Middle Front Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, -fromYtwo, -toYtwo, fromZ, toZ));

	//Top Middle Back Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, fromYtwo, toYtwo, -fromZ, -toZ));

	//Top Middle Front Right quadrent
	stripOne.push_back(generateRandomPoint(fromX, toX, fromYtwo, toYtwo, fromZ, toZ));



}

void Asteroid::asteroidDraw()
{
	/*===========MAIN SHAPE==============*/
	int numberOf = stripOne.size();
	numberOf -= 1;

	glBegin(GL_QUAD_STRIP);
	
	//Calculates normals for all verticies.
	if (bNorm == true)
	{
		//Top corners normals
		calcCornerNorm(0, 1, 2, 14);
		calcCornerNorm(1, 3, 0, 15);
		calcCornerNorm(2, 0, 3, 4);
		calcCornerNorm(3, 1, 2, 5);

		//Left side normals
		calcMainNorm(4, 2, 5, 6, 14);
		calcMainNorm(5, 4, 3, 15, 7);
		calcMainNorm(6, 4, 7, 8, 12);
		calcMainNorm(7, 5, 6, 9, 13);

		//Bottom corners normals
		calcCornerNorm(8, 6, 10, 9);
		calcCornerNorm(9, 8, 7, 11);
		calcCornerNorm(10, 12, 11, 8);
		calcCornerNorm(11, 10, 9, 13);

		//Right side normals
		calcMainNorm(12, 10, 13, 14, 6);
		calcMainNorm(13, 15, 7, 11, 12);
		calcMainNorm(14, 12, 4, 0, 15);
		calcMainNorm(15, 14, 1, 5, 13);

		bNorm = false;
	}

	//Draws the overall strip with normals appended.
	for (int iterator = 0; iterator <= numberOf; iterator++)
	{
		glNormal3f(normals[iterator][0], normals[iterator][1], normals[iterator][2]);
		glVertex3f(stripOne[iterator][0], stripOne[iterator][1], stripOne[iterator][2]);
	}
	

	glEnd();

	/*==============LAST QUAD====================*/
	glBegin(GL_QUADS);
	//StartVerts
	glNormal3f(normals[0][0], normals[0][1], normals[0][2]);
	glVertex3f(stripOne[0][0], stripOne[0][1], stripOne[0][2]);
	glNormal3f(normals[1][0], normals[1][1], normals[1][2]);
	glVertex3f(stripOne[1][0], stripOne[1][1], stripOne[1][2]);
	//EndVerts
	glNormal3f(normals[numberOf][0], normals[numberOf][1], normals[numberOf][2]);
	glVertex3f(stripOne[numberOf][0], stripOne[numberOf][1], stripOne[numberOf][2]);
	glNormal3f(normals[numberOf - 1][0], normals[numberOf - 1][1], normals[numberOf - 1][2]);
	glVertex3f(stripOne[numberOf - 1][0], stripOne[numberOf - 1][1], stripOne[numberOf - 1][2]);
	glEnd();


	/*=================CAPS===================*/
	/*Creates the two caps via triangle strips*/
	//FRONT
	glBegin(GL_TRIANGLE_STRIP);
	
	glNormal3f(normals[1][0], normals[1][1], normals[1][2]);
	glVertex3f(stripOne[1][0], stripOne[1][1], stripOne[1][2]);
	glNormal3f(normals[3][0], normals[3][1], normals[3][2]);
	glVertex3f(stripOne[3][0], stripOne[3][1], stripOne[3][2]);
	glNormal3f(normals[15][0], normals[15][1], normals[15][2]);
	glVertex3f(stripOne[15][0], stripOne[15][1], stripOne[15][2]);
	glNormal3f(normals[5][0], normals[5][1], normals[5][2]);
	glVertex3f(stripOne[5][0], stripOne[5][1], stripOne[5][2]);
	glNormal3f(normals[13][0], normals[13][1], normals[13][2]);
	glVertex3f(stripOne[13][0], stripOne[13][1], stripOne[13][2]);
	glNormal3f(normals[7][0], normals[7][1], normals[7][2]);
	glVertex3f(stripOne[7][0], stripOne[7][1], stripOne[7][2]);
	glNormal3f(normals[1][0], normals[1][1], normals[1][2]);
	glVertex3f(stripOne[11][0], stripOne[11][1], stripOne[11][2]);
	glNormal3f(normals[9][0], normals[9][1], normals[9][2]);
	glVertex3f(stripOne[9][0], stripOne[9][1], stripOne[9][2]);
	glEnd();

	//BACK
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(normals[0][0], normals[0][1], normals[0][2]);
	glVertex3f(stripOne[0][0], stripOne[0][1], stripOne[0][2]);
	glNormal3f(normals[2][0], normals[2][1], normals[2][2]);
	glVertex3f(stripOne[2][0], stripOne[2][1], stripOne[2][2]);
	glNormal3f(normals[14][0], normals[14][1], normals[14][2]);
	glVertex3f(stripOne[14][0], stripOne[14][1], stripOne[14][2]);
	glNormal3f(normals[4][0], normals[4][1], normals[4][2]);
	glVertex3f(stripOne[4][0], stripOne[4][1], stripOne[4][2]);
	glNormal3f(normals[12][0], normals[12][1], normals[12][2]);
	glVertex3f(stripOne[12][0], stripOne[12][1], stripOne[12][2]);
	glNormal3f(normals[6][0], normals[6][1], normals[6][2]);
	glVertex3f(stripOne[6][0], stripOne[6][1], stripOne[6][2]);
	glNormal3f(normals[10][0], normals[10][1], normals[10][2]);
	glVertex3f(stripOne[10][0], stripOne[10][1], stripOne[10][2]);
	glNormal3f(normals[8][0], normals[8][1], normals[8][2]);
	glVertex3f(stripOne[8][0], stripOne[8][1], stripOne[8][2]);
	glEnd();
}

/*
This method is called in the draw loop.
It calls asteroidDraw() which is treated like a solid object. 
All other methods and functions are the same as a the Celestial::createPlanet() method.
*/
void Asteroid::createAsteroid(GLdouble spinSpeed)
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);

	glRotated((GLdouble)direction, 0.0, 1.0, 0.0); //Rotates to a random degree
	glRotated((GLdouble)spinSpeed, 0.0, 1.0, 0.0);
	
	glTranslated(0.0, ofset, distance);
	asteroidDraw();

	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}
/*
The main difference between the createAsteroid and createCommet methods is introduction
of an eliptical orbit via the introduction of a main and secondary radious representing 
an elipse.
*/
void Asteroid::createCommet(GLdouble spinSpeed, GLdouble radOne, GLdouble radTwo)
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor3f(red, green, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, gloss);

	//Calculates the position based on the speed and the main and secondary radious.
	Xoffset = radOne*cos(spinSpeed);
	Zoffset = radTwo*sin(spinSpeed);

	//Translates the initial position of the asteroid to the same distance as pluto.
	glTranslated(Xoffset, 0.0, 99500 + Zoffset);
	asteroidDraw();

	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}