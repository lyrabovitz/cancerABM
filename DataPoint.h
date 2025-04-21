#ifndef DATAPOINT_H
#define DATAPOINT_H

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
};

#endif
