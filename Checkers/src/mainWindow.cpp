#include <stdlib.h>
#include <stdio.h>
#include <SFML/Window.hpp>
#include <queue>
#include "checkersCore.h"
#include "glyphs.h"
using namespace std;

// Defines and constants
#define ROUND_PRECISION 50
#define CIRC 2 * M_PI
#define DTHETA CIRC / ROUND_PRECISION
#define WINDOW_SIZE 700
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
const GLfloat BLUE[4] = { 0.0f, 0.0f, 0.4f, 1.0f };
const GLfloat RED[4] = { 0.35f, 0.0f, 0.0f, 1.0f };
const GLfloat BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat BEIGE[4] = { 0.3f, 0.3f, 0.1f, 1.0f };
const GLfloat BUTTON_1_COLOR[4] = { 0.6f, 0.6f, 0.3f, 1.0f };
const GLfloat BUTTON_2_COLOR[4] = { 0.6f, 0.6f, 0.3f, 1.0f };

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
GLfloat buttonsColor[2][1][4];

// Mouse click selection results
int lastSelectedCoordinates[2];
int lastHitCount = 0;

// Piece stack from the core
std::queue<piece> pieceList;
Coordinate lastSelectedCoordinate;
Coordinate NO_SELECTION(-1, -1);
bool quit = false;
//The game core
CheckersCore GameCore;

/**
 * Resets the black/white color pattern for all the board tile
 */
void initBoardColors() {
	for (int col = 0; col < 8; col++) {
		for (int lin = 0; lin < 8; lin++) {
			if ((col + lin) % 2 == 0) {
				memcpy(boardColor[lin][col], WHITE, 4 * sizeof(GLfloat));
			} else {
				memcpy(boardColor[lin][col], BEIGE, 4 * sizeof(GLfloat));
			}
		}
	}
}

