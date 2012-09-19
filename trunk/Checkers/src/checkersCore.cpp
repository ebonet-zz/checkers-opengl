#include <stdlib.h>
#include <stdio.h>

#define BLACK_KING 'B'
#define BLACK_PAWN 'b'
#define WHITE_KING 'W'
#define WHITE_PAWN 'w'
#define EMPTY_CELL ' '
#define VOID_CEL5 '#'

/**
 *
 *
 * gameMatrix
gameMatrix[8][8] ->-2,-1,0,1,2

getPieceStack()
int[][] getBoard(selectedCoordinates)


operaçoes sobre a gameMatrix
+ isValid
+ move
 *
 */



char boardState[8][8];




/**
 * Sets the board matrix to the one that represents the initial state of the game
 */
void initBoardState(){

	boardState = {{'#','b','#','b','#','b','#','b'},
				  {'b','#','b','#','b','#','b','#'},
				  {'#','b','#','b','#','b','#','b'},
				  {' ','#',' ','#',' ','#',' ','#'},
				  {'#',' ','#',' ','#',' ','#',' '},
				  {'w','#','w','#','w','#','w','#'},
				  {'#','w','#','w','#','w','#','w'},
				  {'w','#','w','#','w','#','w','#'}};
}


