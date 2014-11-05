//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Asteroids.h"

Asteroids::Asteroids() {
	Init();
	srand(time(NULL));
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	gravity_x = 0.0f;
	gravity_y = 0.0f;
	bulletTime = 0;
	state = STATE_MAIN_MENU;

	spriteSheetTexture = LoadTexture("sheet.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	bulletSprite = SpriteSheet(spriteSheetTexture, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f);
	SpriteSheet playerSprite = SpriteSheet(spriteSheetTexture, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	player = new Player();
	player->sprite = playerSprite;
	player->scale_x = 1.0f;
	player->scale_y = 1.0f;
	player->x = 0.0f;
	player->y = 0.0f;
	player->friction_x = 3.0f;
	player->friction_y = 3.0f;
	entities.push_back(player);

	SpriteSheet asteroidSprite = SpriteSheet(spriteSheetTexture, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
	for (int i = 0; i < 40; i++) {
		Entity* asteroid = new Entity();
		asteroid->sprite = asteroidSprite;
		float scale = generateRandomNumber(0.7f, 1.5f);
		asteroid->scale_x = scale;
		asteroid->scale_y = scale;
		asteroid->x = generateRandomNumber(-1.33f, 1.33f);
		asteroid->y = generateRandomNumber(-1.0f, 1.0f);
		asteroid->rotation = generateRandomNumber(-90.0f, 90.0f);
		asteroid->velocity_x = generateRandomNumber(-0.15f, 0.15f);
		asteroid->velocity_y = generateRandomNumber(-0.15f, 0.15f);
		entities.push_back(asteroid);
	}
}
Asteroids::~Asteroids() {
	SDL_Quit();
}
void Asteroids::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
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
		player->velocity_x = 1.0f;
		player->velocity_y = 1.0f;
	}
	else if (keys[SDL_SCANCODE_S]) {
		player->velocity_x = 0.0f;
		player->velocity_y = 0.0f;
	}
	if (keys[SDL_SCANCODE_D]) {
		player->rotation -= 5.0f * elapsed;
	}
	else if (keys[SDL_SCANCODE_A]) {
		player->rotation += 5.0f * elapsed;
	}
	if (keys[SDL_SCANCODE_SPACE]) {
		if (!playerBullet.visible) {
			shootBullet();
		}
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	playerBullet.Update(elapsed);

	bulletTime += elapsed;
	enemySpawnTimer += elapsed;
}
void Asteroids::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {
		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (checkCollision(entities[i], entities[j])) {
					Vector distance = Vector(entities[i]->x - entities[j]->x, entities[i]->y - entities[j]->y, 0.0f);
					float distLength = distance.length();
					distance.normalize();

					entities[i]->x += distance.x * 0.0001f / pow(distLength, 2);
					entities[i]->y += distance.y * 0.0001f / pow(distLength, 2);

					entities[j]->x -= distance.x * 0.0001f / pow(distLength, 2);
					entities[j]->y -= distance.y * 0.0001f / pow(distLength, 2);
				}
			}
			
		}
		entities[i]->y += entities[i]->velocity_y * -cos(entities[i]->rotation) * FIXED_TIMESTEP;
		entities[i]->x += entities[i]->velocity_x * sin(entities[i]->rotation) * FIXED_TIMESTEP;
	}
	for (int i = 1; i < entities.size(); i++) {
		if (playerBullet.visible && checkCollision(entities[i], &playerBullet)) {
			playerBullet.visible = false;
			if (entities[i]->scale_x < 1.0f) {
				delete entities[i];
				entities.erase(entities.begin() + i);
				break;
			}
			else {
				entities[i]->scale_x = entities[i]->scale_x * 0.67;
				entities[i]->scale_y = entities[i]->scale_y * 0.67;
				entities[i]->rotation = generateRandomNumber(-90.0f, 90.0f);
			}
		}
	}
	if (bulletTime >= 1 && playerBullet.visible){
		playerBullet.visible = false;
		bulletTime = 0;
	}
	else if (bulletTime >= 1 && !playerBullet.visible){
		bulletTime = 0;
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
	glTranslatef(-0.8f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to shoot", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.2f, 0.0f);
	DrawText(fontSheetTexture, "Note: Single Bullet is intended", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
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
	playerBullet.Render();
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
bool Asteroids::checkCollision(Entity* entity1, Entity* entity2) {
	if (entity1 == entity2)
		return false;
	entity1->buildMatrix();
	entity2->buildMatrix();

	Matrix entity1Inverse = entity1->matrix.inverse();
	Matrix entity2Inverse = entity2->matrix.inverse();

	float minX, maxX, minY, maxY;

	Vector ent2TL = Vector(-entity2->sprite.width * entity2->scale_x, entity2->sprite.height * entity2->scale_y, 0.0);
	Vector ent2BL = Vector(-entity2->sprite.width * entity2->scale_x, -entity2->sprite.height * entity2->scale_y, 0.0);
	Vector ent2TR = Vector(entity2->sprite.width * entity2->scale_x, entity2->sprite.height * entity2->scale_y, 0.0);
	Vector ent2BR = Vector(entity2->sprite.width * entity2->scale_x, -entity2->sprite.height * entity2->scale_y, 0.0);

	ent2TL = entity2->matrix * ent2TL;
	ent2BL = entity2->matrix * ent2BL;
	ent2TR = entity2->matrix * ent2TR;
	ent2BR = entity2->matrix * ent2BR;

	ent2TL = entity1Inverse * ent2TL;
	ent2BL = entity1Inverse * ent2BL;
	ent2TR = entity1Inverse * ent2TR;
	ent2BR = entity1Inverse * ent2BR;
	
	minX = min(min(min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	maxX = max(max(max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	if (!(minX <= entity1->sprite.width * entity1->scale_x && maxX >= -entity1->sprite.width * entity1->scale_x))
		return false;

	minY = min(min(min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	maxY = max(max(max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	if (!(minY <= entity1->sprite.height * entity1->scale_y && maxY >= -entity1->sprite.height * entity1->scale_y))
		return false;

	Vector ent1TL = Vector(-entity1->sprite.width * entity1->scale_x, entity1->sprite.height * entity1->scale_y, 0.0);
	Vector ent1BL = Vector(-entity1->sprite.width * entity1->scale_x, -entity1->sprite.height * entity1->scale_y, 0.0);
	Vector ent1TR = Vector(entity1->sprite.width * entity1->scale_x, entity1->sprite.height * entity1->scale_y, 0.0);
	Vector ent1BR = Vector(entity1->sprite.width * entity1->scale_x, -entity1->sprite.height * entity1->scale_y, 0.0);

	ent1TL = entity1->matrix * ent1TL;
	ent1BL = entity1->matrix * ent1BL;
	ent1TR = entity1->matrix * ent1TR;
	ent1BR = entity1->matrix * ent1BR;

	ent1TL = entity2Inverse * ent1TL;
	ent1BL = entity2Inverse * ent1BL;
	ent1TR = entity2Inverse * ent1TR;
	ent1BR = entity2Inverse * ent1BR;

	minX = min(min(min(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	maxX = max(max(max(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	if (!(minX <= entity2->sprite.width * entity2->scale_x && maxX >= -entity2->sprite.width * entity2->scale_x))
		return false;
	minY = min(min(min(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	maxY = max(max(max(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	if (!(minY <= entity2->sprite.height * entity2->scale_y && maxY >= -entity2->sprite.height * entity2->scale_y))
		return false;

	return true;
}
void Asteroids::shootBullet() {
	playerBullet.sprite = bulletSprite;
	playerBullet.visible = true;
	playerBullet.x = player->x;
	playerBullet.y = player->y;
	playerBullet.rotation = player->rotation;
}
float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}
float generateRandomNumber(float low, float high) {
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
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