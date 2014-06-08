#pragma once

#include "Component.h"

//Envia al original un tipo de mensaje que se le indica en el constructor
//Si se quiere reenviar varios, hay que crear un componente por tipo de mensaje.
//Por ahora no funciona con mensajes que envien GameObjects
class ComponentRemoteMessage : public Component
{
public:
	ComponentRemoteMessage(Message::Type messageType):messageType(messageType){};
	~ComponentRemoteMessage(void){};

	//virtual void awake();

	virtual void update(){};
	virtual void onMessage(const Message& message);

private:
	Message::Type messageType;

	void sendMessage(Message message);

};