void initButtonsColors() {
	for (int col = 0; col < 2; col++) {
		for (int lin = 0; lin < 1; lin++) {
			if ((col + lin) % 2 == 0) {
				memcpy(buttonsColor[lin][col], BUTTON_1_COLOR, 4 * sizeof(GLfloat));
			} else {
				memcpy(buttonsColor[lin][col], BUTTON_2_COLOR, 4 * sizeof(GLfloat));
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
	initButtonsColors();
	pieceList = GameCore.getPieces();
}

void drawBoardBorders() {
	/* Board Lower Face*/
	glBegin(GL_POLYGON);
	{
		glNormal3f(0, 0, -1);
		glColor4fv(GREY);
		glVertex3f(X0, Y0, -4.0f);
		glVertex3f(X0, Y1, -4.0f);
		glColor4fv(RED);
		glVertex3f(X1, Y1, -4.0f);
		glVertex3f(X1, Y0, -4.0f);

	}
	glEnd();

	/* Board Grey Face*/
	glBegin(GL_POLYGON);
	{
		glColor4fv(GREY);

		glNormal3f(0, -1, 0);
		glVertex3f(X0, Y1, -4.00f);
		glVertex3f(X0, Y0, -4.00f);
		glVertex3f(X0, Y0, 0.00f);
		glVertex3f(X0, Y1, 0.00f);
	}
	glEnd();

	/* Board Red Face*/
	glBegin(GL_POLYGON);
	{
		glColor4fv(RED);
		glNormal3f(0, -1, 0);
		glVertex3f(X1, Y1, -4.00f);
		glVertex3f(X1, Y0, -4.00f);
		glVertex3f(X1, Y0, 0.00f);
		glVertex3f(X1, Y1, 0.00f);
	}
	glEnd();

	/* Board Right Face*/
	glBegin(GL_POLYGON);
	{
		glColor4fv(GREY);
		glVertex3f(X0, Y1, 0.00f);
		glVertex3f(X0, Y1, -4.00f);

		glColor4fv(RED);
		glVertex3f(X1, Y1, -4.00f);
		glVertex3f(X1, Y1, 0.00f);
	}
	glEnd();

	/* Board Left Face*/
	glBegin(GL_POLYGON);
	{
		glColor4fv(GREY);
		glVertex3f(X0, Y0, 0.00f);
		glVertex3f(X0, Y0, -4.00f);

		glColor4fv(RED);
		glVertex3f(X1, Y0, -4.00f);
		glVertex3f(X1, Y0, 0.00f);
	}
	glEnd();

}

/**
 * Reads the current board color state and draws the alternated squares.
 * Naming happens here in order to allow picking with GL_SELECT render mode.
 *
 * Param mode - The current OpenGl rendering mode.
 */
void drawBoard(GLenum mode) {

	for (int lin = 0; lin < 8; lin++) {
		GLfloat x = X0 + boardTileWidth * lin;
		if (mode == GL_SELECT) {
			glLoadName(7 - lin);
		}
		for (int col = 0; col < 8; col++) {
			GLfloat y = Y0 + boardTileHeight * col;
			if (mode == GL_SELECT) {
				glPushName(col);
			}
			glBegin(GL_POLYGON);
			{
				glColor4fv(boardColor[7 - lin][col]);
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

	glTranslatef(X0 + boardTileWidth * (7 - p.line), Y0 + boardTileHeight * (p.column), 0.1);
	glTranslatef(boardTileWidth / 2, boardTileHeight / 2, 0);

	gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
	gluCylinder(gluNewQuadric(), pieceWidth, pieceWidth, pieceHeight, ROUND_PRECISION, ROUND_PRECISION);
	glTranslatef(0, 0, pieceHeight);
	gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);

	if (p.pieceType == 'R' || p.pieceType == 'B') {
		glTranslatef(0, 0, 0.1);
		gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
		gluCylinder(gluNewQuadric(), pieceWidth, pieceWidth, pieceHeight, ROUND_PRECISION, ROUND_PRECISION);
		glTranslatef(0, 0, pieceHeight);
		gluDisk(gluNewQuadric(), 0, pieceWidth, ROUND_PRECISION, ROUND_PRECISION);
	}

	glPopMatrix();
}

void drawMenuButtons(GLenum mode) {
	GLfloat x = X0 - 15;
	GLfloat y = Y0 - 36;
	if (mode == GL_SELECT) {
		glLoadName(BUTTON_1_NAME);
		glPushName(1);
	}
	glBegin(GL_POLYGON);
	{
		glColor4fv(buttonsColor[0][0]);
		glNormal3f(0, 0, -1);
		glVertex3f(x, y, 0.0f);
		glVertex3f(x + boardTileWidth, y, 0.0f);
		glVertex3f(x + boardTileWidth, y + boardTileHeight, 0.0f);
		glVertex3f(x, y + boardTileHeight, 0.0f);
	}
	glEnd();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor4fv(BLACK);
	glRasterPos3d(x + 5.5, y - 3, 0.1);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['N']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['e']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['w']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs[' ']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['G']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['a']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['m']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['e']);

	if (mode == GL_SELECT) {
		glPopName();
	}

	y = Y0 - 52;
	if (mode == GL_SELECT) {
		glLoadName(BUTTON_2_NAME);
		glPushName(2);
	}
	glBegin(GL_POLYGON);
	{
		glColor4fv(buttonsColor[1][0]);
		glNormal3f(0, 0, -1);
		glVertex3f(x, y, 0.0f);
		glVertex3f(x + boardTileWidth, y, 0.0f);
		glVertex3f(x + boardTileWidth, y + boardTileHeight, 0.0f);
		glVertex3f(x, y + boardTileHeight, 0.0f);
	}
	glEnd();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor4fv(BLACK);
	glRasterPos3d(x + 5.5, y - 4, 0.1);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['E']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['x']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['i']);
	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*) glyphs['t']);

	if (mode == GL_SELECT) {
		glPopName();
	}
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
	drawBoardBorders();

	unsigned int i = 0;

	while (i < pieceList.size()) {
		Piece somePiece = pieceList.front();
		drawPiece(somePiece);
		pieceList.pop();
		pieceList.push(somePiece);
		i++;
	}
	/*
	 for (list<piece>::iterator it = pieceList.begin(); it != pieceList.end(); it++){
	 drawPiece(*it);
	 }*/

//	piece p(3, 3, 'r');
//	piece p2(1, 1, 'b');
//	drawPiece(p);
//	drawPiece(p2);
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

	cout << "hits = " << hits << endl;
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
		cout << "   names are ";

		for (GLuint j = 0; j < names; j++) { /*  for each name */
			lastSelectedCoordinates[j] = *bufferIntegerPointer;
			cout << *bufferIntegerPointer << " ";
			bufferIntegerPointer++;
		}
		cout << endl;
	}
}

