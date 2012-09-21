/*
 * gameGrid.cpp
 *
 *  Created on: Sep 20, 2012
 *      Author: BONET
 */

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "gameGrid.h"
#include "constants.h"
#include "coordinate.h"
using namespace std;

GameGrid::GameGrid() {
	createInitialGrid();
}

void GameGrid::restart() {
	createInitialGrid();
}
char GameGrid::getCellAt(Coordinate A) {
	return GameGridMatrix[A.row][A.column];
}

char GameGrid::getCellAt(int row, int column) {
	return GameGridMatrix[row][column];
}

void GameGrid::makeKing(int row, int column) {
	if (GameGridMatrix[row][column] == 'b') {
		GameGridMatrix[row][column] = 'B';
	} else if (GameGridMatrix[row][column] == 'r') {
		GameGridMatrix[row][column] = 'R';
	}
}

bool GameGrid::isEmptyCell(Coordinate A) {
	bool r = getCellAt(A) == EMPTY_CELL;
	return r;
}

bool GameGrid::isPiece(Coordinate A) {
	return (getCellAt(A) == BEIGE_PAWN || getCellAt(A) == RED_PAWN);
}

bool GameGrid::isKing(Coordinate A) {
	return (getCellAt(A) == BEIGE_KING || getCellAt(A) == RED_KING);
}

void GameGrid::movePiece(Coordinate A, Coordinate B) {
	if (!isEmptyCell(B)) {
		cout << "Invalid movement: target is not empty" << endl;
		return;
	}
	if (!isPiece(A) && !isKing(A)) {
		cout << "Invalid movement: source is not a piece" << endl;
		return;
	}

	GameGridMatrix[B.row][B.column] = getCellAt(A);
	GameGridMatrix[A.row][A.column] = EMPTY_CELL;
}

void GameGrid::removePieceAt(Coordinate A) {
	if (getCellAt(A) == VOID_CELL) {
		cout << "Illegal move: tried to remove piece from void cell" << endl;
		return;
	}

	GameGridMatrix[A.row][A.column] = EMPTY_CELL;
}

bool GameGrid::isEnemyPiece(int currentPlayer, Coordinate B) {
	return (!currentPlayer && (getCellAt(B) == 'b' || getCellAt(B) == 'B'))
			|| (currentPlayer && (getCellAt(B) == 'r' || getCellAt(B) == 'R'));
}

void GameGrid::createGridFromFile(char * s) {
	FILE * pFile;
	int c;
	int row = 0, column = 0;

	pFile = fopen(s, "r");

	if (pFile == NULL)
		perror("Error opening file");
	else {
		do {
			c = fgetc(pFile);
			if (c == '\n') {
				row++;
				column = 0;
				continue;
			}
			GameGridMatrix[row][column] = c;
			column++;

		} while (c != EOF);
		fclose (pFile);
	}

	return;
}

void GameGrid::createInitialGrid() {
	createGridFromFile("InitialGameGrid.txt");
}

