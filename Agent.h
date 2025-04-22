#ifndef AGENT_H
#define AGENT_H

class Agent {

private:
	int agentType;

public:
	Agent(int _agentType)
	{
		agentType = _agentType;
	}

	void SetAgentType(int _agentType) {agentType = _agentType;}
	int GetAgentType() {return agentType;}
};

#endif
