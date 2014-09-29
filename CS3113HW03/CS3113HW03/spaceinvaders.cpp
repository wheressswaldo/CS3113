//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SpaceInvaders.h"

SpaceInvaders::SpaceInvaders() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	
	done = false;
	lastFrameTicks = 0.0f;
	state = STATE_MAIN_MENU;
	score = 0;
	init();
}
void SpaceInvaders::shootPlayerBullet() {
	SpriteSheet bulletSprite = SpriteSheet(spriteSheetTexture, 849.0f / 1024.0f, 364.0f / 1024.0f, 9.0f / 1024.0f, 57.0f / 1024.0f);

	bullets[playerBulletIndex].sprite = bulletSprite;
	bullets[playerBulletIndex].visible = true;
	bullets[playerBulletIndex].x = entities[0]->getX();
	bullets[playerBulletIndex].y = entities[0]->getY();
	bullets[playerBulletIndex].scale = 1.0f;
	bullets[playerBulletIndex].rotation = 0.0f;
	bullets[playerBulletIndex].speed = 2.0f;
	bullets[playerBulletIndex].playerBullet = true;

	playerBulletIndex++;
	if (playerBulletIndex > MAX_BULLETS - 1) {
		playerBulletIndex = 0;
	}
	shootTimer = 0;
}
void SpaceInvaders::shootEnemyBullet(int shooter) {
	SpriteSheet bulletSprite = SpriteSheet(spriteSheetTexture, 849.0f / 1024.0f, 364.0f / 1024.0f, 9.0f / 1024.0f, 57.0f / 1024.0f);

	enemyBullets[enemyBulletIndex].sprite = bulletSprite;
	enemyBullets[enemyBulletIndex].visible = true;
	enemyBullets[enemyBulletIndex].x = entities[shooter]->getX();
	enemyBullets[enemyBulletIndex].y = entities[shooter]->getY();
	enemyBullets[enemyBulletIndex].scale = 1.0f;
	enemyBullets[enemyBulletIndex].rotation = 180.0f;
	enemyBullets[enemyBulletIndex].speed = -1.0f;
	enemyBullets[enemyBulletIndex].playerBullet = false;

	enemyBulletIndex++;
	if (enemyBulletIndex > 5 - 1) {
		enemyBulletIndex = 0;
	}
	shootTimer = 0;
}
void SpaceInvaders::init() {
	playerBulletIndex = 0;
	enemyBulletIndex = 0;
	shootTimer = 0.3f;
	score = 0;

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].visible = FALSE;
	}
	for (size_t i = 0; i < 5; i++) {
		enemyBullets[i].visible = FALSE;
	}
	for (size_t i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
	
	spriteSheetTexture = LoadTexture("sheet3.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	SpriteSheet playerSprite = SpriteSheet(spriteSheetTexture, 10.0f / 1024.0f, 791.0f / 1024.0f, 112.0f / 1024.0f, 75.0f / 1024.0f);
	Player* player = new Player(playerSprite, 0.9f, 0.0f, -0.85f, 0.0f, 1.0f);
	entities.push_back(player);

	SpriteSheet enemySprite = SpriteSheet(spriteSheetTexture, 10.0f / 1024.0f, 520.0f / 1024.0f, 120.0f / 1024.0f, 98.0f / 1024.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.85f; j > 0.65f; j = j - 0.2f) {
			Entity* enemy = new Entity(enemySprite, 0.7f, i, j, 0.0f, 1.0f, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	enemySprite = SpriteSheet(spriteSheetTexture, 10.0f / 1024.0f, 618.0f / 1024.0f, 120.0f / 1024.0f, 98.0f / 1024.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.45f; j > 0.35f; j = j - 0.2f) {
			Entity* enemy = new Entity(enemySprite, 0.7f, i, j, 0.0f, 1.0f, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	enemySprite = SpriteSheet(spriteSheetTexture, 10.0f / 1024.0f, 716.0f / 1024.0f, 112.0f / 1024.0f, 75.0f / 1024.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.25f; j > 0.15f; j = j - 0.2f) {
			Entity* enemy = new Entity(enemySprite, 0.7f, i, j, 0.0f, 1.0f, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	enemySprite = SpriteSheet(spriteSheetTexture, 10.0f / 1024.0f, 412.0f / 1024.0f, 133.0f / 1024.0f, 108.0f / 1024.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.05f; j > 0.0f; j = j - 0.2f) {
			Entity* enemy = new Entity(enemySprite, 0.7f, i, j, 0.0f, 1.0f, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

}
SpaceInvaders::~SpaceInvaders() {
	SDL_Quit();
}
void SpaceInvaders::update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		updateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
		break;
	case STATE_GAME_OVER:
		updateGameOver(elapsed);
		break;
	}
}
void SpaceInvaders::updateMainMenu(float elapsed) {
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
bool SpaceInvaders::isColliding(const Entity& e1, const Bullet& e2) {
	float e1_y_max = e1.y + e1.sprite.height / 2;
	float e1_y_min = e1.y - e1.sprite.height / 2;
	float e1_x_max = e1.x + e1.sprite.width / 2;
	float e1_x_min = e1.x - e1.sprite.width / 2;

	float e2_y_max = e2.y + e2.sprite.height / 2;
	float e2_y_min = e2.y - e2.sprite.height / 2;
	float e2_x_max = e2.x + e2.sprite.width / 2;
	float e2_x_min = e2.x - e2.sprite.width / 2;

	if (e1_y_min > e2_y_max)
		return false;
	if (e1_y_max < e2_y_min)
		return false;
	if (e1_x_min > e2_x_max)
		return false;
	if (e1_x_max < e2_x_min)
		return false;

	return true;
}
void SpaceInvaders::updateGameLevel(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (shootTimer > 0.1f)
					shootPlayerBullet();
			}
		}
	}
	
	for (size_t i = 1; i < entities.size(); i++) {

		if (entities[i]->getY() < -0.7f) {
			state = STATE_GAME_OVER;
			if (highScore < score){
				highScore = score;
			}
			score = 0;
			init();
		}

		if (entities[i]->getX() > 1.2 || entities[i]->getX() < -1.2) {
			for (size_t k = 1; k < entities.size(); k++) {
				entities[k]->direction_x = -entities[k]->direction_x;
			}
			break;
		}

		for (size_t j = 0; j < MAX_BULLETS; j++) {
			if (bullets[j].visible && isColliding(*entities[i], bullets[j]) && bullets[j].playerBullet) {
				bullets[j].visible = false;
				delete entities[i];
				entities.erase(entities.begin() + i);
				if (i < 12 && i > 0){
					score += 10;
				}
				else if (i < 23 && i > 11) {
					score += 10;
				}
				else if (i < 34 && i > 22){
					score += 5;
				}
				else{
					score += 1;
				}
				break;
			}
		}
	}
	int temp = 1 + (rand() % (int)(5000 - 1 + 1));
	if (temp == 568){
		int randomShooter = 0 + (rand() % (int)(entities.size() - 0 + 0));
		if (randomShooter != 0){
			shootEnemyBullet(randomShooter);
		}
	}
	for (size_t j = 0; j < 5; j++){
		if (enemyBullets[j].visible && isColliding(*entities[0], enemyBullets[j]) && !enemyBullets[j].playerBullet) {
			state = STATE_GAME_OVER;
			if (highScore < score){
				highScore = score;
			}
			score = 0;
			init();
		}
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	for (size_t i = 0; i < 5; i++) {
		enemyBullets[i].Update(elapsed);
	}
	//stage cleared
	if (entities.size() <= 1) {
		state = STATE_GAME_OVER;
		if (highScore < score){
			highScore = score;
		}
		init();
	}

	shootTimer += elapsed;
}
void SpaceInvaders::updateGameOver(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_MAIN_MENU;
			}
		}
	}
}
void SpaceInvaders::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
		case STATE_MAIN_MENU:
			renderMainMenu();
			break;
		case STATE_GAME_LEVEL:
			renderGameLevel();
			break;
		case STATE_GAME_OVER:
			renderGameOver();
			break;
	}
	SDL_GL_SwapWindow(displayWindow);
}
void SpaceInvaders::renderMainMenu() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.66f, 0.8f, 0.0f);
	DrawText(fontSheetTexture, "Space Invaders", 0.1, 0.0, 0.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glLoadIdentity();
	glTranslatef(-0.5f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to start", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
void SpaceInvaders::renderGameLevel() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
	for (size_t i = 0; i < 5; i++) {
		enemyBullets[i].Render();
	}
	glLoadIdentity();
	glTranslatef(0.8f, -0.9f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.05, 0.0, 0.0, 1.0, 0.0, 1.0);
}
void SpaceInvaders::renderGameOver() {
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glTranslatef(-0.4f, 0.8f, 0.0f);
	DrawText(fontSheetTexture, "GAME OVER", 0.1, 0.0, 0.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.3f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "High Score: " + to_string(highScore), 0.05, 0.0, 0.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.85f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to go back to main menu", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
bool SpaceInvaders::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	update(elapsed);
	render();
	return done;
}
void SpaceInvaders::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
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