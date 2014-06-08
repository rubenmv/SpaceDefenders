#include "Message.h"

const char* Message::TYPE_STRINGS[] = {
	"INCOMING_DAMAGE", 
	"HEALTH_CHANGED",
	"SHIELD_CHANGED",
	"START_SHOOTING",
	"STOP_SHOOTING",
	"COLLISION",
	"NEW_VISION",
	"LOST_VISION", 
	"NEW_AIM",
	"STOP_AIM",
	"NEW_TARGET",
	"TARGET_REACHED",
	"NEW_SNAPSHOT",
	"DEATH_SNAPSHOT",
	"NEW_FORCE",
	"ASSIGN_GAME_OBJECT",
	"JADE_CHANGED",
	"TRY_BUY",
	"SOLD",
	"NOT_SOLD",
	"STOP_MOVING",
	"START_MOVING",
	"EXPERIENCE_CHANGED",
	"LEVEL_CHANGED",
	"NEXT_EXPERIENCE_CHANGED",
	"NEXT_WAVE",
	"TARGET_SHOT",
	"ADD_EXPERIENCE",
	"RAY_SHOT",
	"DEATH",
	"STOP_RAY",
	"REPAIRED",
	"ON_CAMERA",
	"OUT_OF_CAMERA",
	"USE_SKILL",
	"TYPE_COUNT"
};