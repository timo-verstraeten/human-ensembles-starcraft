#include "SMDPAgent.h"

Action SMDPAgent::startEpisode(const State &state, std::ostream &output)
{
	initialize(output);
	Action action = nextAction(state, output);
	applyAction(action, output);
	return action;
}

Action SMDPAgent::step(double reward, const State &state, std::ostream &output)
{
	giveReward(reward, output);
	Action action = nextAction(state, output);
	applyAction(action, output);
	return action;
}

void SMDPAgent::endEpisode(double reward, std::ostream &output)
{
	giveReward(reward, output);
	finalize(output);
}
