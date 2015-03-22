#pragma once
#ifndef INC_POTENTIALS_H
#define INC_POTENTIALS_H

#include "Potential.h"

#include "../env/Action.h"
#include "../env/State.h"

/**
 * A potential that is zero everywhere.
 */
class ZeroPotential : public Potential
{
public:
	ZeroPotential() : Potential(0) {}
	virtual ~ZeroPotential() {}

protected:
	// --- Potential ---
	virtual double getUnscaled(const State &state, Action action) { return 0; }
};

/**
 * A potential proportional to the difference in hitpoints between the controlled unit and its enemy.
 */
class HealthPotential : public Potential
{
public:
	HealthPotential(double scaling) : Potential(scaling) {}
	virtual ~HealthPotential() {}
	
protected:
	// --- Potential ---
	virtual double getUnscaled(const State &state, Action action) { return state.hitPointDifference / 100.0; }
};

/**
 * A potential proportional to how close the controlled unit is to its enemy.
 */
class DistancePotential : public Potential
{
public:
	DistancePotential(double scaling) : Potential(scaling) {}
	virtual ~DistancePotential() {}
	
protected:
	// --- Potential ---
	virtual double getUnscaled(const State &state, Action action) { return 1 - state.enemyDistance / 1000; }
};

/**
 * A potential combining of the health difference and distance to the enemy, based on whether the enemy is moving/attacking or not.
 */
class HealthDistancePotential : public Potential
{
public:
	HealthDistancePotential(double scaling) : Potential(scaling) {}
	virtual ~HealthDistancePotential() {}
	
protected:
	// --- Potential ---
	virtual double getUnscaled(const State &state, Action action) { return state.enemyActive ? (state.hitPointDifference / 100.0) : (1 - state.enemyDistance / 1000); }
};

#endif // INC_POTENTIALS_H
