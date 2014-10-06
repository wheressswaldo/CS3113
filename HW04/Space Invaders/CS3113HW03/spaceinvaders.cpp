//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SpaceInvaders.h"
#include <iostream>

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
	SpriteSheet bulletSprite = SpriteSheet(spriteSheetTexture, 157.0f / 256.0f, 418.0f / 512.0f, 1.0f / 256.0f, 20.0f / 512.0f);

	playerBullet.sprite = bulletSprite;
	playerBullet.visible = true;
	playerBullet.x = entities[0]->getX();
	playerBullet.y = entities[0]->getY();
	playerBullet.scale = 0.5f;
	playerBullet.rotation = 0.0f;
	playerBullet.speed = 3.0f;
	playerBullet.playerBullet = true;
}
void SpaceInvaders::shootEnemyBullet(int shooter) {
	SpriteSheet bulletSprite = SpriteSheet(spriteSheetTexture, 130.0f / 256.0f, 70.0f / 512.0f, 1.0f / 256.0f, 20.0f / 512.0f);

	enemyBullets[enemyBulletIndex].sprite = bulletSprite;
	enemyBullets[enemyBulletIndex].visible = true;
	enemyBullets[enemyBulletIndex].x = entities[shooter]->getX();
	enemyBullets[enemyBulletIndex].y = entities[shooter]->getY();
	enemyBullets[enemyBulletIndex].scale = 0.5f;
	enemyBullets[enemyBulletIndex].rotation = 180.0f;
	enemyBullets[enemyBulletIndex].speed = -1.0f;
	enemyBullets[enemyBulletIndex].playerBullet = false;

	enemyBulletIndex++;
	if (enemyBulletIndex > 5 - 1) {
		enemyBulletIndex = 0;
	}
}
void SpaceInvaders::init() {
	enemyBulletIndex = 0;
	score = 0;
	delay = 0;
	spriteDeathDelay = 0;
	playerDeathDelay = 0;
	playerDead = false;

	playerBullet.visible = FALSE;

	for (size_t i = 0; i < 5; i++) {
		enemyBullets[i].visible = FALSE;
	}
	for (size_t i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	for (size_t i = 0; i < defences.size(); i++) {
		delete defences[i];
	}
	defences.clear();
	entities.clear();
	
	spriteSheetTexture = LoadTexture("testsheet.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	SpriteSheet playerSprite = SpriteSheet(spriteSheetTexture, 100.0f / 256.0f, 2.0f / 512.0f, 104.0f / 256.0f, 64.0f / 512.0f);
	Player* player = new Player(playerSprite, 0.3f, 0.0f, -0.85f, 0.0f, 1.0f);
	entities.push_back(player);

	SpriteSheet enemySprite = SpriteSheet(spriteSheetTexture, 92.0f / 256.0f, 200.0f / 512.0f, 64.0f / 256.0f, 64.0f / 512.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.18f) {
		for (float j = 0.85f; j > 0.65f; j = j - 0.15f) {
			Entity* enemy = new Entity(enemySprite, 0.25f, i, j, 0.0f, 1.0f, 20, 1, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	enemySprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 200.0f / 512.0f, 88.0f / 256.0f, 64.0f / 512.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.55f; j > 0.35f; j = j - 0.15f) {
			Entity* enemy = new Entity(enemySprite, 0.25f, i, j, 0.0f, 1.0f, 10, 1, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	enemySprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 134.0f / 512.0f, 88.0f / 256.0f, 64.0f / 512.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.25f; j > 0.15f; j = j - 0.15f) {
			Entity* enemy = new Entity(enemySprite, 0.25f, i, j, 0.0f, 1.0f, 5, 1, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	enemySprite = SpriteSheet(spriteSheetTexture, 87.0f / 256.0f, 68.0f / 512.0f, 96.0f / 256.0f, 64.0f / 512.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.1f; j > 0.0f; j = j - 0.15f) {
			Entity* enemy = new Entity(enemySprite, 0.25f, i, j, 0.0f, 1.0f, 1, 1, 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

	SpriteSheet defenceSprite = SpriteSheet(spriteSheetTexture, 107.0f / 256.0f, 418.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);

	for (float i = -0.9f; i < 1.0f; i = i + 0.6f){
		Entity* defence = new Entity(defenceSprite, 0.4f, i, -0.6f, 0.0f, 0.0f, 5, 6, 0.0f, 0.0f);
		defences.push_back(defence);
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
				if (!playerBullet.visible)
					shootPlayerBullet();
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_R) {
				state = STATE_GAME_OVER;
				if (highScore < score){
					highScore = score;
				}
				roundScore = score;
				score = 0;
				init();
			}
		}
	}
	if (!playerDead){
		for (size_t i = 1; i < entities.size(); i++) {

			if (entities[i]->getY() < -0.45f) {
				playerDead = true;
				playerDeathDelay = 60;
			}

			delay = delay - elapsed;
			if ((entities[i]->getX() > 1.2 || entities[i]->getX() < -1.2) && delay <= 0) {
				for (size_t k = 1; k < entities.size(); k++) {
					entities[k]->direction_x = -entities[k]->direction_x;
					entities[k]->speed += 0.08f;
					entities[k]->y = entities[k]->y - 0.008f;
				}
				delay = 1;
				break;
			}

			if (entities[i]->dead == true){
				spriteDeathDelay--;
				if (spriteDeathDelay == 0){
					delete entities[i];
					entities.erase(entities.begin() + i);
					break;
				}
			}

			if (playerBullet.visible && isColliding(*entities[i], playerBullet) && playerBullet.playerBullet) {
				playerBullet.visible = false;
				score += entities[i]->score;
				entities[i]->dead = true;
				SpriteSheet testSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 68.0f / 512.0f, 83.0f / 256.0f, 52.0f / 512.0f);
				entities[i]->sprite = testSprite;
				spriteDeathDelay = 5;
				break;
			}
		}

		int temp = 1 + (rand() % (int)(100 - 1 + 1));
		if (temp == 50){
			int randomShooter = 0 + (rand() % (int)(entities.size() - 0 + 0));
			if (randomShooter != 0){
				shootEnemyBullet(randomShooter);
			}
		}
		if (playerBullet.y > 1){
			playerBullet.visible = false;
		}

		for (size_t j = 0; j < 5; j++){
			if (enemyBullets[j].visible && isColliding(*entities[0], enemyBullets[j]) && !enemyBullets[j].playerBullet) {
				entities[0]->sprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 2.0f / 512.0f, 96.0f / 256.0f, 51.0f / 512.0f);
				playerDead = true;
				playerDeathDelay = 60;
			}
		}

		for (size_t i = 0; i < defences.size(); i++){
			for (size_t j = 0; j < 5; j++){
				if (enemyBullets[j].visible && isColliding(*defences[i], enemyBullets[j])) {
					enemyBullets[j].visible = false;
					defences[i]->HP -= 1;
					if (defences[i]->HP <= 0){
						delete defences[i];
						defences.erase(defences.begin() + i);
						break;
					}
					if (defences[i]->HP == 5){
						SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 418.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
						defences[i]->sprite = tempSprite;
					}
					else if (defences[i]->HP == 4){
						SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 107.0f / 256.0f, 342.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
						defences[i]->sprite = tempSprite;
					}
					else if (defences[i]->HP == 3){
						SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 342.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
						defences[i]->sprite = tempSprite;
					}
					else if (defences[i]->HP == 2){
						SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 107.0f / 256.0f, 266.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
						defences[i]->sprite = tempSprite;
					}
					else if (defences[i]->HP == 1){
						SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 266.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
						defences[i]->sprite = tempSprite;
					}

					break;
				}
			}

			if (playerBullet.visible && isColliding(*defences[i], playerBullet)){
				playerBullet.visible = false;
				defences[i]->HP -= 1;
				if (defences[i]->HP <= 0){
					delete defences[i];
					defences.erase(defences.begin() + i);
					break;
				}
				if (defences[i]->HP == 5){
					SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 418.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
					defences[i]->sprite = tempSprite;
				}
				else if (defences[i]->HP == 4){
					SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 107.0f / 256.0f, 342.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
					defences[i]->sprite = tempSprite;
				}
				else if (defences[i]->HP == 3){
					SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 342.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
					defences[i]->sprite = tempSprite;
				}
				else if (defences[i]->HP == 2){
					SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 107.0f / 256.0f, 266.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
					defences[i]->sprite = tempSprite;
				}
				else if (defences[i]->HP == 1){
					SpriteSheet tempSprite = SpriteSheet(spriteSheetTexture, 2.0f / 256.0f, 266.0f / 512.0f, 103.0f / 256.0f, 74.0f / 512.0f);
					defences[i]->sprite = tempSprite;
				}
				break;
			}
		}


		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->Update(elapsed);
		}

		playerBullet.Update(elapsed);

		for (size_t i = 0; i < 5; i++) {
			enemyBullets[i].Update(elapsed);
		}
		if (entities.size() <= 1) {
			playerDead = true;
			playerDeathDelay = 60;
		}
	}
	else {
		if (playerDeathDelay == 0){
			state = STATE_GAME_OVER;
			if (highScore < score){
				highScore = score;
			}
			roundScore = score;
			score = 0;
			init();
		}
		else{
			playerDeathDelay--;
		}
	}
}
void SpaceInvaders::updateGameOver(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
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
	glTranslatef(-0.8f, 0.7f, 0.0f);
	DrawText(fontSheetTexture, "Space Invaders", 0.1, 0.0, 0.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "Press Arrow Keys to Move", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to shoot", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to start the game", 0.05, 0.0, 0.0, 1.0, 0.0, 1.0);
}
void SpaceInvaders::renderGameLevel() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	playerBullet.Render();

	for (size_t i = 0; i < 5; i++) {
		enemyBullets[i].Render();
	}

	for (size_t i = 0; i < defences.size(); i++){
		defences[i]->Render();
	}

	glLoadIdentity();
	glTranslatef(0.8f, -0.9f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.05, 0.0, 0.0, 1.0, 0.0, 1.0);
}
void SpaceInvaders::renderGameOver() {
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glTranslatef(-0.8f, 0.7f, 0.0f);
	DrawText(fontSheetTexture, "GAME OVER", 0.1, 0.0, 0.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "High Score: " + to_string(highScore), 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Round Score: " + to_string(roundScore), 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press Z to go back to main menu", 0.05, 0.0, 0.0, 1.0, 0.0, 1.0);
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