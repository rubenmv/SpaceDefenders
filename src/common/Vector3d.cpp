#include "Vector3d.h"
#include "Math.h"


Vector3d::Vector3d()
{
	x=0;
	y=0;
	z=0;
}


Vector3d::~Vector3d(void)
{
}

Vector3d::Vector3d(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;	
}

Vector3d::Vector3d(const Vector3d &other)
{
	this->x=other.x;
	this->y=other.y;
	this->z=other.z;
}

Vector3d::Vector3d(float n)
{
	x=n;
	y=n;
	z=n;
}

//Operadores
Vector3d Vector3d::operator-() const 
{ 
	return Vector3d(-x, -y, -z); 
} 

Vector3d& Vector3d::operator=(const Vector3d &other) 
{ 	
	this->x = other.x; 
	this->y = other.y; 
	this->z = other.z; 
	return *this; 
} 

Vector3d Vector3d::operator+(const Vector3d& other) const 
{ 
	return Vector3d(this->x + other.x, this->y + other.y, this->z + other.z); 
}		 		 		 

Vector3d& Vector3d::operator+=(const Vector3d& other) 
{ 
	this->x+=other.x; 
	this->y+=other.y; 
	this->z+=other.z; 
	return *this; 
}

Vector3d Vector3d::operator+(const float val) const 
{ 
	return Vector3d(this->x + val, this->y + val, this->z + val); 
}

Vector3d& Vector3d::operator+=(const float val) 
{ 
	x+=val; 
	y+=val; 
	z+=val; 
	return *this; 
} 

Vector3d Vector3d::operator-(const Vector3d &other) const 
{ 
	return Vector3d(x - other.x, y - other.y, z - other.z); 
}

Vector3d& Vector3d::operator-=(const Vector3d& other) 
{ 
	x-=other.x; 
	y-=other.y; 
	z-=other.z; 
	return *this; 
}

Vector3d Vector3d::operator-(const float val) const 
{ 
	return Vector3d(x - val, y - val, z - val); 
}

Vector3d& Vector3d::operator-=(const float val)
{ 
	x-=val; 
	y-=val; 
	z-=val; 
	return *this; 
} 

Vector3d Vector3d::operator*(const Vector3d& other) const 
{ 
	return Vector3d(x * other.x, y * other.y, z * other.z); 
}		 

Vector3d& Vector3d::operator*=(const Vector3d& other) 
{ 
	x*=other.x; 
	y*=other.y; 
	z*=other.z; 
	return *this; 
}		 

Vector3d Vector3d::operator*(const float v) const 
{ 
	return Vector3d(x * v, y * v, z * v); 
}

Vector3d& Vector3d::operator*=(const float v) 
{ 
	x*=v; y*=v; z*=v; return *this; 
}

Vector3d Vector3d::operator/(const Vector3d& other) const 
{
	return Vector3d(x / other.x, y / other.y, z / other.z); 
}

Vector3d& Vector3d::operator/=(const Vector3d& other) 
{ 
	x/=other.x; 
	y/=other.y; 
	z/=other.z; 
	return *this; 
}

Vector3d Vector3d::operator/(const float v) const 
{ 
	float i=(float)1.0/v; 
	return Vector3d(x * i, y * i, z * i); 
}

Vector3d& Vector3d::operator/=(const float v) 
{ 
	float i=(float)1.0/v; 
	x*=i; 
	y*=i; 
	z*=i; 
	return *this; 
}

bool Vector3d::operator!=(const Vector3d& other) const
{
	return !this->equals(other);
}

bool Vector3d::operator==(const Vector3d& other) const
{
	return this->equals(other);
}

bool Vector3d::operator<(const Vector3d&other) const
{
	return  (x<other.x && !Math::equals(x, other.x)) ||
			(Math::equals(x, other.x) && y<other.y && !Math::equals(y, other.y)) ||
			(Math::equals(x, other.x) && Math::equals(y, other.y) && z<other.z && !Math::equals(z, other.z));
}

bool Vector3d::operator<=(const Vector3d&other) const
{
	return  (x<other.x || Math::equals(x, other.x)) ||
			(Math::equals(x, other.x) && (y<other.y || Math::equals(y, other.y))) ||
			(Math::equals(x, other.x) && Math::equals(y, other.y) && (z<other.z || Math::equals(z, other.z)));
}

bool Vector3d::operator>(const Vector3d&other) const
{
	return  (x>other.x && !Math::equals(x, other.x)) ||
			(Math::equals(x, other.x) && y>other.y && !Math::equals(y, other.y)) ||
			(Math::equals(x, other.x) && Math::equals(y, other.y) && z>other.z && !Math::equals(z, other.z));
}

bool Vector3d::operator>=(const Vector3d&other) const
{
	return  (x>other.x || Math::equals(x, other.x)) ||
			(Math::equals(x, other.x) && (y>other.y || Math::equals(y, other.y))) ||
			(Math::equals(x, other.x) && Math::equals(y, other.y) && (z>other.z || Math::equals(z, other.z)));
}

