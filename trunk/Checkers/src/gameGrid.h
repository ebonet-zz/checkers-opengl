/*
 * gameGrid.h
 *
 *  Created on: Sep 20, 2012
 *      Author: BONET
 */
#include "coordinate.h"
#ifndef GAMEGRID_H_
#define GAMEGRID_H_

struct GameGrid {

	char GameGridMatrix[8][8];

	GameGrid();

	void restart();

	char getCellAt(Coordinate);

	char getCellAt(int, int);

	bool isEmptyCell(Coordinate);

	bool isPiece(Coordinate);

	void movePiece(Coordinate, Coordinate);

	void removePieceAt(Coordinate);

	bool isEnemyPiece(Coordinate, Coordinate);

	void createGridFromFile(char *);

	void createInitialGrid();
};

#endif /* GAMEGRID_H_ */
