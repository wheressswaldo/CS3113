////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS311 - Introduction to Game Programming
//		Assignment 2 - Pong
//
//		Notes:
//			- Everything appears to move and behave as expected
//			- Ball starting position and direction could use work, it only moves sorta diagonally for now
//			- Score is tracked but not displayed
//			- In retrospect, should have split up direction and speed into two variables instead of keeping it as one variable
//			  Limited me for some things.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#include <random>
#include <iostream>
#include <SDL_mixer.h>

#include "Entity.h"

SDL_Window* displayWindow;
using namespace std;

// resets the ball, including its position, direction, and speed
// this is called when a player scores or when you start a new game
void resetBall(Entity* ball){
	ball->xPosition = 0.0f;
	ball->yPosition = 0.0f;
	// get random X and Y values
	float randomX = ((float)rand() / (float)RAND_MAX) - 0.5f;
	float randomY = ((float)rand() / (float)RAND_MAX) - 0.5f;
	// if statements to check for the ball's speed (to make sure it doesn't move too slow)
	if (randomX < 0.0f && randomX > -0.3f){
		ball->xDirection = randomX - 0.25f;
	}
	else if (randomX > 0.0f && randomX < 0.3f){
		ball->xDirection = randomX + 0.25f;
	}
	else{
		ball->xDirection = randomX;
	}
	if (randomY < 0.0f && randomY > -0.3f){
		ball->yDirection = randomY - 0.25f;
	}
	else if (randomY > 0.0f && randomY < 0.3f){
		ball->yDirection = randomY + 0.25f;
	}
	else{
		ball->yDirection = randomY;
	}
}

// setup
void Setup(Entity* ball){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	// initialize ball direction and speed to 0 (so it stays still)
	ball->xDirection = 0.0f;
	ball->yDirection = 0.0f;
}

// keyboard events
bool ProcessEvents(Entity* player1, Entity* player2, Entity* ball){
	SDL_Event event;
	// set the paddle direction and speed to 0
	player1->yDirection = 0.0f;
	player2->yDirection = 0.0f;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		// press spacebar to reset the ball
		else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
			resetBall(ball);
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	// keyboard state here
	// use W for player one upward movement
	if (keys[SDL_SCANCODE_W]){
		player1->yDirection = 1.5f;
		// allow multiple keys to be pressed at once
		if (keys[SDL_SCANCODE_UP]){
			player2->yDirection = 1.5f;
		}
		else if (keys[SDL_SCANCODE_DOWN]){
			player2->yDirection = -1.5f;
		}
	}
	// use S for player one downward movement
	else if (keys[SDL_SCANCODE_S]){
		player1->yDirection = -1.5f;
		// allow multiple keys to be pressed at once
		if (keys[SDL_SCANCODE_UP]){
			player2->yDirection = 1.5f;
		}
		else if (keys[SDL_SCANCODE_DOWN]){
			player2->yDirection = -1.5f;
		}
	}
	// use UP arrow key for player two upward movement
	else if (keys[SDL_SCANCODE_UP]){
		player2->yDirection = 1.5f;
		// allow multiple keys to be pressed at once
		if (keys[SDL_SCANCODE_W]){
			player1->yDirection = 1.5f;
		}
		else if (keys[SDL_SCANCODE_S]){
			player1->yDirection = -1.5f;
		}
	}
	// use DOWN arrow key for player two upward movement
	else if (keys[SDL_SCANCODE_DOWN]){
		player2->yDirection = -1.5f;
		// allow multiple keys to be pressed at once
		if (keys[SDL_SCANCODE_W]){
			player1->yDirection = 1.5f;
		}
		else if (keys[SDL_SCANCODE_S]){
			player1->yDirection = -1.5f;
		}
	}
	return true;
}

// box-box collision detection between the paddle and the ball
bool checkPaddleCollision(Entity* paddle, Entity* ball){
	// width/800.0f and height/600.0f are used to scale to the coordinate system
	return ((ball->xPosition < (paddle->xPosition + paddle->width/800.0f)) && 
			((ball->xPosition + ball->width/800.0f) > paddle->xPosition) &&
			(ball->yPosition < (paddle->yPosition + paddle->height/600.0f)) && 
			((ball->yPosition + ball->height/600.0f) > paddle->yPosition));
}

// check if the ball collides with the top and bottom walls
bool checkWallCollision(Entity* ball){
	// the ball->yDirection check is needed so that if the ball doesn't leave the area in time, it doesn't keep changing direction
	return (((ball->yPosition + ball->height/600.0f >= 1.0f) && ball->yDirection > 0)|| (ball->yPosition <= -1.0f && ball->yDirection < 0));
}

