#pragma once
#pragma warning( disable : 4244 4305)

#define KPI 3.14159265358979323846
//#define ROUNDING_ERROR_f32 0.000001f;

class Math {
public:

	static const float PI;
	static const float HALF_PI;
	static const float TWO_PI;
	static const float DEGTORAD;
	static const float RADTODEG;
	//static const float ROUNDING_ERROR_f32;

	static float abs(float n);
	static float max_(float a, float b);
	static float min_(float a, float b);
	//Restringe un valor n para que no exceda un minimo ni un maximo 
	static float constraint(float n, float low, float high);

	static float cos(float n);
	static float sin(float n);
	static float tan(float n);
	static float acos(float n);
	static float asin(float n);
	static float atan(float n);
	static float atan2(float y, float x);

	static float pow(float base, float exponent);
	static float sqrt(float n);

	//Redondea hacia abajo
	static float floor(float n);
	//Redondea hacia arriba
	static float ceil(float n);
	//Redondea normal
	static float round(float n);
	static float round(float n, float decimals);

	static float mod(float n, int base);

	//Reduce el angulo al rango -180, 180
	static float warpAngle(float angle);

	//Devuelve un entero entre 0 y high
	static int random(int high);

	//Devuelve un entero entre low y high
	static int random(int low, int high);

	//returns if a equals b, taking possible rounding errors into account 
	//http://irrlicht.sourceforge.net/docu/namespaceirr_1_1core.html#abf9b9b140cc365908ea4c8c47451e4e3
	//static bool equals(const float a, const float b, const float tolerance = 0.00000001); //ROUNDING_ERROR_f64=0.00000001
	static bool equals(const float a, const float b); //ROUNDING_ERROR_f64=0.00000001 //NOTA tolerancia de 0.00000001
	

};

#undef KPI


