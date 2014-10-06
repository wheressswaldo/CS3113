//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 4 - Simple Screen Platformer
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "platformer.h"

platformer::platformer() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	done = false;

	state = STATE_MAIN_MENU;

	init();
}
platformer::~platformer() {
	SDL_Quit();
}
void platformer::init() {
	tileIndex = 0;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	gravity_x = 0.0f;
	gravity_y = -9.8f;
	score = 0;

	mapsheet = LoadTexture("tile1.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	SpriteSheet tileSprite;
	for (float i = -1.4f; i <= 1.5f; i = i + 0.2f) {
		tileSprite = SpriteSheet(mapsheet, 0.0f, 0.0f, 1.0f, 1.0f);
		tiles[tileIndex].sprite = tileSprite;
		tiles[tileIndex].isStatic = true;
		tiles[tileIndex].y = -0.9f;
		tiles[tileIndex].x = i;
		tiles[tileIndex].scale = 0.2f;
		tiles[tileIndex].width = 0.2f;
		tiles[tileIndex].height = 0.2f;
		entities.push_back(&tiles[tileIndex]);
		tileIndex++;
	}
	for (float i = -1.4f; i <= -0.8f; i = i + 0.2f) {
		tileSprite = SpriteSheet(mapsheet, 0.0f, 0.0f, 1.0f, 0.5f);
		tiles[tileIndex].sprite = tileSprite;
		tiles[tileIndex].isStatic = true;
		tiles[tileIndex].y = -0.3f;
		tiles[tileIndex].x = i;
		tiles[tileIndex].scale = 0.1f;
		tiles[tileIndex].width = 0.2f;
		tiles[tileIndex].height = 0.1f;
		entities.push_back(&tiles[tileIndex]);
		tileIndex++;
	}

	charactersheet = LoadTexture("player.png");
	enemysheet = LoadTexture("enemyy.png");

	SpriteSheet playerSprite = SpriteSheet(charactersheet, 2.0f / 55.0f, 72.0f / 142.0f, 51.0f / 55.0f, 68.0f / 142.0f);
	player = new Player();
	player->sprite = playerSprite;
	player->x = -1.1f;
	player->y = -0.2f;
	player->width = 75.0f / 800.0f;
	player->height = 75.0f / 600.0f;
	player->friction_x = 3.0f;
	player->face_left = false;
	entities.push_back(player);

}
void platformer::shootPlayerBullet() {
	SpriteSheet bulletSprite = SpriteSheet(charactersheet, 20.0f / 55.0f, 20.0f / 142.0f, 5.0f / 55.0f, 40.0f / 142.0f);
	playerBullet.sprite = bulletSprite;
	playerBullet.visible = true;
	playerBullet.x = player->x;
	playerBullet.y = player->y;
	playerBullet.rotation = 90.0f;
	playerBullet.scale = 0.05f;
	if (player->face_left)
		playerBullet.velocity_x = -3.5f;
	else
		playerBullet.velocity_x = 3.5f;
}
float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}
void platformer::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
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
void platformer::update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		updateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
		break;
	}
}
void platformer::updateMainMenu(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_GAME_LEVEL;
			}
		}
	}

}
void platformer::updateGameLevel(float elapsed) {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_X) {
				if (!playerBullet.visible)
					shootPlayerBullet();
			}
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_Z]) {
		if (!player->isJumping) {
			player->jump();
		}

	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		SpriteSheet playerSprite = SpriteSheet(charactersheet, 2.0f / 55.0f, 72.0f / 142.0f, 51.0f / 55.0f, 68.0f / 142.0f);
		player->sprite = playerSprite;
		if (player->x < 1.33){
			player->setWalkRight();
		}
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		SpriteSheet playerSprite = SpriteSheet(charactersheet, 2.0f / 55.0f, 2.0f / 142.0f, 51.0f / 55.0f, 68.0f / 142.0f);
		player->sprite = playerSprite;
		if (player->x > -1.33){
			player->setWalkLeft();
		}
	}
	else {
		player->setIdle();
	}

	if (enemySpawnTimer > 1.0f && enemyIndex < 4) {
		SpriteSheet enemySprite = SpriteSheet(enemysheet, 2.0f / 71.0f, 2.0f / 69.0f, 71.0f / 71.0f, 69.0f / 69.0f);
		enemies[enemyIndex].sprite = enemySprite;
		enemies[enemyIndex].y = 0.85f;
		enemies[enemyIndex].scale = 0.1f;
		enemies[enemyIndex].x = 0.65f;
		enemies[enemyIndex].width = 0.2f;
		enemies[enemyIndex].height = 0.2f;
		enemies[enemyIndex].acceleration_x = -1.0f;
		entities.push_back(&enemies[enemyIndex]);
		enemyIndex++;
		enemySpawnTimer = 0.0f;
	}

	playerBullet.Update(elapsed);

	enemySpawnTimer += elapsed;
}
void platformer::fixedUpdate() {

	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->collidedBottom) {
			entities[i]->isJumping = false;
			entities[i]->velocity_y = 0.0f;
		}
		if (entities[i]->collidedTop)
			entities[i]->velocity_y = 0.0f;
		if (entities[i]->collidedLeft)
			entities[i]->velocity_x = 0.0f;
		if (entities[i]->collidedRight)
			entities[i]->velocity_x = 0.0f;

		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;
		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j]) {
					float yPenetration = fabs(fabs(entities[j]->y - entities[i]->y) - entities[i]->height / 2.0f - entities[j]->height / 2.0f);
					if (entities[i]->y > entities[j]->y) {
						entities[i]->y += yPenetration + 0.001f;
						entities[i]->collidedBottom = true;
					}
					else if (entities[i]->y < entities[j]->y) {
						entities[i]->y -= yPenetration + 0.001f;
						entities[i]->collidedTop = true;
					}
				}
			}
		}

		entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;
		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j]) {
					float xPenetration = fabs(fabs(entities[j]->x - entities[i]->x) - entities[i]->width / 2.0f - entities[j]->width / 2.0f);
					if (entities[i]->x > entities[j]->x) {
						entities[i]->x += xPenetration + 0.001f;
						entities[i]->collidedLeft = true;
					}
					else if (entities[i]->x < entities[j]->x) {
						entities[i]->x -= xPenetration + 0.001f;
						entities[i]->collidedRight = true;
					}
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {

		if (enemies[i].x < -1.33){
			enemies[i].collidedLeft = true;
		}
		if (enemies[i].x > 1.33){
			enemies[i].collidedRight = true;
		}

		if (enemies[i].collidedRight) {
			enemies[i].acceleration_x = -1.0f;
		}

		if (enemies[i].collidedLeft) {
			enemies[i].acceleration_x = 1.0f;
		}
		if (enemies[i].collidesWith(&playerBullet) && playerBullet.visible) {
			playerBullet.visible = false;
			score++;
			enemies[i].y = 1.3f;
			enemies[i].x = 0.0f;
		}

	}

	if (playerBullet.x > 1.33 || playerBullet.x < -1.33){
		playerBullet.visible = false;
	}

	if (player->x < -1.3 || player->x > 1.3){
		player->setIdle();
	}
}
void platformer::render() {
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
void platformer::renderMainMenu() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.7f, 0.0f);
	DrawText(fontSheetTexture, "Platformer", 0.1, 0.0, 1.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "Press Arrow Keys to Move", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Press Z to jump", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.2f, 0.0f);
	DrawText(fontSheetTexture, "Press X to shoot", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to start the game", 0.05, 0.0, 1.0, 1.0, 0.0, 1.0);
}
void platformer::renderGameLevel() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	playerBullet.Render();

	glLoadIdentity();
	glTranslatef(0.5f, 0.5f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.05, 0.0, 1.0, 1.0, 0.0, 1.0);
}
bool platformer::updateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		fixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	update(elapsed);
	render();
	return done;
}