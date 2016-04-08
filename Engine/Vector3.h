/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Vector class for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#include "GameEngine.h"

#ifndef VECTOR3_H_
#define VECTOR3_H_

namespace GameEngine {
	class Vector3 {
	private:
		double x, y, z;
	
	public:
		Vector3();
		Vector3(const Vector3& v);
		Vector3(double x, double y, double z);
		Vector3(int x, int y, int z);
		void Set(double x1,double y1,double z1);
		void Set(const Vector3& v);
		double getX() { return x; }
		void setX(double v) { x = v; }
		double getY() { return y; }
		void setY(double v) { y = v; }
		double getZ() { return z; }
		void setZ(double v) { z = v; }
		void Move( double mx,double my,double mz);
		void operator+=(const Vector3& v);
		void operator-=(const Vector3& v);
		void operator*=(const Vector3& v);
		void operator/=(const Vector3& v);
		bool operator==( const Vector3& v ) const;
		bool operator!=( const Vector3& p ) const;
		Vector3& operator=( const Vector3& v);
		double Distance( const Vector3& v );
		double Length();
		double DotProduct( const Vector3& v );
		Vector3 CrossProduct( const Vector3& v );
		Vector3 Normal();
	};
}

#endif