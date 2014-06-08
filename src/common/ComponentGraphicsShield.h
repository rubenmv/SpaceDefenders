#pragma once
#include "Component.h"
#include "Clock.h"

class NodeMesh;

class ComponentGraphicsShield : public Component
{
public:
	ComponentGraphicsShield(bool groundShield, float radius);
	~ComponentGraphicsShield(void);

	virtual void update();
	virtual void onMessage(const Message& message);
private:
	//cambia enciende o apaga el escudo
	void changedShield( Vector2d target );
	//Cambia la intesidad del escudo;
	void activateShield();
	//ACtualiza el escudo
	void updateShield();
	NodeMesh* node;
	bool enabled;
	//Debe ir entre 0 y 1;
	float intensity;
	//controla si debe subir la energia o bajar
	bool up;
	
	//Si es un edificio debe bajar la esfera.
	bool groundShield;
	Clock clock;

	//Numero de impactos que lleva
	int nextImpact; 
	//Lista de impactos
	float impactList[16];
	float impactListTemp[16];
	//Agrega un nuevo impacto a la lista;
	void impactEnabled( Vector2d position );
	void updateImpacts();
	//Envia la lista de impactos al nodeMEsh, la tercera posicion sera el eje Z
	void sendImpacts();
	void initImpact();
};

