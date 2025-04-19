#ifndef ABNORMALCELL_H
#define ABNORMALCELL_H

class Model;
class AbnormalCell : Cell {

private:
	Model* model;
	float age;
	int nDivisions;
	float stickiness;
	float jumpRate;
	int xPos;
	int yPos;

public:
	AbnormalCell() (int _agentType, float _fitness, Model* _model, float _age, int _nDivisions, float _stickiness, float _jumpRate, int _xPos, int _yPos)
	{
		agentType = _agentType;
		fitness = _fitness;
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
