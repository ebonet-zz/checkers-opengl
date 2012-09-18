//#include <SFML/Window.hpp>
//#include "glyphs.h"
//#define RESOLUTION 512
//
///* This is the callback function that gets executed every time the
// * display needs to be updated.
// */
//void display(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
//	glColor3f(0,0,0);
//	glRasterPos2d(0.5, 0.5);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['1']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['2']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['3']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['4']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs[' ']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['t']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['e']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['s']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['t']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs[' ']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['T']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['E']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['S']);
//	glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs['T']);
//}
//
///* This is the function that gets executed to draw the original
// * graphic image.
// */
//void gfxinit()
//{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0, 1, 0, 1);
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//}
//
//
//class GLBox
//{
//public:
//	GLBox()
//	{
//		App = new sf::Window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "Glyph");
//		
//		gfxinit();
//		
//		while (App->IsOpened())
//		{
//			handleEvents();
//			display();
//			App->Display();
//		}
//	}
//	
//private:
//	sf::Window *App;
//	
//	void handleEvents()
//	{
//		sf::Event Event;
//		while (App->GetEvent(Event))
//		{
//			// Close window : exit
//			if (Event.Type == sf::Event::Closed)
//				App->Close();
//			
//			// Escape key : exit
//			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
//				App->Close();
//		}
//	}
//};
//
//int main(int argc, char **argv)
//{
//	GLBox prog;
//	return EXIT_SUCCESS;
//}
