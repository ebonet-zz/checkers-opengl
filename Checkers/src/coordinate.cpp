/*


 * Coordinate.cpp
 *
 *  Created on: Sep 19, 2012
 *      Author: BONET
 */
#include "coordinate.h"
using namespace std;



int coordinate::rowDistance(Coordinate anotherCoordinate){
	return abs(this->row-anotherCoordinate.row);
}

int coordinate::columnDistance(Coordinate anotherCoordinate){
	return abs(this->column-anotherCoordinate.column);
}

bool coordinate::isDiagonalNeighbor(Coordinate anotherCoordinate){
	return this->rowDistance(anotherCoordinate)==1 && this->columnDistance(anotherCoordinate)==1;
}

