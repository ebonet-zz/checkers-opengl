#include <cmath>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

/*


 * Coordinate.cpp
 *
 *  Created on: Sep 19, 2012
 *      Author: BONET
 */




#define BEIGE_PAWN 'b'
#define RED_PAWN 'r'
#define EMPTY_CELL ' '
#define VOID_CELL '#'

typedef struct coordinate Coordinate;

struct coordinate{
	int row;
	int column;


	coordinate(int theRow,int theColumn){
		row = theRow;
		column=theColumn;
	}

	coordinate(){
		row = -1;
		column=-1;
	}

	int rowDistance(Coordinate anotherCoordinate){
		return abs(this->row-anotherCoordinate.row);
	}

	int columnDistance(Coordinate anotherCoordinate){
		return abs(this->column-anotherCoordinate.column);
	}

	bool isDiagonalNeighbor(Coordinate anotherCoordinate){
		return this->rowDistance(anotherCoordinate)==1 && this->columnDistance(anotherCoordinate)==1;
	}

};

struct GameGrid{

	char GameGridMatrix[8][8];

	GameGrid(){
		createInitialGrid();
	}

	void restart(){
		createInitialGrid();
	}
	char getCellAt(Coordinate A){
		return GameGridMatrix[A.row][A.column];
	}

	char getCellAt(int row,int column){
		return GameGridMatrix[row][column];
	}

	bool isEmptyCell(Coordinate A){
		return getCellAt(A) == EMPTY_CELL;
	}

	bool isPiece(Coordinate A){
		return (getCellAt(A)==BEIGE_PAWN || getCellAt(A)==RED_PAWN);
	}

	void movePiece(Coordinate A, Coordinate B){
		if(!isEmptyCell(B)){
			cout<< "Invalid movement: target is not empty" <<endl;
			return;
		}
		if(!isPiece(A)){
			cout<< "Invalid movement: source is not a piece" <<endl;
			return;
		}

		GameGridMatrix[B.row][B.column] = getCellAt(A);
		GameGridMatrix[A.row][A.column] = EMPTY_CELL;
	}

	void removePieceAt(Coordinate A){
		if(getCellAt(A)==VOID_CELL){
			cout << "Illegal move: tried to remove piece from void cell" <<endl;
			return;
		}

		GameGridMatrix[A.row][A.column] = EMPTY_CELL;
	}



	bool isEnemyPiece(Coordinate A, Coordinate B){
		return (getCellAt(A) =='r' && getCellAt(B) == 'b') || (getCellAt(A) =='b' && getCellAt(B) == 'r');
	}

	void createGridFromFile(char * s){
		FILE * pFile;
		int c;
		int row = 0,column=0;

		pFile=fopen (s,"r");

		if (pFile==NULL) perror ("Error opening file");
		else
		{
			do {
				c = fgetc (pFile);
				if(c=='\n'){
					row++;
					column=0;
					continue;
				}
				GameGridMatrix[row][column]=c;
				column++;

			} while (c != EOF);
			fclose (pFile);
		}

		return ;
	}

	void createInitialGrid(){
		createGridFromFile("InitialGameGrid.txt");
	}


};

