#include <stdlib.h>
#include <stdio.h>
#include <SFML/Window.hpp>

#define BUFSIZE 900
#define NCOLS_BOARD 8
#define NLINS_BOARD 8
#define X0 -50.0f
#define X1 50.0f
#define Y0 -50.0f
#define Y1 50.0f
#define WHITE 1
#define GREY 2

GLfloat ratio = 1.0f;
GLfloat eyePositionModifier = 1.0f;
GLfloat eyePositionX = 100.0f;
GLfloat eyePositionY = 0.0f;
GLfloat eyePositionZ = 80.0f;

GLfloat AMB[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DIF[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat ESP[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat POS[2][4] = { { 50.0f, 50.0f, 100.0f, 1.0f }, { -50.0f, -50.0f, 20.0f, 1.0f } };

GLfloat* color[8][8];

void drawBoard(GLenum mode)
{
	GLfloat pieceWidth = (X1-X0)/NCOLS_BOARD;
	GLfloat pieceHeight = (Y1-Y0)/NLINS_BOARD;

	for (int col = 0; col < 8; col++) {
		GLfloat x = X0 + pieceWidth * col;
		for (int lin = 0; lin < 8; lin++) {
			GLfloat y = Y0 + pieceHeight * lin;

			// TODO: set names for picking

			glBegin(GL_POLYGON);
			{
				glColor4fv(color[col][lin]);
				glNormal3f(0, 0, 1);
				glVertex3f(x, y, 0.0f);
				glVertex3f(x + pieceWidth, y, 0.0f);
				glVertex3f(x + pieceWidth, y + pieceHeight, 0.0f);
				glVertex3f(x, y + pieceHeight, 0.0f);
			}
			glEnd();
		}
	}
}

void initBoardColors() {
	for (int col = 0; col < 8; col++) {
		for (int lin = 0; lin < 8; lin++) {
			if ((col + lin) % 2 == 0) {
				GLfloat c[4] = {1.0f,1.0f,1.0f,1.0f};
				color[col][lin] = c;
			} else {
				GLfloat c[4] = {0.2f,0.2f,0.2f,1.0f};
				color[col][lin] = c;
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

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eyePositionX / eyePositionModifier, eyePositionY / eyePositionModifier, eyePositionZ / eyePositionModifier, 0, 0, 0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBoard(GL_RENDER);

	glFlush();
}

void setSize(unsigned int w, unsigned int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ratio = (GLfloat) w / (GLfloat) h;
	gluPerspective(60, ratio, 0.5, 500.0);
}

class GLBox
{
public:
	GLBox()
	{
		App = new sf::Window(sf::VideoMode(BUFSIZE, BUFSIZE, 32), "Click me!");

		setSize(BUFSIZE, BUFSIZE);
		init();
		while (App->IsOpened())
		{
			App->SetActive();

			sf::Event Event;
			while (App->GetEvent(Event))
			{
				// Close window : exit
				if (Event.Type == sf::Event::Closed)
					App->Close();

				// Escape key : exit
				if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
					App->Close();

				if (Event.Type == sf::Event::MouseButtonPressed)
				{
					//pickSquares(Event.MouseButton.X, Event.MouseButton.Y);
					//printf("pick\n");
				}

				if (Event.Type == sf::Event::Resized)
				{
					setSize(Event.Size.Width, Event.Size.Height);
				}
			}

			display();

			App->Display();
		}
	}

private:
	sf::Window *App;
};


int main(int argc, char **argv)
{
	init();
	GLBox prog;
	return EXIT_SUCCESS;
}
