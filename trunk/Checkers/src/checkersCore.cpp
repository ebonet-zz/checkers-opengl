#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLACK_KING 'B'
#define BLACK_PAWN 'b'
#define RED_KING 'R'
#define RED_PAWN 'r'
#define EMPTY_CELL ' '
#define VOID_CEL5 '#'

const char initialBoardState[8][8]= {{'#','b','#','b','#','b','#','b'},
		  {'b','#','b','#','b','#','b','#'},
		  {'#','b','#','b','#','b','#','b'},
		  {' ','#',' ','#',' ','#',' ','#'},
		  {'#',' ','#',' ','#',' ','#',' '},
		  {'w','#','w','#','w','#','w','#'},
		  {'#','w','#','w','#','w','#','w'},
		  {'w','#','w','#','w','#','w','#'}};

char * currentBoardState;
/**
 * Sets the board matrix to the one that represents the initial state of the game
 */
void initBoardState(){
	memcpy(currentBoardState,initialBoardState,64*sizeof(char));
}

/**
 *

Stack getPieceStack(){
	return NULL;
}*/


/**
 * Get Current State Board
 */

void getCurrentStateBoard(int oldBoard[8][8],int selectedPositionX,int selectedPositionY){

}

/**
 * isValid(old Coord, new Coord) -> checks if the movement from cell A to cell B is valid
 */

bool isValid(int oldX, int oldY, int newX, int newY){
	/**
	 * TODO: Implement this method
	 */

	return false;
}

/**
 * move:
 *
 *  moves a piece from point A to point B.
 * 	returns true if the action can be completed
 * 	returns false otherwise
 */
bool move(int oldX, int oldY, int newX, int newY){
	/**
	 * TODO: Implement this method
	 */

	return false;
}





