#ifndef ABNORMALCELL_H
#define ABNORMALCELL_H

class Model;
class AbnormalCell : public Cell {

private:
	int age;
	int nDivisions;
	double stickiness;
	int jumpRate;

public:
	AbnormalCell(double _fitness, int _xPos, int _yPos, int _age, int _nDivisions, double _stickiness, int _jumpRate) : 
		Cell(_fitness, _xPos, _yPos)
	{
		age = _age;
		nDivisions = _nDivisions;
		stickiness = _stickiness;
		jumpRate = _jumpRate;
		
	}

	void SetAge(int _age) {age = _age;}
	int GetAge() {return age;}

	void SetDivisions(int _divisions) {nDivisions = _divisions;}
	int GetDivisions() {return nDivisions;}

	void SetStickiness(double _stickiness) {stickiness = _stickiness;}
	double GetStickiness() {return stickiness;}

	void SetJumpRate(int _jumpRate) {jumpRate = _jumpRate;}
	int GetJumpRate() {return jumpRate;}
	
};

#endif
