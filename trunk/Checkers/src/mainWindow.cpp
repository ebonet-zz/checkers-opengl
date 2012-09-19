#include <stdlib.h>
#include <stdio.h>
#include <SFML/Window.hpp>
#include <stack>
#include "piece.cpp"

#define ROUND_PRECISION 50
#define CIRC 2 * M_PI
#define DTHETA CIRC / ROUND_PRECISION
#define WINDOW_SIZE 900
#define NCOLS_BOARD 8
#define NLINS_BOARD 8
#define X0 -50.0f
#define X1 50.0f
#define Y0 50.0f
#define Y1 -50.0f

GLfloat boardSquareWidth = (X1 - X0) / NCOLS_BOARD;
GLfloat boardSquareHeight = (Y1 - Y0) / NLINS_BOARD;
GLfloat pieceHeight = 2.5;
GLfloat pieceWidth = boardSquareWidth / 2 - 1;

const GLfloat WHITE[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat GREY[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat LIGHT_GREEN[4] = { 0.0f, 0.4f, 0.0f, 1.0f };
const GLfloat RED[4] = { 0.4f, 0.0f, 0.0f, 1.0f };
const GLfloat BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat BEIGE[4] = { 0.3f, 0.3f, 0.1f, 1.0f };

GLfloat ratio = 1.0f;
GLfloat eyePositionModifier = 1.0f;
GLfloat eyePositionX = -100.0f;
GLfloat eyePositionY = 0.0f;
GLfloat eyePositionZ = 80.0f;

GLfloat AMB[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DIF[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat ESP[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat POS[2][4] = { { 50.0f, 50.0f, 100.0f, 1.0f }, { -50.0f, -50.0f, 20.0f, 1.0f } };

GLfloat boardColor[8][8][4];

int lastSelectedCoordinates[2];
int lastHitCount = 0;

std::stack<piece> pieceStack;

void initBoardColors() {
	for (int col = 0; col < 8; col++) {
		for (int lin = 0; lin < 8; lin++) {
			if ((col + lin) % 2 == 0) {
				memcpy(boardColor[col][lin], WHITE, 4 * sizeof(GLfloat));
			} else {
				memcpy(boardColor[col][lin], GREY, 4 * sizeof(GLfloat));
			}
		}
	}
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AMB);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	for (int i = 0; i < 2; i++) {
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, AMB);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, DIF);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, ESP);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, POS[i]);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, AMB);
	glMateriali(GL_FRONT, GL_SHININESS, 7);

	initBoardColors();
}

void drawBoard(GLenum mode) {
	for (int col = 0; col < 8; col++) {
		GLfloat x = X0 + boardSquareWidth * col;
		if (mode == GL_SELECT) {
			glLoadName(col);
		}
		for (int lin = 0; lin < 8; lin++) {
			GLfloat y = Y0 + boardSquareHeight * lin;
			if (mode == GL_SELECT) {
				glPushName(lin);
			}
			glBegin(GL_POLYGON);
			{
				glColor4fv(boardColor[col][lin]);
				glNormal3f(0, 0, -1);
				glVertex3f(x, y, 0.0f);
				glVertex3f(x + boardSquareWidth, y, 0.0f);
				glVertex3f(x + boardSquareWidth, y + boardSquareHeight, 0.0f);
				glVertex3f(x, y + boardSquareHeight, 0.0f);
			}
			glEnd();
			if (mode == GL_SELECT) {
				glPopName();
			}
		}
	}
}

void drawPiece(piece p) {
	glPushMatrix();

	if (p.pieceType == 'r' || p.pieceType == 'R') {
		glColor4fv(RED);
	} else {
		glColor4fv(BEIGE);
	}

	glTranslatef(X0 + boardSquareWidth * p.column, Y0 + boardSquareHeight * p.line, 0);
	glTranslatef(boardSquareWidth / 2, boardSquareHeight / 2, 0);

	gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
	gluCylinder(gluNewQuadric(), pieceWidth, pieceWidth, pieceHeight, ROUND_PRECISION, ROUND_PRECISION);
	glTranslatef(0, 0, pieceHeight);
	gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
	glPopMatrix();
}

