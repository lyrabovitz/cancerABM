#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <iostream>

class DataPoint {

private:
	double time;
	int nNormalCells;
	int nAbnormalCells;
	double avgStickiness;
	double avgJumpRate;
	int nInvasiveCells;
	double membraneDensity;

public:
	DataPoint(double _time, int _nNormalCells, int _nAbnormalCells, double _avgStickiness, double _avgJumpRate, int _nInvasiveCells, double _membraneDensity)
	{
		time = _time;
		nNormalCells = _nNormalCells;
		nAbnormalCells = _nAbnormalCells;
		avgStickiness = _avgStickiness;
		avgJumpRate = _avgJumpRate;
		nInvasiveCells = _nInvasiveCells;
		membraneDensity = _membraneDensity;
	}
	
	double GetTime() {return time;}
	double GetAvgStickiness() {return avgStickiness;}
	double GetAvgJumpRate() {return avgJumpRate;}
	double GetMembraneDensity() {return membraneDensity;}
	int GetNormalCells() {return nNormalCells;}
	int GetAbnormalCells() {return nAbnormalCells;}
	int GetInvasiveCells() {return nInvasiveCells;}
};

#endif
