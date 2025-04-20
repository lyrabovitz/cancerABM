#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "DataPoint.h"
#include "Agent.h"
#include "Cell.h"
#include "AbnormalCell.h"

class Model{

private:
	std::vector<AbnormalCell> abnormal_cells;
	std::vector<DataPoint> data_points;
	std::vector<Cell> cells;
	std::vector<Agent> agents;
	int tStep;
	int maxCells;
	float membranePorosity;
	float jumpMutationRate;
	int divisionAge;
	float stickinessMutationRate;
	int maxDivisions;
	float divisionRateAbnormal;
	float deathRateAbnormal;
	float proteinDensity;
	int maxNormalCellsPerColumn;
	int timeTotal;
	int nTimeSteps;
	float firstCellExitTime;
	float hundredCellExitTime;

public:
	void Initialize();

	void AdvanceSimulation();

	void ProliferateCell(AbnormalCell cell);

	void MoveCell(AbnormalCell cell);

	Model();
};

#endif