void display(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eyePositionX / eyePositionModifier, eyePositionY / eyePositionModifier,
			eyePositionZ / eyePositionModifier, 0, 0, 0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBoard(GL_RENDER);

	glFlush();
}

void setWindowSize(unsigned int w, unsigned int h) {
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ratio = (GLfloat) w / (GLfloat) h;
	gluPerspective(60, ratio, 0.5, 500.0);
}

/*  processHits prints out the contents of the selection array.  */
void processHits(GLint hits, GLuint buffer[]) {

	// Method extracted from the example. Won't touch it because its ugly.
	// I'm gonna treat it like a black box.
	GLuint names, *bufferIntegerPointer;

	printf("hits = %d\n", hits);
	lastHitCount = hits;
	bufferIntegerPointer = (GLuint *) buffer;
	for (int i = 0; i < hits; i++) { /*  for each hit  */
		names = *bufferIntegerPointer;
		//printf(" number of names for this hit = %d\n", names);
		bufferIntegerPointer++;
		//printf("  z1 is %g;", (float) *ptr / 0x7fffffff);
		bufferIntegerPointer++;
		//printf(" z2 is %g\n", (float) *bufferIntegerPointer / 0x7fffffff);
		bufferIntegerPointer++;
		printf("   names are ");

		for (GLuint j = 0; j < names; j++) { /*  for each name */
			lastSelectedCoordinates[j] = *bufferIntegerPointer;
			printf("%d ", *bufferIntegerPointer);
			bufferIntegerPointer++;
		}
		printf("\n");
	}
}

void pickSquares(int x, int y) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLuint selectBuf[WINDOW_SIZE];
	glSelectBuffer(WINDOW_SIZE, selectBuf);

	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	/* 	 create 5x5 pixel picking region near cursor location	*/
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);

	gluPerspective(60, ratio, 0.5, 500.0);
	drawBoard(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void lightSquare(int col, int lin, const GLfloat color[4]) {
	memcpy(boardColor[col][lin], color, 4 * sizeof(GLfloat));
}

void restoreSquare(int col, int lin) {
	if ((col + lin) % 2 == 0) {
		memcpy(boardColor[col][lin], WHITE, 4 * sizeof(GLfloat));
	} else {
		memcpy(boardColor[col][lin], GREY, 4 * sizeof(GLfloat));
	}
}

class GLBox {
public:
	GLBox() {
		App = new sf::Window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE, 32), "Click me!");

		setWindowSize(WINDOW_SIZE, WINDOW_SIZE);
		init();
		while (App->IsOpened()) {
			App->SetActive();

			sf::Event Event;
			while (App->GetEvent(Event)) {
				// Close window : exit
				if (Event.Type == sf::Event::Closed)
					App->Close();

				// Escape key : exit
				if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
					App->Close();

				if (Event.Type == sf::Event::MouseButtonPressed) {

					//TODO: This is a Test
					initBoardColors();
					pickSquares(Event.MouseButton.X, Event.MouseButton.Y);
					if (lastHitCount != 0) {
						lightSquare(lastSelectedCoordinates[0], lastSelectedCoordinates[1], LIGHT_GREEN);
					} else {
						initBoardColors();
					}
				}

				if (Event.Type == sf::Event::Resized) {
					setWindowSize(Event.Size.Width, Event.Size.Height);
				}
			}

			display();

			//TODO: This is a Test
			piece p(3, 3, 'r');
			piece p2(1, 1, 'b');
			drawPiece(p);
			drawPiece(p2);

			App->Display();
		}
	}

private:
	sf::Window *App;
};

int main(int argc, char **argv) {
	init();
	GLBox prog;
	return EXIT_SUCCESS;
}