Vector3d::operator irr::core::vector3df() const
{
	return(irr::core::vector3df(x,y,z));
}

std::ostream& operator << (std::ostream &o,const Vector3d &v)
{
	o << "(" << v.x << ", " << v.y <<", "<< v.z <<")";
	return o;
}

//Metodos
//Calculates the cross product with another vector. 
Vector3d Vector3d::crossProduct(const Vector3d& p) const
{
	return Vector3d(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

//Get the dot product with another vector. 
float Vector3d::dotProduct(const Vector3d& other) const
{
	return x*other.x + y*other.y + z*other.z;
}

//returns if this vector equals the other one, taking floating point rounding errors into account 
bool Vector3d::equals(const Vector3d& other ) const
{
	//return irr::core::equals(x, other.x, tolerance) &&
	//irr::core::equals(y, other.y, tolerance) &&
	//irr::core::equals(z, other.z, tolerance);
	
	//return Math::equals(x, other.x, tolerance) &&
	//Math::equals(y, other.y, tolerance) &&
	//Math::equals(z, other.z, tolerance);

	return Math::equals(x, other.x) &&
	Math::equals(y, other.y) &&
	Math::equals(z, other.z);
}

//Fills an array of 3 values with the vector data (usually floats).  Useful for setting in shader constants for example. 
void Vector3d::getAs3Values(float* array) const
{
	array[0] = x;
	array[1] = y;
	array[2] = z;
}

//Fills an array of 4 values with the vector data (usually floats). Useful for setting in shader constants for example. The fourth value will always be 0. 
void Vector3d::getAs4Values(float* array) const
{
	array[0] = x;
	array[1] = y;
	array[2] = z;
	array[3] = 0.0;
}

float Vector3d::getLength() const 
{ 
	return Math::sqrt( x*x + y*y + z*z ); 
}

float Vector3d::getLengthSQ() const 
{ 
	return x*x + y*y + z*z; 
}

//Get distance from another point. Here, the vector is interpreted as point in 3 dimensional space. 
float Vector3d::getDistanceFrom(const Vector3d& other) const
{
	return Vector3d(x - other.x, y - other.y, z - other.z).getLength();
}

//Returns squared distance from another point. Here, the vector is interpreted as point in 3 dimensional space. 
float Vector3d::getDistanceFromSQ(const Vector3d& other) const
{
	return Vector3d(x - other.x, y - other.y, z - other.z).getLengthSQ();
}

//Get the rotations that would make a (0,0,1) direction vector point in the same direction as this direction vector. 
//RETURNS A rotation vector containing the X (pitch) and Y (raw) rotations (in degrees) that when applied to a +Z (e.g. 0, 0, 1) direction vector would make it point in the same direction as this vector. The Z (roll) rotation is always 0, since two Euler rotations are sufficient to point in any given direction. 
Vector3d Vector3d::getHorizontalAngle() const
{
	Vector3d angle;

	const float tmp = (Math::atan2(x, z) * Math::RADTODEG);
	angle.y = (float) tmp;

	if (angle.y < 0)
		angle.y += 360;
	if (angle.y >= 360)
		angle.y -= 360;

	const float z1 = Math::sqrt(x*x + z*z);

	angle.x = (float) (Math::atan2(z1, y) * Math::RADTODEG - 90.0);

	if (angle.x < 0)
		angle.x += 360;
	if (angle.x >= 360)
		angle.x -= 360;

	return angle;
}

//Creates an interpolated vector between this vector and another vector. //d = nterpolation value between 0.0f (all the other vector) and 1.0f (all this vector). Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
Vector3d Vector3d::getInterpolated(const Vector3d& other, float d) const
{
	const float inv = 1.0 - d;
	return Vector3d((float)(other.x*inv + x*d), (float)(other.y*inv + y*d), (float)(other.z*inv + z*d));
}

//Creates a quadratically interpolated vector between this and two other vectors. 
Vector3d Vector3d::getInterpolated_quadratic(const Vector3d& v2, const Vector3d& v3, float d) const
{
	// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
	const float inv = (float) 1.0 - d;
	const float mul0 = inv * inv;
	const float mul1 = (float) 2.0 * d * inv;
	const float mul2 = d * d;

	return Vector3d ((float)(x * mul0 + v2.x * mul1 + v3.x * mul2),
			(float)(y * mul0 + v2.y * mul1 + v3.y * mul2),
			(float)(z * mul0 + v2.z * mul1 + v3.z * mul2));
}

//Get the spherical coordinate angles. This returns Euler degrees for the point represented by this vector. The calculation assumes the pole at (0,1,0) and returns the angles in X and Y. 
Vector3d Vector3d::getSphericalCoordinateAngles() const
{
	Vector3d angle;
	const float length = x*x + y*y + z*z;
 
	if (length)
	{
		if (x!=0)
		{
			angle.y = (float)(Math::atan2((float)z,(float)x) * Math::RADTODEG);
		}
		else if (z<0)
			angle.y=180;
 
		angle.x = (float)(Math::acos(y / Math::sqrt(length)) * Math::RADTODEG);
	}
	return angle;
}

//Sets this vector to the linearly interpolated vector between a and b. 
Vector3d& Vector3d::interpolate(const Vector3d& a, const Vector3d& b, float d)
{
	x = (float)((float)b.x + ( ( a.x - b.x ) * d ));
	y = (float)((float)b.y + ( ( a.y - b.y ) * d ));
	z = (float)((float)b.z + ( ( a.z - b.z ) * d ));
	return *this;
}

//Inverts the vector. 
Vector3d& Vector3d::invert()
{
	x *= -1;
	y *= -1;
	z *= -1;
	return *this;
}

//Returns if this vector interpreted as a point is on a line between two other points. 
bool Vector3d::isBetweenPoints(const Vector3d& begin, const Vector3d& end) const
{
	const float f = (end - begin).getLengthSQ();
	return getDistanceFromSQ(begin) <= f &&
		getDistanceFromSQ(end) <= f;
}

//Normalizes the vector. In case of the 0 vector the result is still 0, otherwise the length of the vector will be 1. 
Vector3d& Vector3d::normalize()
{
	float length = x*x + y*y + z*z;
	if (length == 0 ) // this check isn't an optimization but prevents getting NAN in the sqrt.
		return *this;
	length = 1/Math::sqrt(length);
 
	x = (float)(x * length);
	y = (float)(y * length);
	z = (float)(z * length);
	return *this;
}


//Rotates the vector by a specified number of degrees around the Z axis and the specified center. 
void Vector3d::rotateXYBy(float degrees, const Vector3d& center)
{
	//center=Vector3d();
	degrees *= Math::DEGTORAD;
	float cs = cos(degrees);
	float sn = sin(degrees);
	x -= center.x;
	y -= center.y;
	set((float)(x*cs - y*sn), (float)(x*sn + y*cs), z);
	x += center.x;
	y += center.y;
}

//Rotates the vector by a specified number of degrees around the Y axis and the specified center. 
void Vector3d::rotateXZBy(float degrees, const Vector3d& center)
{
	degrees *= Math::DEGTORAD;
	float cs = cos(degrees);
	float sn = sin(degrees);
	x -= center.x;
	z -= center.z;
	set((float)(x*cs - z*sn), y, (float)(x*sn + z*cs));
	x += center.x;
	z += center.z;
}

//Rotates the vector by a specified number of degrees around the X axis and the specified center. 
void Vector3d::rotateYZBy(float degrees, const Vector3d& center)
{
	degrees *=Math::DEGTORAD;
	float cs = cos(degrees);
	float sn = sin(degrees);
	z -= center.z;
	y -= center.y;
	set(x, (float)(y*cs - z*sn), (float)(y*sn + z*cs));
	z += center.z;
	y += center.y;
}

//Builds a direction vector from (this) rotation vector. This vector is assumed to be a rotation vector composed of 3 Euler angle rotations, in degrees. The implementation performs the same calculations as using a matrix to do the rotation.
Vector3d Vector3d::rotationToDirection(const Vector3d & forwards) const
{
	const float cr = Math::cos( Math::DEGTORAD * x );
	const float sr = Math::sin( Math::DEGTORAD * x );
	const float cp = Math::cos( Math::DEGTORAD * y );
	const float sp = Math::sin( Math::DEGTORAD * y );
	const float cy = Math::cos( Math::DEGTORAD * z );
	const float sy = Math::sin( Math::DEGTORAD * z );

	const float srsp = sr*sp;
	const float crsp = cr*sp;

	const float pseudoMatrix[] = {
		( cp*cy ), ( cp*sy ), ( -sp ),
		( srsp*cy-cr*sy ), ( srsp*sy+cr*cy ), ( sr*cp ),
		( crsp*cy+sr*sy ), ( crsp*sy-sr*cy ), ( cr*cp )};

	return Vector3d(
		(float)(forwards.x * pseudoMatrix[0] +
			forwards.y * pseudoMatrix[3] +
			forwards.z * pseudoMatrix[6]),
		(float)(forwards.x * pseudoMatrix[1] +
			forwards.y * pseudoMatrix[4] +
			forwards.z * pseudoMatrix[7]),
		(float)(forwards.x * pseudoMatrix[2] +
			forwards.y * pseudoMatrix[5] +
			forwards.z * pseudoMatrix[8]));
}

Vector3d& Vector3d::set(const float nx, const float ny, const float nz) 
{
	x=nx; 
	y=ny; 
	z=nz; 
	return *this;
}

Vector3d& Vector3d::set(const Vector3d& p) 
{
	x=p.x;
	y=p.y;
	z=p.z;
	return *this;
}

Vector3d& Vector3d::setLength(float newlength)
{
	normalize();
	return (*this *= newlength);
}