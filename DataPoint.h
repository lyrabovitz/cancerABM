#ifndef DATAPOINT_H
#define DATAPOINT_H

class DataPoint {

private:
	Model model;
	int nNormalCells;
	int nAbnormalCells;
	float avgStickiness;
	float avgJumpRate;
	int nInvasiveCells;
	float membraneDensity;
};

#endif
