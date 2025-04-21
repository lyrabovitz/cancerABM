#include <iostream>
#include <chrono>

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
		std::chrono::duration_cast<std::chrono::milliseconds>
		(tEnd - tStart).count()<< "[ms]" << std::endl;
}

void Model::Initialize()
{
	//generate empty agent space and cell space
	Agent* nullAgent = new Agent(0);
	agents.push_back(nullAgent);
	Cell* nullCell = new Cell(0, 0);
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
	
	//generate healthy cells

	//generate initial mutation

	//generate extracellular matrix proteins

	//generate initial data point
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
	//nInvasiveCells and membraneDensity

	//increase time step and time
	tStep++;
	time += timePerStep;
	
}

void Model::ProliferateCell(AbnormalCell cell) 
{
	// TODO - implement Model::ProliferateCell
	throw "Not yet implemented";
}

void Model::MoveCell(AbnormalCell cell) 
{
	// TODO - implement Model::MoveCell
	throw "Not yet implemented";
}
