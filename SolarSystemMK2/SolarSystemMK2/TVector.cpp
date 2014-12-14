#include "TVector.h"

//Constructors
TVector::TVector()
{
	XYZ[0] = 0;
	XYZ[1] = 0;
	XYZ[2] = 0;

	magnitude = 0;
}
TVector::TVector(double x, double y, double z) 
{
	XYZ[0] = x;
	XYZ[1] = y;
	XYZ[2] = z;

	magnitude = 0;
}
TVector::TVector(double* xyz)
{
	XYZ[0] = *xyz;
	XYZ[1] = *xyz+1;
	XYZ[2] = *xyz+2;

	magnitude = 0;
}

//Destructor
TVector::~TVector()
{
}

//Getters Setters
void TVector::setVec(double x, double y, double z)
{
	XYZ[0] = x;
	XYZ[1] = y;
	XYZ[2] = z;
}
void TVector::setVec(double* xyz)
{
	XYZ[0] = *xyz;
	XYZ[1] = *xyz + 1;
	XYZ[2] = *xyz + 2;
}

double* TVector::getVec()
{
	return XYZ;
}
double  TVector::getMagnitude()
{
	if (magnitude == 0 && (XYZ[0] != 0 || XYZ[1] != 0 || XYZ[2] != 0))
	{
		calcMagnitude();
	}
	return magnitude;
}
double* TVector::getUnit()
{
	if (XYZ[0] == 0 && XYZ[1] == 0 && XYZ[2] == 0)
	{
		unit[0] = 0;
		unit[1] = 0;
		unit[2] = 0;
	}
	else
	{
		calcUnit();
	}
	return unit;
}
double* TVector::getInvertedVec()
{
	XYZinvert[0] = -XYZ[0];
	XYZinvert[1] = -XYZ[1];
	XYZinvert[2] = -XYZ[2];
	return XYZinvert;
}

//Private Functions
void TVector::calcMagnitude()
{
	magnitude = sqrt(pow(XYZ[0], 2) + pow(XYZ[1], 2) + pow(XYZ[2], 2));
}
void TVector::calcUnit()
{
	if (magnitude == 0 && (XYZ[0] != 0 || XYZ[1] != 0 || XYZ[2] != 0))
		magnitude = sqrt(pow(XYZ[0], 2) + pow(XYZ[1] + 1, 2) + pow(XYZ[2] + 2, 2));

	//Normalize
	for (int i = 0; i <= 2; i++)
	{
		unit[i] = XYZ[i] / magnitude;
	}
}

//TVector FRIEND FUNCTIONS
double dotPrdct(TVector& first, TVector& second)
{
	return first.XYZ[0] * second.XYZ[0] + first.XYZ[1] * second.XYZ[1] + first.XYZ[2] * second.XYZ[2];
}
TVector crossPrdct(TVector& first, TVector& second)
{
	return TVector((first.XYZ[1] * second.XYZ[2] - first.XYZ[2] * second.XYZ[1]), (first.XYZ[2] * second.XYZ[1] - first.XYZ[1] * second.XYZ[2]), (first.XYZ[0] * second.XYZ[1] - first.XYZ[1] * second.XYZ[0]));
}

//Operator Overloads
TVector operator+(const TVector& first, const TVector& second)
{
	return TVector(first.XYZ[0] + second.XYZ[0], first.XYZ[1] + second.XYZ[1], first.XYZ[2] + second.XYZ[2]);
}

TVector operator-(const TVector& first, const TVector& second)
{
	return TVector(first.XYZ[0] - second.XYZ[0], first.XYZ[1] - second.XYZ[1], first.XYZ[2] - second.XYZ[2]);
}