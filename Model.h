#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <chrono>
#include <random>

#include "DataPoint.h"
#include "Agent.h"
#include "Cell.h"
#include "AbnormalCell.h"

class Model{

private:
	//data structures for memory management and abnormal cell parameters
	std::vector<AbnormalCell*> abnormalCells = {};
	std::vector<DataPoint*> dataPoints = {};
	std::vector<Cell*> normalCells = {};
	std::vector<Agent*> agents = {};

	//data structures representing the space
	std::vector<std::vector<Agent*>> agentSpace = {};
	std::vector<std::vector<Cell*>> normalCellSpace = {};

	//simulation
	int tStep;
	double time = 0;
	double firstCellExitTime = 0;
	double hundredCellExitTime = 0;
	double membraneDensity = 1;
	double membraneDensityPerCell;
	int nInvasiveCells = 0;
	int nAbnormalCells = 0;
	int nNormalCells = 0;

	//real time
	std::chrono::steady_clock::time_point tStart;
	std::chrono::steady_clock::time_point tEnd;

	//random number generation 
	std::mt19937 generator; //(mersenne twister)

	//generation methods
	void GenerateMembranes();
	void GenerateNormalCell(int row, int column);
	void GenerateNormalCells();
	void GenerateInitialMutation();
	void GenerateProteins();

	//simulation methods
	void ProliferateCell(AbnormalCell* cell);
	void MoveCell(AbnormalCell* cell);

public:
	enum agentType
	{
		EMPTY, NORMAL, ABNORMAL, PROTEIN, MEMBRANE, BREACHEDMEMBRANE
	};

	//simulation parameters
	int maxCells;
	double membranePorosity;
	int divisionAge;
	double stickinessMutationRate;
	int jumpMutationRate;
	int maxDivisions;
	double divisionRateAbnormal;
	double deathRateAbnormal;
	double proteinDensity;
	int maxNormalCellsPerRow;
	double timePerStep;
	
	int nRows;
	int nColumns;

	void Initialize();

	void AdvanceSimulation();

	std::vector<DataPoint*> GetDataPoints() {return dataPoints;}

	//visualize agent space for testing purposes
	void TestPrint();
	void AbTest();
	
	Model();
	~Model();
};

#endif
