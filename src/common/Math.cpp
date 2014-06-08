
#include "math.h"
#include <cmath>
#include <stdlib.h>

#define KPI 3.14159265358979323846

const float Math::PI = KPI;
const float Math::HALF_PI = KPI / 2;
const float Math::TWO_PI = KPI * 2;
const float Math::DEGTORAD = KPI / 180;
const float Math::RADTODEG = 180 / KPI;
//const float Math::ROUNDING_ERROR_f32 = 0.000001;


float Math::abs(float n)
{
	return (std::abs(n));
}

float Math::max_(float a, float b)
{
	if (a >= b) return a;
	else return b;

}

float Math::min_(float a, float b)
{
	if (a <= b) return a;
	else return b;
}

float Math::constraint(float n, float low, float high)
{
	if (n > low && n < high) {
		return n;
	} else if (n >= high) {
		return high;
	} else {
		return low;
	}
}

float Math::cos(float n)
{
	return std::cos(n);
}

float Math::sin(float n)
{
	return std::sin(n);
}

float Math::tan(float n)
{
	return std::tan(n);
}

float Math::acos(float n)
{
	return std::acos(n);
}

float Math::asin(float n)
{
	return std::asin(n);
}

float Math::atan(float n)
{
	return std::atan(n);
}

float Math::atan2(float y, float x)
{
	return std::atan2(y, x);
}

float Math::pow(float base, float exponent)
{
	return std::pow(base, exponent);
}

float Math::sqrt(float n)
{
	return std::sqrt(n);
}

float Math::floor(float n)
{
	return std::floor(n);
}

float Math::ceil(float n)
{
	return std::ceil(n);
}

float Math::round(float n)
{
	float aux = n - floor(n);

	if (aux < 0.5) {
		return (n - aux);
	} else {
		return (n - aux + 1);
	}
}

float Math::round(float n, float decimals)
{
	float digits = pow(10, decimals);
	n *= digits;
	n = round(n);
	n /= digits;
	return n;
}

float Math::mod(float n, int base)
{
	int rest = n/base;
	return n-rest*base;
}


float Math::warpAngle(float angle)
{
	return (angle + 360 * Math::floor((180 - angle) / 360));
}


int Math::random(int high)
{

	return (rand() % high); //Windows
	//return (random()%high); //Mac
}

int Math::random(int low, int high)
{
	return (low + (rand() % (high - low))); //Windows
	//return (low + (random() % (high-low))); //Mac

}

bool Math::equals(const float a, const float b) //ROUNDING_ERROR_f64=0.00000001
{
	const float tolerance = 0.00000001; //
	return (a + tolerance >= b) && (a - tolerance <= b);
}