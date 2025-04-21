#ifndef ABNORMALCELL_H
#define ABNORMALCELL_H

class Model;
class AbnormalCell : public Cell {

private:
	Model* model;
	int age;
	int nDivisions;
	double stickiness;
	double jumpRate;

public:
	AbnormalCell(double _fitness, int _xPos, int _yPos, Model* _model, int _age, int _nDivisions, double _stickiness, double _jumpRate) : 
		Cell(_fitness, _xPos, _yPos)
	{
		model = _model;
		age = _age;
		nDivisions = _nDivisions;
		stickiness = _stickiness;
		jumpRate = _jumpRate;
		
	}

	void SetAge(int _age) {age = _age;}
	double GetAge() {return age;}

	void SetDivisions(int _divisions) {nDivisions = _divisions;}
	double GetDivisions() {return nDivisions;}

	void SetStickiness(int _stickiness) {stickiness = _stickiness;}
	double GetStickiness() {return stickiness;}

	void SetJumpRate(int _jumpRate) {jumpRate = _jumpRate;}
	double GetJumpRate() {return jumpRate;}
	
};

#endif
