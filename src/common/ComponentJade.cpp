#include "ComponentJade.h"
#include "GameObject.h"

ComponentJade::ComponentJade(int basic)
{
	jade = basic;
}


ComponentJade::~ComponentJade(void)
{

}

void ComponentJade::update()
{

}
void ComponentJade::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::TRY_BUY:
		canBuy(message.value, message.gameObject);
		break;
	case Message::ADD_EXPERIENCE:
		jade += message.value*2/3;
		Message newMessage;
		newMessage.type = Message::JADE_CHANGED;
		newMessage.value = jade;
		parent->broadcastMessage(newMessage);
		break;
	}
}

void ComponentJade::canBuy(int cant, GameObject* seller)
{
	Message message;
	if(cant <= jade)
	{
		jade = jade - cant;
		message.type = Message::SOLD;
		seller->broadcastMessage(message);

		message.type = Message::JADE_CHANGED;
		message.value = jade;
		parent->broadcastMessage(message);
	}
	else
	{
		message.type = Message::NOT_SOLD;
		seller->broadcastMessage(message);
	}
}