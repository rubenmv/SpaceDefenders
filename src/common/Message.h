#pragma once
#include "Vector2d.h"
#include "ID.h"


/* EJEMPLOS
Message message;
message.type = Message::INCOMING_DAMAGE;
message.value = damage;

void ComponentDebugGameObject::onMessage(const Message& message)
{
	switch (message.type)
	{
	case Message::HEALTH_CHANGED:
		onHealthChanged(message.value);
		break;
	case Message::SHIELD_CHANGED:
		onShieldChanged(message.value);
		break;
	}
}

*/

class GameObject;

struct Message
{
	//Los componentes estan puestos sin la palabra Componente :D ComponentExplosionDamage -> ExplosionDamage
	enum Type{

		//Tiene que recibir danyo,
		//value: Damage, si es negativo curara
		//Emisores: Otros GameObjects
		//Receptores: Health
		//DebugColor: RED
		INCOMING_DAMAGE, 

		//La vida ha cambiado
		//target: vida actual y maxima
		//Emisores: Health.
		//Receptores: DebubGameObject, GraphisEngine
		//DebugColor: GREEN
		HEALTH_CHANGED,

		//El escudo ha cambiado
		//target: escudo actual y maximo
		//Emisores: Health.
		//Receptores: DebubGameObject, GraphisEngine
		//DebugColor: BLUE
		SHIELD_CHANGED,

		//Empieza a disparar o explota si es una bomba
		//gameObject: Objetivo al que disparar
		//Emisores: AI, AIMine, ...
		//Receptores: ExplosionDamage, DirectDamage, ...
		//DebugColor: YELLOW
		START_SHOOTING,

		//Deja de disparar
		//Emisores: AI,
		//Receptores: DirectDamage, ...
		//DebugColor: WHITE
		STOP_SHOOTING,

		//Se ha chocado con algo
		//gameObject: GameObject con el que ha chocado
		//Emisores: Collider.
		//Receptores: Ninguno?
		//DebugColor: GREY
		COLLISION,
		
		//Algun gameObject ha entrado en el rango de vision
		//gameObject: GameObject que ha entrado en el rango de vision
		//Emisores: Collider.
		//Receptores: AITurret, DirectDamage, AIMine (Hace empezar la cuentra atras para la explosion)
		//DebugColor: RED
		NEW_VISION,

		//Algun gameObject ha salido del rango de vision
		//gameObject: GameObject que ha dejado de ver
		//Emisores: Collider.
		//Receptores: AITurret, AIMine
		//DebugColor: GREEN
		LOST_VISION, 

		//Nuevo GameObject al que apuntar, null para dejar de apuntar
		//GameObject: objetivo, NULL para dejar de apuntar
		//Emisores: AI
		//Receptores: TargetAim
		//DebugColor: BLUE
		NEW_AIM,

		//Deja de apuntar, null para dejar de apuntar
		//GameObject: objetivo, NULL para dejar de apuntar
		//Emisores: AI
		//Receptores: TargetAim
		//DebugColor: BLUE
		STOP_AIM,


		//Nuevo objetivo al que moverse, para que se quede quieto se le mandara como objetivo su posicion actual
		//target: Posicion objetiva, 
		//Emisores: AI
		//Receptores: TargetPath
		//DebugColor: YELLOW
		NEW_TARGET,
		
		//Llegada al objetivo
		// --- Sin datos
		//Emisores: ComponentTargetPath
		//Receptores: ComponentAImyDumb
		//DebugColor: YELLOW
		TARGET_REACHED,

		//Mensaje con la posicion del servidor para interpolar en los remote GameObjects
		//target: Vector2d con la nueva posicion
		//value: Nueva rotation.
		//time: Tiempo en el que se ha recibido.
		//Emisores: GameObject::Deserialize(Funcion de la interfaz replica3 de RakNet)
		//Receptores: ComponentMovementInterpolation
		NEW_SNAPSHOT,

		//Mensaje de que debe morir al terminar de interpolar
		//time: Tiempo en el que se ha recibido en el cliente.
		//Emisores: GameObject::Dealloc(Funcion de la interfaz replica3 de RakNet)
		//Receptores: ComponentMovementInterpolation
		DEATH_SNAPSHOT,

		//Añade fuerzas del steering behaviours
		//target: Vector2d con la fuerza a añadir
		//Emisores: AIFlocking
		//Receptores: GameObject en movimiento
		NEW_FORCE,

