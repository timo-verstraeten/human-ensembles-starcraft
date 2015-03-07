#pragma once
#ifndef INC_POTENTIALS_H
#define INC_POTENTIALS_H

#include "Potential.h"

#include "State.h"

class ZeroPotential : public Potential
{
public:
	ZeroPotential() : Potential(0) {}
	virtual ~ZeroPotential() {}

protected:
	virtual double getUnscaled(const State &state, Action action) { return 0; }
};

class HealthPotential : public Potential
{
public:
	HealthPotential(double scaling) : Potential(scaling) {}
	virtual ~HealthPotential() {}
	
protected:
	virtual double getUnscaled(const State &state, Action action) { return state.hitPointDifference / 100.0; }
};

class DistancePotential : public Potential
{
public:
	DistancePotential(double scaling) : Potential(scaling) {}
	virtual ~DistancePotential() {}
	
protected:
	virtual double getUnscaled(const State &state, Action action) { return 1 - state.enemyDistance / 1000; }
};

class HealthDistancePotential : public Potential
{
public:
	HealthDistancePotential(double scaling) : Potential(scaling) {}
	virtual ~HealthDistancePotential() {}
	
protected:
	virtual double getUnscaled(const State &state, Action action) { return state.enemyActive ? (state.hitPointDifference / 100.0) : (1 - state.enemyDistance / 1000); }
};

#endif // INC_POTENTIALS_H
