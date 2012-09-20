/*
 * coordinate.h
 *
 *  Created on: Sep 20, 2012
 *      Author: BONET
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <cmath>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

typedef struct coordinate Coordinate;

struct coordinate {
	int row;
	int column;

	coordinate(int, int);

	coordinate();

	int rowDistance(Coordinate);

	int columnDistance(Coordinate);

	bool isDiagonalNeighbor(Coordinate);

};

#endif /* COORDINATE_H_ */
