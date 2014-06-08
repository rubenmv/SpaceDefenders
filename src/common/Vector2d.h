#pragma once

#include <iostream>
#include "irrlicht/irrlicht.h"
#include "Math.h"
#include "Vector3d.h"


class Vector2d
{
public:

	float x, y; 
	Vector2d() { this->x = 0; this->y = 0; };
	Vector2d(float x, float y) { this->x = x; this->y = y; };
	
	//----------------------------------------------------------
	//Sobrecarga de operadores
	inline Vector2d operator-(const Vector2d &right)
	{
		return Vector2d(this->x - right.x, this->y - right.y);
	}

	inline Vector2d& operator +=(const Vector2d& right)
	{
		x += right.x;
		y += right.y;

		return *this;
		//return *(new Vector2d(this->x + right.x, this->y + right.y));
	}

	inline Vector2d& operator -=(const Vector2d& right)
	{
		x -= right.x;
		y -= right.y;

		return *this;
	}

	inline Vector2d operator +(const Vector2d& right)
	{
		return Vector2d(this->x + right.x, this->y + right.y);
	}

	inline Vector2d operator *(float right)
	{
		return Vector2d(this->x * right, this->y * right);
	}

	inline Vector2d& operator *=(float right)
	{
		x *= right;
		y *= right;

		return *this;
	}

	inline Vector2d operator /(float right)
	{
		return Vector2d(this->x / right, this->y / right);
	}

	inline Vector2d& operator /=( float right)
	{
		x /= right;
		y /= right;

		return *this;
	}

	inline bool operator ==(const Vector2d& right)
	{
		return (x == right.x && y == right.y);
	}

	inline bool operator !=(const Vector2d& right)
	{
		return !(x == right.x && y == right.y);
	}
	
	

	inline friend std::ostream& operator <<(std::ostream &o,const Vector2d &v)
	{
		o << "x:" << Math::round(v.x, 2) << ", y:" <<Math::round(v.y, 2);
		return o;
	}

	inline friend std::wostream& operator <<(std::wostream &o,const Vector2d &v)
	{
		o << L"x:" << Math::round(v.x, 2) << L", y:" <<Math::round(v.y, 2);
		return o;
	}

	/*
	Vector2d operator-(const Vector2d &right);
	Vector2d& operator +=( const Vector2d& right);
	Vector2d& operator -=( const Vector2d& right);
	Vector2d operator +( const Vector2d& right);
	Vector2d operator *( float right);
	Vector2d& operator *=( float right);
	Vector2d operator /( float right);
	Vector2d& operator /=( float right);
	friend std::ostream& operator << (std::ostream &o,const Vector2d &v); //Sobrecarga del operador << (por si queremos imprimir un vecot en pantalla mas facilmente)
	friend std::wostream& operator << (std::wostream &o,const Vector2d &v); //Sobrecarga del operador << (por si queremos imprimir un vecot en pantalla mas facilmente)
	*/

	void truncate(float max) 
	{
		if (this->getLength() > max)
		{
			this->normalize();

			this->operator*=(max/getLength());
		}
	}

	//Convierte un vector2d al vector3d de irrlicht en el formato Vector3df(x, 0, y)
	operator irr::core::vector3df() 
	{
		return(irr::core::vector3df(x, 0, y));
	}

	operator irr::core::vector2di() 
	{
		return(irr::core::vector2di(x,y));
	}
	Vector3d asVector3d()
	{
		return Vector3d(x,0,y);
	}

	//----------------------------------------------------------
	//Metodos
	static Vector2d getVector2dByAngle(float angle)
	{
		return Vector2d(Math::cos(angle * Math::DEGTORAD), Math::sin(angle * Math::DEGTORAD));
	}

	Vector2d& set(float nx, float ny) {x=nx; y=ny; return *this; }

	inline float getAngle() const
	{
		//double pi=3.1415926535897932384626433832795028841971693993751;  //Si lo voy a usar en casi todos los metodos mejor los defino como constantes
		//double Math::RADTODEG=180.0/pi;
		if (y == 0) // corrected thanks to a suggestion by Jox
			return x < 0 ? 180.0 : 0.0;
		else if (x == 0)
			return y < 0 ? 90.0 : 270.0;
 
			 // don't use getLength here to avoid precision loss with s32 vectors
			 // avoid floating-point trouble as sqrt(y*y) is occasionally larger than y, so clamp
		//const float tmp = irr::core::clamp(y / sqrt((float)(x*x + y*y)), -1.0, 1.0);
		float tmp;  //=this.clamp( y / sqrt((float)(x*x + y*y)), -1.0, 1.0);
		if(y / sqrt((float)(x*x + y*y))>-1.0) //maximo
			tmp=y / sqrt((float)(x*x + y*y));
		else
			tmp=-1.0;

		if(tmp>1.0) //minimo
			tmp=1.0;
	

		const float angle = atan( sqrt(1 - tmp*tmp) / tmp) * Math::RADTODEG;
 
		if (x>0 && y>0)
			return angle + 270;
		else
		if (x>0 && y<0)
			return angle + 90;
		else
		if (x<0 && y<0)
			return 90 - angle;
		else
			return 270 - angle;
 
		 return angle;
	}// /getAngle


