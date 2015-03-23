#pragma once
#ifndef INC_STATE_H
#define INC_STATE_H

#include <vector>

/**
 * Struct containing the features of the state in which the controlable unit finds itself.
 */
struct State
{
public:
	int x;                  // my X position (0-1000)
	int y;                  // my Y position (0-1000)
	double enemyDistance;   // Straight-line distance between me and enemy (0-1000)
	int hitPointDifference; // difference in hit points between me and enemy (-50 - 50)
	bool enemyActive;       // enemy is moving/attacking? (0 or 1)
	double enemyAngle;      // angle of enemy relative to me w.r.t. the coordinate system (pi - pi)
};

#endif // INC_STATE_H
