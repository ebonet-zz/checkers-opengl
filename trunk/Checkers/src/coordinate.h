/*
 * coordinate.h
 *
 *  Created on: Sep 20, 2012
 *      Author: BONET
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <cmath>

typedef struct coordinate Coordinate;

struct coordinate {
	int row;
	int column;

	coordinate(int, int);

	coordinate();

	coordinate(int []);

	int rowDistance(Coordinate);

	int columnDistance(Coordinate);

	bool isDiagonalNeighbor(Coordinate);

};

#endif /* COORDINATE_H_ */
