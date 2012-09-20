/*
 * piece.h
 *
 *  Created on: Sep 20, 2012
 *      Author: BONET
 */

#ifndef PIECE_H_
#define PIECE_H_

typedef struct piece Piece;

struct piece {
	int line, column;
	char pieceType;

	piece(int, int, char);

};

#endif /* PIECE_H_ */
