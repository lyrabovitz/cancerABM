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
	int GetYPos() {return yPos;}
};

#endif
