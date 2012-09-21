/*
 * Coordinate.cpp
 *
 *  Created on: Sep 19, 2012
 *      Author: BONET
 */
#include "coordinate.h"
using namespace std;

coordinate::coordinate(int row, int column) {
	this->row = row;
	this->column = column;
}

coordinate::coordinate() {
	this->column = -1;
	this->row = -1;
}

coordinate::coordinate(int tile[]) {
	this->column = tile[1];
	this->row = tile[0];
}

int coordinate::rowDistance(Coordinate anotherCoordinate) {
	return abs(this->row - anotherCoordinate.row);
}

int coordinate::columnDistance(Coordinate anotherCoordinate) {
	return abs(this->column - anotherCoordinate.column);
}

bool coordinate::isDiagonalNeighbor(Coordinate anotherCoordinate) {
	int r = this->rowDistance(anotherCoordinate);
	int c = this->columnDistance(anotherCoordinate);
	return r == 1 && c == 1;
}

bool coordinate::operator==(const Coordinate &other) const {
	return other.column == this->column && other.row == this->row;
}

bool coordinate::operator!=(const Coordinate &other) const {
	return !(*this == other);
}