	inline float getAngleWith(const Vector2d &b) const
	{
		float tmp = (float)(x*b.x + y*b.y);
 
		if (tmp == 0.0)
			return 90.0;
 
		tmp = tmp / sqrt((float)((x*x + y*y) * (b.x*b.x + b.y*b.y)));
		if (tmp < 0.0)
			tmp = -tmp;
		if ( tmp > 1.0 ) //   avoid floating-point trouble
			tmp = 1.0;
 
		return atan(sqrt(1 - tmp*tmp) / tmp) * Math::RADTODEG;
	}// /getAngleWith

	
	inline bool equals(const Vector2d &other) 
	{	
		if(x==other.x && y==other.y)
			return true;
		else
			return false;
	}

	inline float getAngleTrig() const
	{

		if (y == 0)
			return x < 0 ? 180 : 0;
		else
			if (x == 0)
				return y < 0 ? 270 : 90;

		if ( y > 0){
			if (x > 0){            
				return atan(y/x) * Math::RADTODEG; 
			}
			else{            
				return 180.0-atan(y/-x) * Math::RADTODEG; 
			}
		}
		else{
			if (x > 0){
				return 360.0-atan(-y/x) * Math::RADTODEG; 
			}
			else{
				return 180.0+atan(-y/-x) * Math::RADTODEG; 
			}
		}
	}


	inline float getLength() const
	{ 
		return sqrt( x*x + y*y ); 
	}
	//Devuelve la longitud sin hacer la raiz cuadrada
	inline float getSqrLength() const
	{ 
		return ( x*x + y*y ); 
	}

	// Devuelve la distancia con respecto a otro vector
	inline float getDistanceFrom(const Vector2d &other) const
	{
		return Vector2d(x - other.x, y - other.y).getLength();
	}

	// Devuelve la distancia con respecto a otro vector
	inline float getSqrDistanceFrom(const Vector2d &other) const
	{
		return Vector2d(x - other.x, y - other.y).getSqrLength();
	}

	//Returns if this vector interpreted as a point is on a line between two other points.    It is assumed that the point is on the line. 

	inline bool isBetweenPoints(const Vector2d &begin, const Vector2d &end) const 
	{
		if (begin.x != end.x)
		{
			return ((begin.x <= x && x <= end.x) ||
				(begin.x >= x && x >= end.x));
		}
		else
		{
			return ((begin.y <= y && y <= end.y) ||
				(begin.y >= y && y >= end.y));
		}
	}


	inline Vector2d& rotateBy(float degrees, const Vector2d &center)
	{
		degrees *= Math::DEGTORAD;
		const float cs = Math::cos(degrees);
		const float sn = Math::sin(degrees);

		x -= center.x;
		y -= center.y;

		set((x*cs - y*sn), (x*sn + y*cs));

		x += center.x;
		y += center.y;
		return *this;
	}

	inline Vector2d& normalize()
	{
		float length = this->getLength();
		if(length == 0) return *this;
		x /= length;
		y /= length;

		return *this;
	}

	inline Vector2d& setLength(float factor)
	{
		normalize();
		x *= factor;
		y *= factor;
		//return (*this *= factor);
		return *this;
	}

	float dot(const Vector2d &v)
	{


		return x*v.x + y*v.y;
	}


	static Vector2d Vector2d::getNormalPoint( Vector2d position, Vector2d start, Vector2d end )
	{

		Vector2d startPosition = position - start;
		Vector2d startEnd = end - start;

		startEnd.normalize();
		startEnd *= startPosition.dot(startEnd);


		Vector2d normalPoint = start + startEnd;

		return normalPoint;
	}

	static Vector2d lerp(Vector2d init, Vector2d end, float factor)
	{
		if(factor > 1) factor = 1;
		else if(factor < 0) factor = 0;

		Vector2d direction = end - init;
		direction.normalize();
		float distance = end.getDistanceFrom(init);

		direction *= distance * factor;

		return init+direction;
	}
};