		//Selecciona esta camara como predetermiada
		//target: NULL
		//Emisores: Cualquiera que quiera cambiar la camara
		//Receptores: Component camara para seleccionar esa
		ASSIGN_GAME_OBJECT,

		//Intenta comprar
		//target: NULL
		//Emisores: Cualquiera que quiera cambiar la camara
		//Receptores: Component camara para seleccionar esa
		JADE_CHANGED,

		//Intenta comprar
		//target: Componente inventario que quiere venderse
		//Emisores: Componente inventario que quiere venderse
		//Receptores: MainPlayer que quiere crearlo
		TRY_BUY,

		//Tienes jade te lo vendo
		//target: NULL
		//Emisores: MainPlayer poseedor de jade
		//Receptores: Componente de inventario
		SOLD,

		//No tienes jade suficiente, no te lo vendo
		//target: NULL
		//Emisores: MainPlayer poseedor de jade
		//Receptores: Componente de inventario
		NOT_SOLD,

		//Detiene el movimiento para disparar
		//Emisores: TargetAim
		//Receptores: Movement
		STOP_MOVING,

		//Activa el movimiento
		//Emisores: TargetAim
		//Receptores: Movement
		START_MOVING,

		//La experiencia ha cambiado
		//value: Nueva cantidad de experiencia
		//Emisores: Experience.
		//Receptores: DebubGameObject, GraphicsEngine
		EXPERIENCE_CHANGED,

		//El nivel ha cambiado
		//target: x modificador de vida y escudo, y modificador de damage
		//value: Nuevo nivel
		//Emisores: Experience.
		//Receptores: DebubGameObject, GraphicsEngine
		LEVEL_CHANGED,

		//La experiencia para el siguiente nivel ha cambiado ha cambiado
		//value: Nueva cantidad de experiencia
		//Emisores: Experience.
		//Receptores: DebubGameObject, GraphicsEngine
		NEXT_EXPERIENCE_CHANGED,

		//La oleada ha cambiado
		//value: Nueva oleada
		//Emisores: Health.
		//Receptores: DebubGameObject, GraphicsEngine
		NEXT_WAVE,

		//Skill de tiro dirigido
		//value: Nueva oleada
		//Emisores: skill
		//Receptores: player
		TARGET_SHOT,

		//Enemigos dando experiencia
		//value: cantidad de experiencia
		//Emisores: ComponentGiveHealh
		//Receptores: ComponentLevelExp
		ADD_EXPERIENCE,

		//Mensaje para cambiar a rayo
		//Emisor: input, boton
		//Receptor: projectilLauncher
		RAY_SHOT,

		//Mensaje que se manda cuando la vida llega a 0
		//Emisor: componentHealth
		//Receptor: todos los componentes que quieran sabes si ha muerto
		DEATH,
		//Numero total de tipos, tiene que estar la ultima

		//Mensaje que se manda cuando se termina el rayo para ver si debe seguir disparando
		//no encuentro una manera de que funcione
		//Emisor: componentRay
		//Receptor: projectilLauncher
		STOP_RAY,

		//Se ha reparado y puede volver a moverse, sirve para poder levantar a los jugadores una vez muertos
		//Emisor: componentRepairSystem 
		//Receptor: componentHealth de otro gameobject
		REPAIRED,

		//A entrado en la camara y tiene que dibujarse (clipping)
		//Emisor: componentCollider del player
		//Receptor: componentGraphics
		ON_CAMERA,

		//Se ha salido de la camara y no tiene que dibujarse (clipping)
		//Emisor: componentCollider del player
		//Receptor: componentGraphics
		OUT_OF_CAMERA,

		//Mensaje para poder seleccionar distintas skills
		//Emisor: eventManager
		//Receptor: componentSkill
		//Value: el numero de la skill a la que deseas cambiar
		USE_SKILL,

		TYPE_COUNT
	};

	static const char* TYPE_STRINGS[];

	friend std::ostream& operator<<(std::ostream& os, enum Type n)
	{
		if (n >= TYPE_COUNT || n < 0) return os << "???";
		return os << TYPE_STRINGS[n];
	}
	friend std::wostream& operator<<(std::wostream& os, enum Type n)
	{
		if (n >= TYPE_COUNT || n < 0) return os << "???";
		return os << TYPE_STRINGS[n];
	}

	Message():type(TYPE_COUNT), value(0), target(Vector2d()), gameObject(NULL), time(0) {}

	Type type;
	float value;
	Vector2d target; // Posicion donde se desea realizar una accion
	GameObject* gameObject;  // Objeto origen o destino de la accion
	unsigned int time;
};

