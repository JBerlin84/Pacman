#include "Advanced2D.h"

#ifndef CAMERA_H_
#define CAMERA_H_

namespace Advanced2D {

	class Camera {
	private:
		D3DXMATRIX matrixProj;
		D3DXMATRIX matrixView;
		D3DXVECTOR3 updir;

		D3DXVECTOR3 position;
		D3DXVECTOR3 target;

		float nearRange;
		float farRange;
		float aspectRatio;
		float fov;

	public:
		Camera(void);
		~Camera(void);

		void setPerspective(float fov, float aspectRatio, float nearRange, float farRange);
		float getNearRange() { return nearRange; }
		void setNearRange(float value) { nearRange = value; }
		float getFarRange() { return farRange; }
		void setFarRange(float value) { farRange = value; }
		float getAspectRatio() { return aspectRatio; }
		void setAspectRatio(float value) { aspectRatio = value; }
		float getFOV() { return fov; }
		void setFOV(float value) { fov = value; }
		void Update();

		D3DXVECTOR3 getPosition() { return position; }
		void setPosition(float x, float y, float z);
		void setPosition(D3DXVECTOR3 position);
		float getX() { return position.x; }
		void setX(float value) { position.x = value; }
		float getY() { return position.y; }
		void setY(float value) { position.y = value; }
		float getZ() { return position.z; }
		void setZ(float value) { position.z = value; }

		D3DXVECTOR3 getTarget() { return target; }
		void setTarget(D3DXVECTOR3 value) { target = value; }
		void setTarget(float x, float y, float z) { target.x = x; target.y = y; target.z = z; }
	};
}

#endif
