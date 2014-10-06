/////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS311 - Introduction to Game Programming
//		Assignment 1
//
/////////////////////////////////////////////////////////////////////////

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation, float scale) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}


void DrawCircle(float r, int num_segments)
{
	float incr = (float) (2* 3.1415926 / num_segments);

	glBegin(GL_TRIANGLE_FAN);
	
	glVertex2f(0, 0);

	for (int i = 0; i < num_segments; i++)
	{
		float angle = incr * i;
		float x = (float)cos(angle) * r;
		float y = (float)sin(angle) * r;

		glVertex2f(x, y);
	}

	glVertex2f(r, 0.0f);

	glEnd();
}


int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("CS3113HW1 - Kong Huang", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	GLuint enemyShip = LoadTexture("enemyShip.png");
	GLuint bigMeteor = LoadTexture("meteorBig.png");
	GLuint smallMeteor = LoadTexture("meteorSmall.png");

	float lastFrameTicks = 0.0f;
	GLfloat angle = 0.0f;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		angle += elapsed * 50;
				
		DrawSprite(enemyShip, 0.0, -0.8, 180, 0.2);
		DrawSprite(bigMeteor, 0.8, 0.5, 0.5, 0.5);
		DrawSprite(smallMeteor, -0.8, -0.3, 0.2, 0.2);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(angle / 2, 0, 0, 1);
		glTranslatef(0.0f, 0.3f, 0.0f);
		glRotatef(angle, 0, 0, 1);

		DrawCircle(0.2f, 200);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-angle / 2, 0, 0, 1);
		glTranslatef(0.0f, -0.3f, 0.0f);
		glRotatef(angle, 0, 0, 1);

		GLfloat triangle[] = { 0.0f, 0.25f,
							  -0.25f, -0.25f,
							   0.25f, -0.25f };
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);


		GLfloat triangleColors[] = { 1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
					0.0, 0.0, 1.0
		};
		glColorPointer(3, GL_FLOAT, 0, triangleColors);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableClientState(GL_COLOR_ARRAY);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}