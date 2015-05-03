#include "Action.h"

std::string actionToString(Action action)
{
	switch (action) {
	case STOP:
		return "STOP";
	case ATTACK:
		return "ATTACK";
	//case MOVE_TOWARDS_ENEMY:
	//	return "MOVE_TOWARDS_ENEMY";
	case MOVE_SOUTH:
		return "MOVE_SOUTH";
	case MOVE_NORTH:
		return "MOVE_NORTH";
	case MOVE_EAST:
		return "MOVE_EAST";
	case MOVE_WEST:
		return "MOVE_WEST";
	default:
		return "UNKNOWN ACTION";
	}
}