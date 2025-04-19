#include <iostream>

#include "Model.h"

Model::Model()
{
	tStep = 1;
}

void Model::AdvanceSimulation() {
	// TODO - implement Model::AdvanceSimulation
	std::cout << tStep << "\n";
	tStep++;
}

void Model::ProliferateCell(AbnormalCell cell) {
	// TODO - implement Model::ProliferateCell
	throw "Not yet implemented";
}

void Model::MoveCell(AbnormalCell cell) {
	// TODO - implement Model::MoveCell
	throw "Not yet implemented";
}
