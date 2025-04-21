#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <array>
#include <numeric>
#include <utility>

#include "Model.h"

Model::Model()
{
	tStart = std::chrono::steady_clock::now();
	tStep = 1;
}

Model::~Model()
{
	//destroy agents and data points
	for (auto element : agents)
	{
		delete(element);
	}
	for (auto element : normalCells)
	{
		delete(element);
	}
	for (auto element : abnormalCells)
	{
		delete(element);
	}
	for (auto element : dataPoints)
	{
		delete(element);
	}

	tEnd = std::chrono::steady_clock::now();
	std::cout << "Total Duration = " << 
		std::chrono::duration_cast<std::chrono::seconds>
		(tEnd - tStart).count()<< "[s]" << std::endl;
	std::cout << "Total Duration = " << 
		std::chrono::duration_cast<std::chrono::milliseconds>
		(tEnd - tStart).count()<< "[ms]" << std::endl;
	
}

void Model::Initialize()
{
	//seed random number generator from system clock
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator.seed(seed);

	//generate empty agent space and cell space
	Agent* nullAgent = new Agent(EMPTY);
	agents.push_back(nullAgent);
	Cell* nullCell = new Cell(0, -1, -1);
	normalCells.push_back(nullCell);

	std::vector<Agent*> emptyAgentRow;
	std::vector<Cell*> emptyCellRow;
	for(int i = 0; i < nRows; i++)
	{
		agentSpace.push_back(emptyAgentRow);
		normalCellSpace.push_back(emptyCellRow);

		for(int j = 0; j < nColumns; j++)
		{
			agentSpace[i].push_back(nullAgent);
			normalCellSpace[i].push_back(nullCell);
		}
	}

	//generate membranes
	GenerateMembranes();
	
	//generate healthy cells
	GenerateNormalCells();

	//generate initial mutation
	GenerateInitialMutation();

	//generate extracellular matrix proteins
	GenerateProteins();

	//generate initial data point
	int nNormalCells = normalCells.size();
	int nAbnormalCells = 1;
	double avgStickiness = abnormalCells[0]->GetStickiness();
	double avgJumpRate = abnormalCells[0]->GetJumpRate();
	
	DataPoint* dataPoint = new DataPoint(time, nNormalCells, 
		nAbnormalCells, avgStickiness, avgJumpRate, nInvasiveCells, 
		membraneDensity);
	dataPoints.push_back(dataPoint);
}

void Model::AdvanceSimulation() 
{
	double sumStickiness = 0;
	double sumJumpRate = 0;
	//for each abnormal cell
	{
		//increase age

		//mutate stickiness and jump radius, add to sums

		//random chance for cell death if divisions left is 0

		//otherwise random chance for proliferation if cell is over division age


	}
	//generate data point
	int nNormalCells = normalCells.size();
	int nAbnormalCells = abnormalCells.size();
	double avgStickiness = sumStickiness / static_cast<double>(nAbnormalCells);
	double avgJumpRate = sumJumpRate / static_cast<double>(nAbnormalCells);
	//!!!!!!!!!MembraneDensity needs to be implemented
	DataPoint* dataPoint = new DataPoint(time, nNormalCells, nAbnormalCells, avgStickiness, avgJumpRate, nInvasiveCells, 0);
	dataPoints.push_back(dataPoint);

	//increase time step and time
	tStep++;
	time += timePerStep;
	
}

/*

Proliferates an abnormal cell

*/
void Model::ProliferateCell(AbnormalCell cell) 
{
	return;
}

/*

Moves an abnormal cell. Returns 1 if cell becomes invasive (membrane breached)

*/
int Model::MoveCell(AbnormalCell cell) 
{
	// TODO - implement Model::MoveCell
	throw "Not yet implemented";
}

void Model::GenerateMembranes()
{
	Agent* membrane;
	//first and last row are all membrane
	for(int i = 0; i < nColumns; i++)
	{
		//first row
		membrane = new Agent(MEMBRANE);
		agentSpace[0][i] = membrane;
		agents.push_back(membrane);

		//last row
		membrane = new Agent(MEMBRANE);
		agentSpace[nRows-1][i] = membrane;
		agents.push_back(membrane);
	}
	//left and right border for every other row
	for(int i = 1; i < nRows - 1; i++)
	{
		//left border
		membrane = new Agent(MEMBRANE);
		agentSpace[i][0] = membrane;
		agents.push_back(membrane);

		//last row
		membrane = new Agent(MEMBRANE);
		agentSpace[i][nColumns-1] = membrane;
		agents.push_back(membrane);
	}
}

