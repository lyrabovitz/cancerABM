#include "Model.h"
#include <iostream>

int main() 
{
    Model model;
    for(int i = 0; i < 10; i++)
    {
        model.AdvanceSimulation();
    }
    return 0;
}