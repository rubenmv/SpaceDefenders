#include "../Vector2d.h"

void testVector2D()
{
	//Prueba Vector2d
  std::cout<<"Probar prueba Vector2d"<<std::endl;
  Vector2d *vec=new Vector2d(20.0, 144.0);
  std::cout<<"Vector 'vec' -> "<<*vec<<std::endl;
  Vector2d *vec2= new Vector2d();
  Vector2d *vec3= new Vector2d(1.0, 1.0);
  Vector2d *vec4= new Vector2d(1.0 ,0.0);

  std::cout<<"Vector 'vec2' -> "<<*vec2<<std::endl;
  //std::cout<<"Vector 'vec2' es igual q 'vec'? -> "<< Vector2d::*vec2.equals(&vec)<<std::endl;
  bool bo;
  std::cout<<"Vector 'vec2' es igual q 'vec'? -> "<< vec2->equals(*vec) << std::endl;
  bo=vec2->equals(*vec3);
  if(bo==false)
	  std::cout<<"ES FALSO "<< std::endl;
  else
	  std::cout<<"ES VERDADERO" << std::endl;
  float angulo=vec4->getAngle();
  std::cout<<"Angulo de VEC4-> " <<angulo<< std::endl;
  //T X
  float angulocon=vec3->getAngleWith(*vec4);
  std::cout<<"Angulo de VEC3 CON VEC4-> " <<angulocon<< std::endl;

  //prueba length
  float dist=vec4->getLength();
  std::cout<<"length de vec4 -> " <<dist<< std::endl;
  float dist2=vec3->getLength();
  std::cout<<"length de vec3 -> " <<dist2<< std::endl;
  //fin prueba de Vector2d


  std::cout<<"Prueba de operadores"<<std::endl;
  system("PAUSE");
}