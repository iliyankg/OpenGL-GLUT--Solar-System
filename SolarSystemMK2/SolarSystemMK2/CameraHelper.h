#pragma once
#include <glut.h>
class CameraHelper
{
public:
	//Camera variables
	int cameraLR;
	int cameraUD;
	int cameraFB;
	int cameraLookX;
	int cameraLookY;
	int cameraLookZ;

	CameraHelper();
	~CameraHelper();

	//Setters
	void setCameraLookX(int x);
	void setCameraLookZ(int y);

	void setLR(int lr);
	void setUD(int ud);
	void setFB(int fb);
};

