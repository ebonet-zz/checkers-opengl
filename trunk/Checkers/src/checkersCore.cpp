#include "checkersCore.h"

using namespace std;

int CheckersCore::average(int someNumber, int anotherNumber) {
	return (someNumber + anotherNumber) / 2;
}

/**
 * Reinitializes the game
 */

void CheckersCore::restartGame() {
	this->initializeGame();
}

/**
 * Creates the game
 */
CheckersCore::CheckersCore() {
	PlayerInTurn = 1;
	nPiecesOnBoard[0] = nPiecesOnBoard[1] = 0;

	//CurrentGameState.createGridFromFile("TestGameGrid.txt");
	CurrentGameState.createInitialGrid();
	PawnsOnBoard = this->updatePawnsOnBoard();
}

CheckersCore::CheckersCore(char * fileName) {
	PlayerInTurn = 1;
	nPiecesOnBoard[0] = nPiecesOnBoard[1] = 0;
	CurrentGameState.createGridFromFile(fileName);
}

void CheckersCore::initializeGame() {
	PlayerInTurn = 1;
	nPiecesOnBoard[0] = nPiecesOnBoard[1] = 0;
	CurrentGameState.createInitialGrid();
	PawnsOnBoard = updatePawnsOnBoard();
}

/**
 * Checks if the game has ended.
 * If the function return 1 the red player won, if 2 the beige player won.
 * If 0 the game is still no finished
 */
int CheckersCore::isGameWon(void) {
	if (!nPiecesOnBoard[BEIGE_PLAYER])
		return 1;
	if (!nPiecesOnBoard[RED_PLAYER])
		return 2;
	return 0;
}

/**
 * Return the player the must do the next move
 */
int CheckersCore::getCurrentPlayer(void) {
	return this->PlayerInTurn;
}

/**
 * Completes a player turn. If the move is invalid, returns false.
 *
 * If the move is valid, performs the actions needed and allows the other player to play
 */
bool CheckersCore::makeMove(Coordinate oldCoordinate, Coordinate newCoordinate) {

	// checks if the move is valid
	if (!isThisMoveValid(oldCoordinate, newCoordinate))
		return false;

	bool ate = false;
	if (isEatingMove(oldCoordinate, newCoordinate)) {
		ate = true;
	}

	// moves the piece
	CurrentGameState.movePiece(oldCoordinate, newCoordinate);

	// removes a piece from the board if that's the case
	if (abs(oldCoordinate.row - newCoordinate.row) == 2) {

		Coordinate pieceToBeRemoved;
		pieceToBeRemoved.column = average(oldCoordinate.column, newCoordinate.column);
		pieceToBeRemoved.row = average(oldCoordinate.row, newCoordinate.row);

		CurrentGameState.removePieceAt(pieceToBeRemoved);

		nPiecesOnBoard[!this->PlayerInTurn] -= 1;
	}

	list<Coordinate> possibleMoves;

	if (ate) {
		int row, column;
		for (row = 0; row < 8; row++) {
			for (column = 0; column < 8; column++) {
				Coordinate currentCoordinate(row, column);
				if (isEatingMove(newCoordinate, currentCoordinate)) { // All first level eating moves
					possibleMoves.push_back(currentCoordinate);
				}
			}
		}
	}

	if (possibleMoves.empty()) {
		// gives the turn to the next player
		this->PlayerInTurn = !PlayerInTurn;
	}

	PawnsOnBoard = updatePawnsOnBoard();

	return true;
}

/**
 * Returns a stack containing the pawns that are currently on board
 */
std::queue<piece> CheckersCore::getPieces() {
	return this->PawnsOnBoard;
}

/**
 * PRIVATE METHOD
 *
 * updates the list of pawns that are still in the board
 *
 */
std::queue<piece> CheckersCore::updatePawnsOnBoard() {
	std::queue<piece> newPawns;

	int row, column;

	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			if (CurrentGameState.isPiece(Coordinate(row, column))) {
				newPawns.push(piece(row, column, CurrentGameState.getCellAt(Coordinate(row, column))));
			}
		}
	}

	return newPawns;
}

/**
 * Updates the matrix that describes the state of the board tiles
 *
 * If there's  no tile is passed as argument the initial board style is used
 */
