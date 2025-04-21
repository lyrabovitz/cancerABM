#ifndef ABNORMALCELL_H
#define ABNORMALCELL_H

class Model;
class AbnormalCell : Cell {

private:
	Model* model;
	double age;
	int nDivisions;
	double stickiness;
	double jumpRate;
	int xPos;
	int yPos;

public:
	AbnormalCell(int _agentType, double _fitness, Model* _model, double _age, int _nDivisions, double _stickiness, double _jumpRate, int _xPos, int _yPos) : 
		Cell(_agentType, _fitness)
	{
		model = _model;
		age = _age;
		nDivisions = _nDivisions;
		stickiness = _stickiness;
		jumpRate = _jumpRate;
		xPos = _xPos;
		yPos = _yPos;
	}
};

#endif
