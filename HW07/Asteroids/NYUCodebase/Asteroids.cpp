//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Asteroids.h"

float Asteroids::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void Asteroids::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), 0.5f* size, ((size + spacing) * i) + (-0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), 0.5f * size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}

float Asteroids::generateRandomNumber(float min, float max) {
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

Asteroids::Asteroids() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	// general values	
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	// setting gravity
	gravity_x = 0.0f;
	gravity_y = 0.0f;

	// loading textures
	spriteSheetTexture = LoadTexture("sheet.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	// setting state
	state = STATE_MAIN_MENU;

	Init();
}

Asteroids::~Asteroids() {
	SDL_Quit();
}

void Asteroids::Init() {
	// insert player as the first entity
	SpriteSheet playerSprite = SpriteSheet(spriteSheetTexture, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	Entity* player = new Entity();
	player->sprite = playerSprite;
	player->scale_x = 1.0f;
	player->scale_y = 1.0f;
	player->x = 0.0f;
	player->y = 0.0f;
	player->friction_x = 3.0f;
	player->friction_y = 3.0f;
	entities.push_back(player);

	// insert asteroids
	SpriteSheet asteroidSprite = SpriteSheet(spriteSheetTexture, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
	for (int i = 0; i < 10; i++) {
		Entity* asteroid = new Entity();
		asteroid->sprite = asteroidSprite;
		// set random size
		float scale = generateRandomNumber(0.2f, 2.0f);
		asteroid->scale_x = scale;
		asteroid->scale_y = scale;

		// set random location
		asteroid->x = generateRandomNumber(-1.33f, 1.33f);
		asteroid->y = generateRandomNumber(-1.0f, 1.0f);

		// set random direction
		asteroid->rotation = generateRandomNumber(-90.0f, 90.0f);

		// set random speed
		asteroid->velocity_x = generateRandomNumber(-0.1f, 0.2f);
		asteroid->velocity_y = generateRandomNumber(-0.1f, 0.2f);
		entities.push_back(asteroid);
	}
}

void Asteroids::Update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		updateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
		break;
	}
}

void Asteroids::updateMainMenu(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_GAME_LEVEL;
				break;
			}
		}
	}
}

void Asteroids::updateGameLevel(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W]) {
		entities[0]->velocity_x = 0.5f;
		entities[0]->velocity_y = 0.5f;
	}

	if (keys[SDL_SCANCODE_D]) {
		entities[0]->rotation -= 5.0f * elapsed;
	}
	else if (keys[SDL_SCANCODE_A]) {
		entities[0]->rotation += 5.0f * elapsed;
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}
}

void Asteroids::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {

		// collision
		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (entities[i] != entities[j])
					collision(entities[i], entities[j]);
			}
		}

		// apply gravity
		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		// apply friction
		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		// apply velocity
		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		// update y values
		entities[i]->y += entities[i]->velocity_y * -cos(entities[i]->rotation) * FIXED_TIMESTEP;

		// update x values
		entities[i]->x += entities[i]->velocity_x * sin(entities[i]->rotation) * FIXED_TIMESTEP;
	}
}

void Asteroids::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	switch (state) {
	case STATE_MAIN_MENU:
		renderMainMenu();
		break;
	case STATE_GAME_LEVEL:
		renderGameLevel();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void Asteroids::renderMainMenu() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.7f, 0.0f);
	DrawText(fontSheetTexture, "Asteroids", 0.1, 0.0, 0.7, 0.4, 0.3, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.4f, 0.0f);
	DrawText(fontSheetTexture, "Press ASD to TURN", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "Press W to MOVE FORWARD", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to start the game", 0.05, 0.0, 0.7, 0.4, 0.3, 1.0);
}

void Asteroids::renderGameLevel() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}
}

bool Asteroids::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;
	Update(elapsed);
	Render();
	return done;
}

bool Asteroids::collision(Entity* e1, Entity* e2) {
	// set up matrices
	e1->buildMatrixAndSetUp();
	e2->buildMatrixAndSetUp();

	// setting up vectors
	// for personal reference: 1tl 2tr 3bl 4br
	Vector a1 = e1->inverse * (e2->matrix * e2->TL);
	Vector a2 = e1->inverse * (e2->matrix * e2->TR);
	Vector a3 = e1->inverse * (e2->matrix * e2->BL);
	Vector a4 = e1->inverse * (e2->matrix * e2->BR);

	Vector b1 = e2->inverse *(e1->matrix * e1->TL);
	Vector b2 = e2->inverse *(e1->matrix * e1->TR);
	Vector b3 = e2->inverse *(e1->matrix * e1->BL);
	Vector b4 = e2->inverse *(e1->matrix * e1->BR);
	
	float mXA = min(min(min(a1.x, a3.x), a2.x), a4.x);
	float mxXA = max(max(max(a1.x, a3.x), a2.x), a4.x);

	// check x a
	if (!(mXA <= e1->sprite.width * e1->scale_x && mxXA >= -e1->sprite.width * e1->scale_x))
		return false;

	float mYA = min(min(min(a1.y, a3.y), a2.y), a4.y);
	float mxYA = max(max(max(a1.y, a3.y), a2.y), a4.y);

	// check y a
	if (!(mYA <= e1->sprite.height * e1->scale_y && mxYA >= -e1->sprite.height * e1->scale_y))
		return false;

	float mXB = min(min(min(b1.x, b3.x), b2.x), b4.x);
	float mxXB = max(max(max(b1.x, b3.x), b2.x), b4.x);

	// check x a
	if (!(mXB <= e2->sprite.width * e2->scale_x && mxXB >= -e2->sprite.width * e2->scale_x))
		return false;

	float mYB = min(min(min(b1.y, b3.y), b2.y), b4.y);
	float mxYB = max(max(max(b1.y, b3.y), b2.y), b4.y);

	if (!(mYB <= e2->sprite.height * e2->scale_y && mxYB >= -e2->sprite.height * e2->scale_y))
		return false;

	// adjustment
	float adjust = 0.005f;
	if (e1->x < e2->x){
		e1->x -= adjust;
		if (e1->y < e2->y){
			e1->y -= adjust;
		}
		else if (e1->y > e2->y){
			e1->y += adjust;
		}
	}
	else if (e1->x > e2->x){
		e1->x += adjust;
		if (e1->y < e2->y){
			e1->y -= adjust;
		}
		else if (e1->y > e2->y){
			e1->y += adjust;
		}
	}
	else {
		if (e1->y < e2->y){
			e1->y -= adjust;
		}
		else if (e1->y > e2->y){
			e1->y += adjust;
		}
	}

	// end
	return true;
}
