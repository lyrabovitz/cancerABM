#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <chrono>

#include "DataPoint.h"
#include "Agent.h"
#include "Cell.h"
#include "AbnormalCell.h"

class Model{

private:
	std::vector<AbnormalCell*> abnormalCells;
	std::vector<DataPoint*> dataPoints;
	std::vector<Cell*> normalCells;
	std::vector<Agent*> agents;
	std::vector<std::vector<Agent*>> agentSpace;
	std::vector<std::vector<Cell*>> normalCellSpace;
	int tStep;
	double time;
	std::chrono::steady_clock::time_point tStart;
	std::chrono::steady_clock::time_point tEnd;
	

public:
	int maxCells;
	double membranePorosity;
	int divisionAge;
	double stickinessMutationRate;
	double jumpMutationRate;
	int maxDivisions;
	double divisionRateAbnormal;
	double deathRateAbnormal;
	double proteinDensity;
	int maxNormalCells;
	double timePerStep;
	double firstCellExitTime = 0;
	double hundredCellExitTime = 0;
	int nRows;
	int nColumns;


	void Initialize();

	void AdvanceSimulation();

	void ProliferateCell(AbnormalCell cell);

	void MoveCell(AbnormalCell cell);

	Model();
	~Model();
};

#endif
