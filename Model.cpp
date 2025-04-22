#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <array>
#include <numeric>
#include <utility>
#include <cmath>

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
	
	Cell* nullCell = new Cell(EMPTY, -1, -1);
	normalCells.push_back(nullCell);

	std::vector<Agent*> emptyAgentRow = {};
	std::vector<Cell*> emptyCellRow = {};
	for(int i = 0; i < nRows; i++)
	{
		agentSpace.push_back(emptyAgentRow);
		normalCellSpace.push_back(emptyCellRow);

		for(int j = 0; j < nColumns; j++)
		{
			Agent* nullAgent = new Agent(EMPTY);
			agents.push_back(nullAgent);
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
	//for random chance checks
	std::uniform_real_distribution<double> probability(0.0,1.0);

	double sumStickiness = 0;
	double sumJumpRate = 0;

	std::vector<int> toErase = {};

	//for each abnormal cell
	for(int i = 0; i < nAbnormalCells; i++)
	{
		AbnormalCell* abCell = abnormalCells[i];

		//if cell has breached membrane, mark for deletion and skip rest of this loop iteration
		if(abCell->GetXPos() == -1)
		{
			toErase.push_back(i);
			continue;
		}
		int xPos = abCell->GetXPos();
		int yPos = abCell->GetYPos();

		//increase age
		abCell->SetAge(abCell->GetAge()+1);

		//random chance for cell death if divisions left is >= max divisions
		if(abCell->GetDivisions() >= maxDivisions && probability(generator) < deathRateAbnormal)
		{
			nAbnormalCells -= 1;
			agentSpace[xPos][yPos]->SetAgentType(EMPTY);
			toErase.push_back(i);
		}
		//otherwise random chance for proliferation and possibly movement if cell is over division age
		else
		{
			//add stickiness and jump rate to sums for averaging later
			sumStickiness += abCell->GetStickiness();
			sumJumpRate += abCell->GetJumpRate();

			if(abCell->GetAge() >= divisionAge && probability(generator) < divisionRateAbnormal)
			{
				//cell proliferation
				ProliferateCell(abCell);

				//cell movement
				int x = abCell->GetXPos();
				int y = abCell->GetYPos();

				//stickiness check
				//# occupied neighboring grid cells
				int occupied = 0;
				for(int i = x-1; i <= x+1; i++)
				{
					for(int j = y-1; j <= y+1; j++)
					{
						if((i != x || j != y) //ignore space occupied by this cell
							&& agentSpace[i][j]->GetAgentType() != EMPTY) {occupied++;}
					}
				}
				double threshold = pow(1-abCell->GetStickiness(), occupied);
				if(probability(generator) < threshold)
				{
					MoveCell(abCell);
				}
			}
		}	
	}
	//delete dead cells
	for(int i : toErase)
	{
		delete(abnormalCells[i]);
	}
	int nErased = 0;
	for(int i : toErase)
	{
		abnormalCells.erase(abnormalCells.begin()+i-nErased);
		nErased++;
	}

	//increase time step and time
	tStep++;
	time += timePerStep;

	//generate data point
	double avgStickiness = sumStickiness / static_cast<double>(nAbnormalCells);
	double avgJumpRate = sumJumpRate / static_cast<double>(nAbnormalCells);
	DataPoint* dataPoint = new DataPoint(time, nNormalCells, nAbnormalCells, avgStickiness, avgJumpRate, nInvasiveCells, membraneDensity);
	dataPoints.push_back(dataPoint);
}

/*

Proliferates an abnormal cell

*/
void Model::ProliferateCell(AbnormalCell* cell) 
{
	//for random chance checks
	std::uniform_real_distribution<double> probability(0.0,1.0);

	int x = cell->GetXPos();
	int y = cell->GetYPos();

	//find neighboring grid cells that are empty or contain healthy cells
	std::pair<int,int> destination;
	std::vector<std::pair<int,int>> empty = {};
	std::vector<std::pair<int,int>> normal = {};
	for(int i = x-1; i <= x+1; i++)
	{
		for(int j = y-1; j <= y+1; j++)
		{
			int agentType = agentSpace[i][j]->GetAgentType();
			if(agentType == EMPTY) 
			{
				empty.push_back({i,j});
			}
			else if(agentType == NORMAL)
			{
				normal.push_back({i,j});
			}
		}
	}

	//empty location prioritized over competing with normal cell
	if(empty.size() != 0)
	{
		//pick a random empty grid cell for the new mutant cell
		std::shuffle(empty.begin(), empty.end(), generator);
		destination = empty[0];
	}
	//if no neighboring grid cells are empty, compete with a normal cell
	else if(normal.size() != 0)
	{
		double weakestFitness = 2; //fitness never over 1, so at least one lower will be found
		double cellFitness;
		Cell* weakestCell;
		Cell* normalCell;
		std::pair<int,int> weakestLocation;
		//find weakest normal cell
		for(auto location : normal)
		{
			normalCell = normalCellSpace[location.first][location.second];
			cellFitness = normalCell->GetFitness();
			if(cellFitness < weakestFitness)
			{
				weakestFitness = cellFitness;
				weakestCell = normalCell;
				weakestLocation = location;
			}
		}
		//if abnormal cell is weaker than weakest normal cell, proliferation does not occur
		if(cell->GetFitness() < weakestFitness)
		{
			return;
		}
		destination = weakestLocation;
		nNormalCells--;
	}
	//if no empty spaces or normal cells are available, proliferation does not occur
	else
	{
		return;
	}

	//mother cell age resets
	cell->SetAge(0);

	//number of divisions for mother cell decreases
	cell->SetDivisions(cell->GetDivisions() + 1);

	//
	//generate new abnormal cell
	//
	int xPos = destination.first;
	int yPos = destination.second;
	agentSpace[xPos][yPos]->SetAgentType(ABNORMAL);

	//# of divisions inherited from mother cell
	int divisions = cell->GetDivisions();

	//stickiness mutates from the mother cell, must stay within range [0,1]
	double stickiness = cell->GetStickiness() - stickinessMutationRate + 
		2*stickinessMutationRate*probability(generator);
	if(stickiness < 0 || stickiness > 1) {stickiness = cell->GetStickiness();}

	//jump rate mutates from mother cell, cannot be negative
	std::uniform_int_distribution<int> jumpDistribution(cell->GetJumpRate() - 
		jumpMutationRate, cell->GetJumpRate() + jumpMutationRate);
	int jumpRate = jumpDistribution(generator);
	if(jumpRate < 0) {jumpRate = 0;}

	//fitness is randomly between 0.5 and 1
	double fitness = 0.5 + 0.5*probability(generator);

	//age is of course 0
	int age = 0;
	
	AbnormalCell* newAbCell = new AbnormalCell(fitness, xPos, yPos, age, divisions, stickiness, jumpRate);
	abnormalCells.push_back(newAbCell);
	nAbnormalCells++;

	return;
}

/*

Moves an abnormal cell. Returns 1 if cell becomes invasive (membrane breached)

*/
void Model::MoveCell(AbnormalCell* cell) 
{
	//for random chance checks
	std::uniform_real_distribution<double> probability(0.0,1.0);

	int x = cell->GetXPos();
	int y = cell->GetYPos();
	int jump = cell->GetJumpRate();

	//ensure move will stay in domain
	if((x-jump >= 0) && (x+jump <= nColumns-1) && (y-jump >= 0) && (y+jump <= nRows-1))
	{
		//pick a random direction
		std::uniform_int_distribution<int> directionDistribution(0,3);
		int direction = directionDistribution(generator);

		//attempt to move
		std::vector<std::pair<int,int>> destinations = {{x-jump,y}, {x+jump,y}, {x,y-jump}, {x,y+jump}};
		std::pair<int,int> destination = destinations[direction];
		Agent* destinationAgent = agentSpace[destination.first][destination.second];
		switch(destinationAgent->GetAgentType())
		{
			//empty space
			case EMPTY:
				agentSpace[x][y]->SetAgentType(EMPTY);
				destinationAgent->SetAgentType(ABNORMAL);
				cell->SetXPos(destination.first);
				cell->SetYPos(destination.second);
				break;

			//membrane
			case MEMBRANE:
				//random chance of breaching membrane
				//if statement evaluates true if membrane NOT breached
				if(probability(generator) > membranePorosity)
				{
					break;
				}
				destinationAgent->SetAgentType(BREACHEDMEMBRANE);
				membraneDensity-=membraneDensityPerCell;
				
			//breached membrane
			case BREACHEDMEMBRANE:
				agentSpace[x][y]->SetAgentType(EMPTY);
				nAbnormalCells--;
				nInvasiveCells++;
				if(nInvasiveCells==1) {firstCellExitTime = time;}
				if(nInvasiveCells==100) {hundredCellExitTime = time;}
				break;
		}
	} 
	return;
}

/*

Generate extracellular matrix membrane, which forms the "outer border" of the simulation space

*/
void Model::GenerateMembranes()
{
	
	//first and last row are all membrane
	for(int i = 0; i < nColumns; i++)
	{
		//first row
		agentSpace[0][i]->SetAgentType(MEMBRANE);
	
		//last row
		agentSpace[nRows-1][i]->SetAgentType(MEMBRANE);
	}

	//left and right border for every other row
	for(int i = 1; i < nRows - 1; i++)
	{
		//left border
		agentSpace[i][0]->SetAgentType(MEMBRANE);

		//last row
		agentSpace[i][nColumns-1]->SetAgentType(MEMBRANE);

	}
	int nMembraneCells = 2*nRows + 2*(nColumns-2);
	membraneDensityPerCell = 1/static_cast<double>(nMembraneCells);
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
			nNormalCells += 1;
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
	
	std::uniform_real_distribution<double> distribution(0.0, 0.5);
	double fitnessInitial = 0.5 + distribution(generator);
	int ageInitial = 0;
	int divisionsInitial = 0;
	double stickinessInitial = 0.9;
	int jumpRateInitial = 1;

	//create cell
	agentSpace[xPos][yPos]->SetAgentType(ABNORMAL);
	AbnormalCell* mutatedCell = new AbnormalCell(fitnessInitial, xPos, 
		yPos, ageInitial, divisionsInitial, stickinessInitial, 
		jumpRateInitial);
	abnormalCells.push_back(mutatedCell);
	nAbnormalCells++;
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
	std::vector<std::pair<int,int>> emptyCells = {};

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

	//place proteins
	for(int i = 1; i < nProteins; i++)
	{
		agentSpace[emptyCells[i].first][emptyCells[i].second]->SetAgentType(PROTEIN);
	}

}

double Model::GetTimeSinceStart()
{
	tEnd = std::chrono::steady_clock::now();
	return (double) std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count() / 1000;
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

void Model::AbTest()
{
	for(int i = 0; i < nAbnormalCells; i++)
	{
		std::cout << "\n----abnormal cell " << i+1 << "----\n";
		std::cout << "x: " << abnormalCells[i]->GetXPos() << "\n";
		std::cout << "y: " << abnormalCells[i]->GetYPos() << "\n";
		std::cout << "fitness: " << abnormalCells[i]->GetFitness() << "\n";
		std::cout << "age: " << abnormalCells[i]->GetAge() << "\n";
		std::cout << "divisions: " << abnormalCells[i]->GetDivisions() << "\n";
		std::cout << "stickiness: " << abnormalCells[i]->GetStickiness() << "\n";
		std::cout << "jumpRate: " << abnormalCells[i]->GetJumpRate() << "\n";
	}
}