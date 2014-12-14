#include "CameraHelper.h"


CameraHelper::CameraHelper()
{
	cameraLR = 0.0;
	cameraUD = 30000.0;
	cameraFB = 30000.0;
	cameraLookX = 0;
	cameraLookY = 0;
	cameraLookZ = 0;
}


CameraHelper::~CameraHelper()
{
}

void CameraHelper::setCameraLookX(int x)
{
	cameraLookX = x;
}
void CameraHelper::setCameraLookZ(int z)
{
	cameraLookZ = z;
}
void CameraHelper::setLR(int lr)
{
	cameraLR = lr;
}
void CameraHelper::setUD(int ud)
{
	cameraUD = ud;
}
void CameraHelper::setFB(int fb)
{
	cameraFB = fb;
}