/*

Generate healthy cell at specified position

*/
void Model::GenerateNormalCell(int row, int column)
{
	//generate fitness and position

	//fitness depends on how many times a cell has divided and the maximum # of divisions
	//fitness = (1 - # divisions) / max divisions
	//how many times cell has divided initially is randomly between 0 and maximum divisions
	std::uniform_int_distribution<int> distribution(0, maxDivisions);
	int nDivisions = distribution(generator);
	double fitness = static_cast<double>(maxDivisions-nDivisions) / static_cast<double>(maxDivisions);

	Cell* newCell = new Cell(fitness, row, column);
	Agent* newAgent = new Agent(NORMAL);

	//memory management, cells and agents deleted from these vectors in model destructor
	agents.push_back(newAgent);
	normalCells.push_back(newCell);

	//place in space
	normalCellSpace[row][column] = newCell;
	agentSpace[row][column] = newAgent;
}

/*

Generate initial population of healthy cells

*/
void Model::GenerateNormalCells()
{
	//cells are generated within inner region of space with 10 empty spaces between inner region and membrane
	for(int i = 11; i < nRows-11; i++)
	{
		//place max number of cells in the row
		//generate x coordinates for position

		//range of positions from 11 to nColumns-11
		int nPositions = nColumns-22;
		std::vector<int> xPositions(nPositions);
		std::iota(xPositions.begin(), xPositions.end(), 11);

		//randomly shuffle possible positions
		std::shuffle(xPositions.begin(), xPositions.end(), generator);

		//place cells
		for(int j = 0; j < maxNormalCellsPerRow; j++)
		{
			GenerateNormalCell(i,xPositions[j]);
		}
	}
}

/*

Replaces a random healthy cell with a mutated one

*/
void Model::GenerateInitialMutation()
{
	//pick a healthy cell
	std::shuffle(normalCells.begin(), normalCells.end(), generator);
	Cell* cell = normalCells[normalCells.size()-1];
	int xPos = cell->GetXPos();
	int yPos = cell->GetYPos();

	//delete healthy cell
	normalCells.pop_back(); 
	delete(cell);

	//create mutated cell
	//generate parameters
	//fitness for abnormal cells is randomly between 0.5 and 1
	
	//std::uniform_real_distribution<double> distribution(0, 0.5);
	double fitnessInitial = 0.5; // + distribution(generator);
	int ageInitial = 0;
	int divisionsInitial = 0;
	double stickinessInitial = 0.9;
	double jumpRateInitial = 1;

	//create cell
	Agent* agent = new Agent(ABNORMAL);
	agentSpace[xPos][yPos] = agent;

	AbnormalCell* mutatedCell = new AbnormalCell(fitnessInitial, xPos, 
		yPos, this, ageInitial, divisionsInitial, stickinessInitial, 
		jumpRateInitial);
	abnormalCells.push_back(mutatedCell);
}

/*

Generate initial distribution of extracellular matrix proteins

*/
void Model::GenerateProteins()
{
	//number of proteins to generate
	int nProteins = static_cast<int>(proteinDensity * static_cast<double>(maxCells));

	//locate empty cells
	//x,y coordinates for each empty cell
	std::vector<std::pair<int,int>> emptyCells;

	//we know that the outer border is not empty, so we can ignore those cells
	for(int i = 1; i < nRows-1; i++)
	{
		for(int j = 1; j < nColumns-1; j++)
		{
			//if empty, store position
			if(agentSpace[i][j]->GetAgentType() == EMPTY)	
			{
				std::pair<int,int> cell(i,j);
				emptyCells.push_back(cell);
			}
		}
	}

	//shuffle possible locations
	std::shuffle(emptyCells.begin(), emptyCells.end(), generator);

	Agent* protein;
	//place proteins
	for(int i = 1; i < nProteins; i++)
	{
		protein = new Agent(PROTEIN);

		//memory management: destroyed in model destructor
		agents.push_back(protein);

		agentSpace[emptyCells[i].first][emptyCells[i].second] = protein;
	}

}

void Model::TestPrint()
{
	std::cout << "----Agent Space----\n";
	for(auto row : agentSpace)
	{
		for(Agent* agent : row)
		{
			std::cout << agent->GetAgentType();
		}
		std::cout << "\n";
	}
}