void CheckersCore::updateBoardState(int oldState[8][8], Coordinate SelectedTile) {
	int row, column;

	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			if (CurrentGameState.getCellAt(row, column) == VOID_CELL)
				oldState[row][column] = -1;

			else if (isThisMoveValid(SelectedTile, Coordinate(row, column))) {
				oldState[row][column] = 1;
			}

			else if (CurrentGameState.isEmptyCell(Coordinate(row, column))) {
				oldState[row][column] = 0;
			}
		}

	}

	oldState[SelectedTile.row][SelectedTile.column] = 2;

}

void CheckersCore::updateBoardState(int oldState[8][8]) {
	int row, column;

	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			oldState[row][column] = (row + column) % 2 - 1;
		}
	}
}

std::list<Coordinate> CheckersCore::getPossibleMoves(Coordinate SelectedTile) {
	int row, column;
	std::list<Coordinate> valids;
	for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			Coordinate currentCoordinate(row, column);
			if (isThisMoveValid(SelectedTile, currentCoordinate)) {
				valids.push_back(currentCoordinate);
			}
		}
	}
	return valids;
}

/**
 * Checks if a move is valid or not
 */

bool CheckersCore::isThisMoveValid(Coordinate oldCoordinate, Coordinate newCoordinate) {

	if (isImmediateMove(oldCoordinate, newCoordinate)) {
		cout << "Origin: " << oldCoordinate.row << "," << oldCoordinate.column << endl;
		cout << "Target: " << newCoordinate.row << "," << newCoordinate.column << endl;
		cout << "Valid immediate move: The target coordinate is empty" << endl;
		return true;
	}

	if (isEatingMove(oldCoordinate, newCoordinate)) {
		cout << "Origin: " << oldCoordinate.row << "," << oldCoordinate.column << endl;
		cout << "Target: " << newCoordinate.row << "," << newCoordinate.column << endl;
		cout << "Valid eating move: The target coordinate is empty" << endl;
		return true;
	}

	return false;
}

bool CheckersCore::isEatingMove(Coordinate oldCoordinate, Coordinate newCoordinate) {
	if (oldCoordinate.row < 0 || oldCoordinate.row > 7 || oldCoordinate.column < 0 || oldCoordinate.column > 7
			|| newCoordinate.row < 0 || newCoordinate.row > 7 || newCoordinate.column < 0 || newCoordinate.column > 7) {
		return false;
	}

	// if there's a piece the move is invalid
	if (!CurrentGameState.isEmptyCell(newCoordinate)) {
		//cout << "Invalid move: The target coordinate is not empty" << endl;
		return false;
	}

	// The last valid movement happens when an enemy piece is being killed
	if (abs(oldCoordinate.row - newCoordinate.row) == 2 && abs(oldCoordinate.row - newCoordinate.row) == 2) {
		//cout << "Testing if jumping is valid" << endl;
		Coordinate midPoint;

		midPoint.row = average(oldCoordinate.row, newCoordinate.row);
		midPoint.column = average(oldCoordinate.column, newCoordinate.column);

		return CurrentGameState.isEnemyPiece(getCurrentPlayer(), midPoint);
	}

	return false;
}

bool CheckersCore::isImmediateMove(Coordinate oldCoordinate, Coordinate newCoordinate) {
	if (oldCoordinate.row < 0 || oldCoordinate.row > 7 || oldCoordinate.column < 0 || oldCoordinate.column > 7
			|| newCoordinate.row < 0 || newCoordinate.row > 7 || newCoordinate.column < 0 || newCoordinate.column > 7) {
		return false;
	}

	// if there's a piece the move is invalid
	if (!CurrentGameState.isEmptyCell(newCoordinate)) {
		//cout << "Invalid move: The target coordinate is not empty" << endl;
		return false;
	}

	// if the coordinate is a neighbor empty space the move is valid
	if (oldCoordinate.isDiagonalNeighbor(newCoordinate)) {
		//cout << "Valid move: The target coordinate is empty" << endl;
		return true;
	}

	return false;
}

bool CheckersCore::isTileSelectable(Coordinate A) {
	return (PlayerInTurn && (CurrentGameState.getCellAt(A) == 'b'))
			|| (!PlayerInTurn && (CurrentGameState.getCellAt(A) == 'r'));
}
