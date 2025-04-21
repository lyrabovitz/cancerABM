#ifndef CELL_H
#define CELL_H

class Cell : Agent {

private:
	double fitness;

public:
	Cell(int _agentType, double _fitness) : Agent(_agentType)
	{
		fitness = _fitness;
	}
};

#endif
