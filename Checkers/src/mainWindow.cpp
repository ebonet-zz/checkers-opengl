#include <stdlib.h>
#include <stdio.h>
#include <SFML/Window.hpp>
#include <stack>
#include "piece.cpp"

// Defines and constants
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
#define BUTTON_1_NAME 57
#define BUTTON_2_NAME 58
#define RIGHT 1
#define LEFT -1
#define UP 1
#define DOWN -1

// Colors
const GLfloat WHITE[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat GREY[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat LIGHT_GREEN[4] = { 0.0f, 0.4f, 0.0f, 1.0f };
const GLfloat RED[4] = { 0.35f, 0.0f, 0.0f, 1.0f };
const GLfloat BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat BEIGE[4] = { 0.3f, 0.3f, 0.1f, 1.0f };
const GLfloat BUTTON_1[4] = { 0.35f, 0.0f, 0.0f, 1.0f };
const GLfloat BUTTON_2[4] = { 0.3f, 0.3f, 0.3f, 1.0f };

// Camera and positioning
GLfloat ratio = 1.0f;
GLfloat eyePositionModifier = 1.0f;
GLfloat eyePositionX = -100.0f;
GLfloat eyePositionY = 0.0f;
GLfloat eyePositionZ = 80.0f;
GLfloat leftRightRotationAngle = 0;
GLfloat upDownRotationAngle = 0;
GLfloat rotationSpeed = 1;

// Lights
GLfloat AMB[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DIF[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat ESP[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat POS[2][4] = { { 50.0f, 50.0f, 100.0f, 1.0f }, { -50.0f, -50.0f, 20.0f, 1.0f } };

// Board and piece dimensions
GLfloat boardTileWidth = (X1 - X0) / NCOLS_BOARD;
GLfloat boardTileHeight = (Y1 - Y0) / NLINS_BOARD;
GLfloat pieceHeight = 2.5;
GLfloat pieceWidth = boardTileWidth / 2 - 1;

// Board color state
GLfloat boardColor[8][8][4];

// Mouse click selection results
int lastSelectedCoordinates[2];
int lastHitCount = 0;

// Piece stack from the core
std::stack<piece> pieceStack;

/**
 * Resets the black/white color pattern for all the board tile
 */
void initBoardColors() {
	for (int col = 0; col < 8; col++) {
		for (int lin = 0; lin < 8; lin++) {
			if ((col + lin) % 2 == 0) {
				memcpy(boardColor[col][lin], WHITE, 4 * sizeof(GLfloat));
			} else {
				memcpy(boardColor[col][lin], BEIGE, 4 * sizeof(GLfloat));
			}
		}
	}
}

/**
 * Initializes OpenGL light and depth configurations for the scene.
 * Also initiates the board state.
 */
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

/**
 * Reads the current board color state and draws the alternated squares.
 * Naming happens here in order to allow picking with GL_SELECT render mode.
 *
 * Param mode - The current OpenGl rendering mode.
 */
void drawBoard(GLenum mode) {
	for (int col = 0; col < 8; col++) {
		GLfloat x = X0 + boardTileWidth * col;
		if (mode == GL_SELECT) {
			glLoadName(col);
		}
		for (int lin = 0; lin < 8; lin++) {
			GLfloat y = Y0 + boardTileHeight * lin;
			if (mode == GL_SELECT) {
				glPushName(lin);
			}
			glBegin(GL_POLYGON);
			{
				glColor4fv(boardColor[col][lin]);
				glNormal3f(0, 0, -1);
				glVertex3f(x, y, 0.0f);
				glVertex3f(x + boardTileWidth, y, 0.0f);
				glVertex3f(x + boardTileWidth, y + boardTileHeight, 0.0f);
				glVertex3f(x, y + boardTileHeight, 0.0f);
			}
			glEnd();
			if (mode == GL_SELECT) {
				glPopName();
			}
		}
	}
}

/**
 * Receives a piece and draws it within the appropriate position and color.
 *
 * Param piece - The piece to draw
 */
void drawPiece(piece p) {
	glPushMatrix();

	if (p.pieceType == 'r' || p.pieceType == 'R') {
		glColor4fv(RED);
	} else {
		glColor4fv(GREY);
	}

	glTranslatef(X0 + boardTileWidth * p.column, Y0 + boardTileHeight * p.line, 0.1);
	glTranslatef(boardTileWidth / 2, boardTileHeight / 2, 0);

	gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
	gluCylinder(gluNewQuadric(), pieceWidth, pieceWidth, pieceHeight, ROUND_PRECISION, ROUND_PRECISION);
	glTranslatef(0, 0, pieceHeight);
	gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
	glPopMatrix();
}

void drawMenuButtons(GLenum mode) {
	//TODO:
}

/**
 * Main drawing function. Renders the game state including the board and pieces.
 */
void display(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eyePositionX / eyePositionModifier, eyePositionY / eyePositionModifier,
			eyePositionZ / eyePositionModifier, 0, 0, 0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(leftRightRotationAngle, 0, 0, 1);

	glRotatef(upDownRotationAngle, 0, 1, 0);

	drawBoard(GL_RENDER);

	//TODO: This is a Test
	piece p(3, 3, 'r');
	piece p2(1, 1, 'b');
	drawPiece(p);
	drawPiece(p2);

	drawMenuButtons(GL_RENDER);

	glFlush();
}

/**
 * Handles window resizing
 */
void setWindowSize(unsigned int w, unsigned int h) {
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ratio = (GLfloat) w / (GLfloat) h;
	gluPerspective(60, ratio, 0.5, 500.0);
}

/**
 * Reads the selection buffer and saves the selection on the proper global variables.
 * Method extracted from the example. Won't touch it too much because its ugly.
 * I'm gonna try to treat it like a black box.
 */
void processHits(GLint hits, GLuint buffer[]) {
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

/**
 * Translates the mouse click coordinates into an element selection using OpenGl GL_SELECT render mode.
 */
void pickTile(int x, int y) {
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
	drawMenuButtons(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

/**
 * Lights up one of the board tiles to the desired new color
 */
void lightBoardTile(int col, int lin, const GLfloat color[4]) {
	memcpy(boardColor[col][lin], color, 4 * sizeof(GLfloat));
}

/**
 * Brings the board tile to its default color.
 */
void restoreBoardTile(int col, int lin) {
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
					pickTile(Event.MouseButton.X, Event.MouseButton.Y);
					if (lastHitCount != 0) {
						lightBoardTile(lastSelectedCoordinates[0], lastSelectedCoordinates[1], LIGHT_GREEN);
					} else {
						initBoardColors();
					}

				}

				if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Right)) {
					handleHorizontalCameraMove(RIGHT);
				}

				if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Left)) {
					handleHorizontalCameraMove(LEFT);
				}

				if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) {
					handleVerticalCameraMove(UP);
				}

				if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) {
					handleVerticalCameraMove(DOWN);
				}

				if (Event.Type == sf::Event::Resized) {
					setWindowSize(Event.Size.Width, Event.Size.Height);
				}
			}

			display();

			App->Display();
		}
	}

	void handleHorizontalCameraMove(int direction) {
		leftRightRotationAngle += rotationSpeed * direction;
	}

	void handleVerticalCameraMove(int direction) {
		upDownRotationAngle += rotationSpeed * direction;
	}

private:
	sf::Window *App;
};

/**
int main(int argc, char **argv) {
>>>>>>> .r11
	init();
	GLBox prog;
	return EXIT_SUCCESS;
}*/
