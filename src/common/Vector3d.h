#pragma once

#include <iostream>
#include "irrlicht/irrlicht.h"

class Vector3d
{
public:
	//Atributos
	float x, y, z;

	//Constructores
	Vector3d();
	~Vector3d(void);
	Vector3d(float x, float y, float z);    
	Vector3d(const Vector3d &other);
	Vector3d(float n);

	//Sobrecarga de operadores 
	Vector3d operator-() const;
	Vector3d& operator=(const Vector3d &other);
	bool operator!=(const Vector3d& other) const;
	bool operator==(const Vector3d& other) const;

	Vector3d operator+(const Vector3d& other) const;
	Vector3d& operator+=(const Vector3d& other);
	Vector3d operator+(const float val) const;
	Vector3d& operator+=(const float val);

	Vector3d operator-(const Vector3d &other) const;
	Vector3d& operator-=(const Vector3d& other);
	Vector3d operator-(const float val) const;
	Vector3d& operator-=(const float val);

	Vector3d operator*(const Vector3d& other) const;
	Vector3d& operator*=(const Vector3d& other); 
	Vector3d operator*(const float v) const;
	Vector3d& operator*=(const float v);

	Vector3d operator/(const Vector3d& other) const;
	Vector3d& operator/=(const Vector3d& other);
	Vector3d operator/(const float v) const;
	Vector3d& operator/=(const float v);

	//Vector3d& Vector3d::operator /=(float val) {x/=val;y/=val;z/=val; return *this;}
	bool operator<(const Vector3d&other) const;
	bool operator<=(const Vector3d&other) const;
	bool operator>(const Vector3d&other) const;
	bool operator>=(const Vector3d&other) const;
	operator irr::core::vector3df() const;

	friend std::ostream& operator << (std::ostream &o,const Vector3d &v);

	//Metodos
	Vector3d Vector3d::crossProduct(const Vector3d& p) const;
	float Vector3d::dotProduct(const Vector3d& other) const;
	//bool equals(const Vector3d& other, const float tolerance = 0.000001 ) const;
	bool equals(const Vector3d& other ) const;
	void getAs3Values(float* array) const;
	void getAs4Values(float* array) const;
	float getLength() const;
	float getLengthSQ() const;
	float getDistanceFrom(const Vector3d& other) const;
	float getDistanceFromSQ(const Vector3d& other) const;
	Vector3d getHorizontalAngle() const;
	Vector3d getInterpolated(const Vector3d& other, float d) const;
	Vector3d getInterpolated_quadratic(const Vector3d& v2, const Vector3d& v3, float d) const;
	Vector3d getSphericalCoordinateAngles() const;
	Vector3d& interpolate(const Vector3d& a, const Vector3d& b, float d);
	Vector3d& invert();
	bool isBetweenPoints(const Vector3d& begin, const Vector3d& end) const;
	Vector3d& normalize();
	void rotateXYBy(float degrees, const Vector3d& center=Vector3d());
	void rotateXZBy(float degrees, const Vector3d& center=Vector3d());
	void rotateYZBy(float degrees, const Vector3d& center=Vector3d());
	Vector3d rotationToDirection(const Vector3d & forwards = Vector3d(0, 0, 1)) const;

	Vector3d& set(const float nx, const float ny, const float nz);
	Vector3d& set(const Vector3d& p);
	Vector3d& setLength(float newlength);

};
