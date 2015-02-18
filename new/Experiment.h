#pragma once
#ifndef INC_EXPERIMENT_H
#define INC_EXPERIMENT_H

class Trial;

class Experiment
{
public:
	virtual ~Experiment() {}
	
	virtual Trial *nextTrial() = 0;
};

#endif // INC_EXPERIMENT_H
