#ifndef DATAPOINT_H
#define DATAPOINT_H

class DataPoint {

private:
	int nNormalCells;
	int nAbnormalCells;
	float avgStickiness;
	float avgJumpRate;
	int nInvasiveCells;
	float membraneDensity;

public:
	DataPoint(int _nNormalCells, int _nAbnormalCells, float _avgStickiness, float _avgJumpRate, int _nInvasiveCells, float _membraneDensity)
	{
		nNormalCells = _nNormalCells;
		nAbnormalCells = _nAbnormalCells;
		avgStickiness = _avgStickiness;
		avgJumpRate = _avgJumpRate;
		nInvasiveCells = _nInvasiveCells;
		membraneDensity = _membraneDensity;
	}
};

#endif
