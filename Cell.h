#ifndef CELL_H
#define CELL_H

class Cell {

private:
	double fitness;
	int xPos;
	int yPos;

public:
	Cell(double _fitness, int _xPos, int _yPos)
	{
		fitness = _fitness;
		xPos = _xPos;
		yPos = _yPos;
	}

	double GetFitness() {return fitness;}

	int GetXPos() {return xPos;}
	void SetXPos(int x) {xPos = x;}

	int GetYPos() {return yPos;}
	void SetYPos(int y) {yPos = y;}
};

#endif