// increase ball speed as the game goes on
void increaseSpeed(Entity* ball){
	if(ball->xDirection > 0.0f)
		ball->xDirection += 0.001f;
	else if(ball->xDirection < 0.0f)
		ball->xDirection -= 0.001f;
	if(ball->yDirection > 0.0f)
		ball->yDirection += 0.001f;
	else if(ball->yDirection < 0.0f)
		ball->yDirection -= 0.001f;
}

//update handles ball mvoement and plaeyr movement and collisions
void Update(Entity* player1, Entity* player2, Entity* ball, float elapsed, Mix_Chunk *hit, Mix_Chunk *lose){
	player1->yPosition += player1->yDirection * elapsed;
	player2->yPosition += player2->yDirection * elapsed;
	ball->xPosition += ball->xDirection * elapsed;
	ball->yPosition += ball->yDirection * elapsed;

	increaseSpeed(ball);

	// reverse y direction if ball collides with the wall
	if(checkWallCollision(ball)) { 
		ball->yDirection = -ball->yDirection; 
		Mix_PlayChannel(-1, hit, 0);
	}
	// reverse x direction if ball hits paddle 1
	if(checkPaddleCollision(player1, ball)){
		// direction check to make sure that the ball doesn't get stuck on paddle sometimes and keep changing direction
		if (ball->xDirection < 0){
			ball->xDirection = -ball->xDirection;
			Mix_PlayChannel(-1, hit, 0);
		}

	}
	// reverse x direction if ball hits paddle 2
	if(checkPaddleCollision(player2, ball)){
		// direction check to make sure that the ball doesn't get stuck on paddle sometimes and keep changing direction
		if (ball->xDirection > 0){
			ball->xDirection = -ball->xDirection;
			Mix_PlayChannel(-1, hit, 0);
		}
	}

	// score, if it gets past the paddle
	if(ball->xPosition > 1.33f){
		resetBall(ball);
		player1->score++;
		Mix_PlayChannel(-1, lose, 0);
	} 
	// score, if it gets past the paddle
	if(ball->xPosition < -1.33f){
		resetBall(ball);
		player2->score++;
		Mix_PlayChannel(-1, lose, 0);
	}

	// if checks to stop the paddle from flying off the screen
	if(player1->yPosition > 1.0f-player1->height/600.0f) {
		player1->yPosition = 1.0f-player1->height/600.0f; 
	}
	if(player2->yPosition > 1.0f-player2->height/600.0f) {
		player2->yPosition = 1.0f-player2->height/600.0f; 
	}
	if(player1->yPosition < -1.0f){
		player1->yPosition = -1.0f; 
	}
	if(player2->yPosition < -1.0f){
		player2->yPosition = -1.0f; 
	}
}

// render handles the background and loads/draws the textures
void Render(Entity* player1, Entity* player2, Entity* ball){
	// black if score is even, green if 1 is winning, blue if 2 is winning
	if(player1->score == player2->score)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	else if(player1->score > player2->score)
		glClearColor(0.0f, 0.6f, 0.0f, 1.0f);
	else if(player1->score < player2->score)
		glClearColor(0.0f, 0.0f, 0.6f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	
	// load textures
	player1->LoadTexture("paddle1.png");
	player2->LoadTexture("paddle2.png");
	ball->LoadTexture("ball.png");
	// draw textures
	player1->DrawSprite();
	player2->DrawSprite();
	ball->DrawSprite();

	SDL_GL_SwapWindow(displayWindow);
}

// main
int main(int argc, char *argv[])
{
	Entity* player1 = new Entity(-1.31f, 0.0f, 50.0f, 200.0f, 0.0f);
	Entity* player2 = new Entity(1.25f, 0.0f, 50.0f, 200.0f, 0.0f);
	Entity* ball = new Entity(0.0f, 0.0f, 50.0f, 50.0f, 0.0f);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *hit;
	hit = Mix_LoadWAV("hitpaddle.wav");
	Mix_Chunk *lose;
	lose = Mix_LoadWAV("pastpaddle.wav");

	Mix_Music *song;
	song = Mix_LoadMUS("song.mp3");

	Mix_PlayMusic(song, -1);

	// setup the ball
	Setup(ball);

	float lastFrameTicks = 0.0f;

	while (ProcessEvents(player1, player2, ball)) {
		// for timer
		float ticks = (float)SDL_GetTicks()/1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		// functions for keyboard, updates, and drawing stuff
		ProcessEvents(player1, player2, ball);
		Update(player1, player2, ball, elapsed, hit, lose);
		Render(player1, player2, ball);
	}
	// quit
	SDL_Quit();
	return 0;
}