/**
 * Translates the mouse click coordinates into an element selection using OpenGl GL_SELECT render mode.
 */
void captureClick(int x, int y) {
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

	/*       create 5x5 pixel picking region near cursor location   */
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 2.0, 2.0, viewport);

	gluPerspective(60, ratio, 0.5, 500.0);

	drawBoard(GL_SELECT);
	drawMenuButtons(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

bool wasBoardHit(int lin, int col) {
	return col >= 0 && col <= 7 && lin >= 0 && lin <= 7;
}

bool wasButtonHit(int lin, int col) {
	return lin >= BUTTON_1_NAME && lin <= BUTTON_2_NAME && col >= 1 && col <= 2;
}

bool wasMoveHit(int lin, int col) {
	if (lastSelectedCoordinate == NO_SELECTION)
		return false;

	std::list<Coordinate> possibleMoves = GameCore.getPossibleMoves(lastSelectedCoordinate);
	for (list<Coordinate>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++) {
		if (it->row == lin && it->column == col) {
			return true;
		}
	}

	return false;
}

/**
 * Lights up one of the board tiles to the desired new color
 */
void lightBoardTile(int lin, int col, const GLfloat color[4]) {
	if (wasBoardHit(lin, col)) {
		memcpy(boardColor[lin][col], color, 4 * sizeof(GLfloat));
	}
}

void highLightPossibleMoves(int lin, int col) {
	std::list<Coordinate> possibleMoves = GameCore.getPossibleMoves(Coordinate(lin, col));
	for (list<Coordinate>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++) {
		lightBoardTile(it->row, it->column, LIGHT_GREEN);
	}
}

void performButtonAction(int lin, int col, const GLfloat color[4]) {
	if (wasButtonHit(lin, col)) {
		if (lin == BUTTON_1_NAME) {
			GameCore.restartGame();
			initBoardColors();
			pieceList = GameCore.getPieces();
		} else {
			quit = true;
		}
	}
}

void movePiece(Coordinate o, Coordinate n) {
	GameCore.makeMove(o, n);
	pieceList = GameCore.getPieces();
}

/**
 * Brings the board tile to its default color.
 */
void restoreBoardTile(int lin, int col) {
	if ((col + lin) % 2 == 0) {
		memcpy(boardColor[lin][col], WHITE, 4 * sizeof(GLfloat));
	} else {
		memcpy(boardColor[lin][col], GREY, 4 * sizeof(GLfloat));
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

					captureClick(Event.MouseButton.X, Event.MouseButton.Y);
					handleClick();
				}

				if (quit) {
					App->Close();
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

	void handleClick() {
		// resets
		initBoardColors();
		initButtonsColors();

		if (lastHitCount != 0) {
			if (wasBoardHit(lastSelectedCoordinates[0], lastSelectedCoordinates[1])) {
				Coordinate newSelected(lastSelectedCoordinates[0], lastSelectedCoordinates[1]);
				if (GameCore.isTileSelectable(newSelected)) {
					if (lastSelectedCoordinate != newSelected) {
						lastSelectedCoordinate = newSelected;
						lightBoardTile(lastSelectedCoordinates[0], lastSelectedCoordinates[1], BLUE);
						highLightPossibleMoves(lastSelectedCoordinates[0], lastSelectedCoordinates[1]);
					} else {
						lastSelectedCoordinate = NO_SELECTION;
						initBoardColors();
						initButtonsColors();
					}
				} else if (wasMoveHit(lastSelectedCoordinates[0], lastSelectedCoordinates[1])) {
					movePiece(lastSelectedCoordinate, newSelected);

					if(GameCore.isGameWon()>0){
						cout << GameCore.isGameWon() << " won the game" <<endl;


						for(int row=0;row<8;row++){
							for(int col =0; col<8;col++){
								lightBoardTile(row, col, (GameCore.isGameWon()==1)? RED: GREY);

							}
						}
					}

					lastSelectedCoordinate = NO_SELECTION;
				} else {
					lastSelectedCoordinate = NO_SELECTION;
					initBoardColors();
					initButtonsColors();
				}
			}
			if (wasButtonHit(lastSelectedCoordinates[0], lastSelectedCoordinates[1])) {
				performButtonAction(lastSelectedCoordinates[0], lastSelectedCoordinates[1], LIGHT_GREEN);
				lastSelectedCoordinate = NO_SELECTION;
			}
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
