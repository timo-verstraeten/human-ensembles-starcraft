#pragma once
#ifndef INC_ACTION_H
#define INC_ACTION_H

enum Action
{
	STOP = 0,
	ATTACK,
	MOVE_TOWARDS_ENEMY,
	MOVE_SOUTH,
	MOVE_NORTH,
	MOVE_EAST,
	MOVE_WEST,
	NUMBER_OF_ACTIONS // Final enumeration value to get the number of actions
};

#endif // INC_ACTION_H