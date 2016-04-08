#include "Advanced2D.h"

namespace Advanced2D {


Camera::Camera(void)
{
	position = D3DXVECTOR3(0.0f,0.0f,10.0f);
	updir = D3DXVECTOR3(0.0f,1.0f,0.0f);

	//hard coded to 1.3333 by default
	float ratio = 640 / 480;
	setPerspective(3.14159f / 4, ratio, 1.0f, 2000.0f);

}

Camera::~Camera(void)
{
}

void Camera::setPerspective(float fov, float aspectRatio, float nearRange, float farRange)
{
	this->setFOV(fov);
	this->setAspectRatio(aspectRatio);
	this->setNearRange(nearRange);
	this->setFarRange(farRange);
}


void Camera::Update()
{
    //set the camera's perspective matrix
    D3DXMatrixPerspectiveFovLH(&this->matrixProj, this->fov, this->aspectRatio, this->nearRange, this->farRange);
    engine->getDevice()->SetTransform(D3DTS_PROJECTION, &this->matrixProj);

    //set the camera's view matrix
    D3DXMatrixLookAtLH(&this->matrixView, &this->position, &this->target, &this->updir);
    engine->getDevice()->SetTransform(D3DTS_VIEW, &this->matrixView);
}

void Camera::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Camera::setPosition(D3DXVECTOR3 position)
{
	this->setPosition(position.x, position.y, position.z);
}

}
