#ifndef ABNORMALCELL_H
#define ABNORMALCELL_H

class AbnormalCell : Cell {

private:
	Model model;
	float age;
	int nDivisions;
	float stickiness;
	float jumpRate;
	int xPos;
	int yPos;
};

#endif
