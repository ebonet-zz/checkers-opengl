#include <SFML/Window.hpp>

struct Board 
{
	void drawChessBoard(GLfloat X0, GLfloat Y0, GLfloat BOARD_PIECE_WIDTH , GLfloat BOARD_PIECE_HEIGHT, GLint NCOLS_BOARD, GLint NLINS_BOARD) {
		for (int col = 0; col < NCOLS_BOARD; col++) {
			GLfloat x = X0 + BOARD_PIECE_WIDTH * col;
			for (int lin = 0; lin < NLINS_BOARD; lin++) {
				GLfloat y = Y0 + BOARD_PIECE_HEIGHT * lin;

				// TODO: set names for picking

				glBegin(GL_POLYGON);
				{
					if ((col + lin) % 2 == 0)
						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					else
						glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
					glNormal3f(0, 0, 1);
					glVertex3f(x, y, 0.0f);
					glVertex3f(x + BOARD_PIECE_WIDTH, y, 0.0f);
					glVertex3f(x + BOARD_PIECE_WIDTH, y + BOARD_PIECE_HEIGHT, 0.0f);
					glVertex3f(x, y + BOARD_PIECE_HEIGHT, 0.0f);
				}
				glEnd();
			}
		}
	}

	/*WOLOLOLO*/

};
