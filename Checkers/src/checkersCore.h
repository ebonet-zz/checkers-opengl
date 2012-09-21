/*
 * checkersCore.h
 *
 *  Created on: Sep 20, 2012
 *      Author: BONET
 */

#ifndef CHECKERSCORE_H_
#define CHECKERSCORE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <list>
#include "piece.h"
#include "coordinate.h"
#include "gameGrid.h"
#include "constants.h"
#include <iostream>

/**
 * Class CheckersCore
 *
 * This class is responsible to handle the logic behind the checkers game
 */

class CheckersCore {

public:
	CheckersCore();
	CheckersCore(char * fileName);

	bool makeMove(Coordinate, Coordinate);

	bool isThisMoveValid(Coordinate, Coordinate);

	bool isImmediateMove(Coordinate, Coordinate);

	bool isEatingMove(Coordinate, Coordinate);

	std::list<Coordinate> getPossibleMoves(Coordinate);

	void updateBoardState(int[8][8], Coordinate);

	void updateBoardState(int[8][8]);

	std::queue<piece> getPieces();

	int isGameWon(void);

	void restartGame(); //TODO: Implement this

	int getCurrentPlayer();

	bool isTileSelectable(Coordinate);

private:

	int average(int, int);

	void initializeGame();

	std::queue<piece> updatePawnsOnBoard();

	/**
	 * The current state of the game
	 */
	GameGrid CurrentGameState;

	/**
	 * The list of pawns that are still on board
	 */
	std::queue<piece> PawnsOnBoard;

	/**
	 * The player that is currently playing (0 for red, 1 for brown)
	 */
	int PlayerInTurn;
	int nPiecesOnBoard[2];
};

#endif /* CHECKERSCORE_H_ */
