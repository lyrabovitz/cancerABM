#include <iostream>

#include "DataPoint.h"


ostream& operator<<(ostream& os, const DataPoint dp)
{
    os < time << "," << nNormalCells << "," << nAbnormalCells << ","
        << avgStickiness << "," << nInvasiveCells << "," 
        << membraneDensity << "," << std::endl;
}