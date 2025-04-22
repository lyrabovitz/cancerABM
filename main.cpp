#include "Model.h"
#include "DataPoint.h"
#include "Agent.h"
#include "Cell.h"
#include "AbnormalCell.h"

#include <iostream>
#include <chrono>
#include <fstream>

int main() 
{
    //running multiple models for benchmarking
    std::vector<int> spaceScales = {102, 200, 500};
    std::vector<int> timeScales = {1800, 2400, 3600};
    int nSimsPerScale = 30;

    //output file
    std::ofstream bm("benchmark.csv");

    if (!bm.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    //first row is column labels
    bm << ",";
    for(int timeScale : timeScales)
    {
        bm << timeScale << ",";
    }
    bm << std::endl;

    for(int spaceScale : spaceScales)
    {   
        //row label
        bm << spaceScale << ",";

        for(int timeScale : timeScales)
        {
            double durationSum = 0;
            for(int i = 0; i < nSimsPerScale; i++)
            {
                Model model;

                //parameters
                model.nRows = spaceScale; //minimum 50
                model.nColumns = spaceScale;   //minimum 50
                model.maxCells = (model.nRows - 2) * (model.nColumns - 2);
                model.membranePorosity = 0.5;
                model.divisionAge = 20;
                model.stickinessMutationRate = 0.1;
                model.jumpMutationRate = 1.0;
                model.maxDivisions = 10;
                model.divisionRateAbnormal = 0.6;
                model.deathRateAbnormal = 0.001;
                model.proteinDensity = 0.1;
                model.maxNormalCellsPerRow = 0.4*model.nColumns;
                double timeTotal = 24;
                int nTimeSteps = timeScale;
                model.timePerStep = timeTotal / static_cast<double>(nTimeSteps);

                //initialize model (initial generation)
                model.Initialize();

                for(int i = 0; i < nTimeSteps; i++)
                {
                    model.AdvanceSimulation();
                }

                //output file
                std::ofstream out("data.csv");

                if (!out.is_open()) {
                    std::cerr << "Error opening file!" << std::endl;
                    return 1;
                }

                out << "Time, Normal Cells, Abnormal Cells, Invasive Cells, Average Stickiness, Average Jump Rate, Membrane Density" << std::endl;
                for(auto dp : model.GetDataPoints())
                {
                    out << dp->GetTime() << ",";
                    out << dp->GetNormalCells() << ",";
                    out << dp->GetAbnormalCells() << ",";
                    out << dp->GetInvasiveCells() << ",";
                    out << dp->GetAvgStickiness() << ",";
                    out << dp->GetAvgJumpRate() << ",";
                    out << dp->GetMembraneDensity() << ",";
                    out << std::endl;
                }

                out.close();

                durationSum += model.GetTimeSinceStart();
            }
            //average duration for this space and time scale
            bm << durationSum/nSimsPerScale << ",";
        }
        bm << std::endl;
    }




    bm.close();


    

    return 0;
}