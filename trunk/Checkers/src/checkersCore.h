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
#include <stack>
#include <cmath>
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


class CheckersCore{

public:
	CheckersCore();
	CheckersCore(char * fileName);

	bool makeMove(Coordinate, Coordinate);

	bool isThisMoveValid(Coordinate,Coordinate);

	void updateBoardState(int [8][8],Coordinate);

	void updateBoardState(int [8][8]);

	std::stack<piece> getPieces();

	int isGameWon(void);

	void restartGame();//TODO: Implement this

	int getCurrentPlayer();

private:

	int average(int,int);

	void initializeGame();

	std::stack<piece> updatePawnsOnBoard();


	/**
	 * The current state of the game
	 */
	GameGrid CurrentGameState;

	/**
	 * The list of pawns that are still on board
	 */
	std::stack<piece> PawnsOnBoard;

	/**
	 * The player that is currently playing (0 for red, 1 for brown)
	 */
	int PlayerInTurn;
	int nPiecesOnBoard[2];
};



#endif /* CHECKERSCORE_H_ */
