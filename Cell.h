#ifndef CELL_H
#define CELL_H

class Cell : Agent {

private:
	float fitness;

public:
	Cell(int _agentType, float _fitness) : Agent(_agentType)
	{
		fitness = _fitness;
	}
};

#endif
