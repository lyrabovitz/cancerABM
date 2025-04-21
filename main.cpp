#include "Model.h"
#include "DataPoint.h"
#include "Agent.h"
#include "Cell.h"
#include "AbnormalCell.h"
#include <iostream>
#include <chrono>

int main() 
{


    Model model;

    //parameters
    model.maxCells = 100*100;
    model.membranePorosity = 0.5;
    model.divisionAge = 20;
    model.stickinessMutationRate = 0.1;
    model.jumpMutationRate = 1.0;
    model.maxDivisions = 10;
	model.divisionRateAbnormal = 0.6;
	model.deathRateAbnormal = 0.001;
    model.proteinDensity = 0.1;
	int maxNormalCellsPerColumn = 40;
    model.maxNormalCells = 81 * maxNormalCellsPerColumn;
	double timeTotal = 24;
	int nTimeSteps = 1800;
    model.timePerStep = timeTotal / static_cast<double>(nTimeSteps);
    model.nRows = 100;
    model.nColumns = 100;
    
    //initialize model (initial generation)
    model.Initialize();

    //set up output file

    for(int i = 0; i < nTimeSteps; i++)
    {
        model.AdvanceSimulation();
    }

    //export final things to output file

    return 0;
}