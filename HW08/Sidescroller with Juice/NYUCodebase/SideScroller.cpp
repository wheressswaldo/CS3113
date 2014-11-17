//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller REMADE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SideScroller.h"

float SideScroller::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void SideScroller::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
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

SideScroller::SideScroller() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Side-Scroller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.33, 2.33, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);

	// general values
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	isShaking = true;

	// setting gravity
	gravity_x = 0.0f;
	gravity_y = -9.8f;

	// loading textures
	layoutSpriteSheetTexture = LoadTexture("sheet_4.png");
	characterSpriteSheetTexture = LoadTexture("characters_1.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	// setting state
	state = STATE_MAIN_MENU;

	Init();
}

SideScroller::~SideScroller() {
	SDL_Quit();
}

void SideScroller::Init() {
	// insert player as the first entity
	SpriteSheet playerSprite = SpriteSheet(characterSpriteSheetTexture, 12, 8, 18);
	Entity* player = new Entity();
	player->sprite = playerSprite;
	player->x = 0.0f;
	player->y = -2.5f;
	player->width = 0.2f;
	player->height = 0.2f;
	player->friction_x = 3.0f;

	entities.push_back(player);

	testParticleSystem.x = player->x;
	testParticleSystem.y = player->y;

	// build the level, aka read tile file and throw into leveldata
	buildLevel();
}

void SideScroller::buildLevel() {
	// read tile file
	ifstream infile("simpleLevel.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
	}
}

bool SideScroller::readHeader(ifstream& stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool SideScroller::readLayerData(ifstream& stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val;
					}
					else {
						levelData[y][x] = 0;
					}
				}

			}
		}
	}
	return true;
}

void SideScroller::Update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		updateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
		break;
	}
}

void SideScroller::updateMainMenu(float elapsed) {
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

void SideScroller::updateGameLevel(float elapsed) {
	// keyboard controls
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_Z]) {
		if (!entities[0]->isJumping) {
			entities[0]->jump();
			isShaking = false;
		}

	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		if (!entities[0]->collidedRight){
			SpriteSheet playerSprite = SpriteSheet(characterSpriteSheetTexture, 12, 8, 30);
			entities[0]->sprite = playerSprite;
			entities[0]->walkRight();
		}
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		if (!entities[0]->collidedLeft){
			SpriteSheet playerSprite = SpriteSheet(characterSpriteSheetTexture, 12, 8, 18);
			entities[0]->sprite = playerSprite;
			entities[0]->walkLeft();
		}
	}
	else {
		entities[0]->idle();
	}

	perlinValue += elapsed;
	if (screenShake > 0) {
		screenShakeValue += elapsed;
		// how long it shakes
		screenShake -= 0.5;
	}

	testParticleSystem.faceLeft = entities[0]->faceLeft;
	if (testParticleSystem.faceLeft == true){
		testParticleSystem.x = entities[0]->x + 0.15;
	}
	else {
		testParticleSystem.x = entities[0]->x - 0.15;
	}
	testParticleSystem.y = entities[0]->y;
	testParticleSystem.Update(elapsed);
}

void SideScroller::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {

		// handle collisions
		// collide with something on bottom
		if (entities[i]->collidedBottom) {
			entities[i]->isJumping = false;
			entities[i]->velocity_y = 0.0f;
		}

		// collide with something on top
		if (entities[i]->collidedTop){
			entities[i]->velocity_y = 0.0f;
		}

		// collide with something on left
		if (entities[i]->collidedLeft){
			entities[i]->velocity_x = 0.0f;
		}

		// collide with something on right
		if (entities[i]->collidedRight){
			entities[i]->velocity_x = 0.0f;
		}

		// reset
		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		// apply gravity
		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		// apply friction
		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		// apply acceleration
		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		// update y values
		entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;

		// collision checks for y between fellow entities
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
		// handle collision Y with tile
		collisionY(entities[i]);

		// update x values
		entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;

		// collision checks for x between fellow entities
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
		// handle collision X with tile
		collisionX(entities[i]);
	}
}

void SideScroller::Render() {
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

void SideScroller::renderMainMenu() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.7f, 0.0f);
	DrawText(fontSheetTexture, "SideScroller", 0.1, 0.0, 1.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "Press Arrow Keys to Move", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Press Z to jump", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.8f, -0.7f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to start the game", 0.05, 0.0, 1.0, 1.0, 0.0, 1.0);
}

void SideScroller::renderGameLevel() {
	loadLevel();

	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// setting limits on view
	float xloc = -entities[0]->x;
	float yloc = -entities[0]->y;

	if (yloc > 1.5){
		yloc = 1.5;
	}
	if (xloc > 8.0){
		xloc = 8.0;
	}
	if (xloc < -8.0){
		xloc = -8.0;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xloc, yloc, 0.0f);
	
	// shake after landing
	glTranslatef(sin(screenShakeValue * screenShakeSpeed) * screenShakeIntensity, 0.0f, 0.0f);
	// perlin cam, may make nausea
	glTranslatef(noise1(perlinValue), noise1(perlinValue + 10.0f), 0.0);

	// render level
	glBindTexture(GL_TEXTURE_2D, layoutSpriteSheetTexture);
	glEnable(GL_TEXTURE_2D);

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

	glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

	// render entities
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	testParticleSystem.Render();
}

bool SideScroller::UpdateAndRender() {
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

void SideScroller::loadLevel() {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float blacklines = 0.003f;
			if (levelData[y][x] != 0){
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u + blacklines, v + blacklines,
					u + blacklines, v + spriteHeight - blacklines,
					u + spriteWidth - blacklines, v + spriteHeight - blacklines,
					u + spriteWidth - blacklines, v + blacklines
				});
			}
		}
	}

}

bool SideScroller::isSolid(unsigned char tile) {
	// says whether or not its solid based on level data
	switch (tile) {
	// 9 is floor
	case 9:
		return true;
		break;
	// 25 is box
	case 25:
		return true;
		break;
	// rest are trees
	default:
		return false;
		break;
	}
}

void SideScroller::worldToTileCoordinates(float worldX, float worldY, int*gridX, int*gridY) {
	*gridX = (int)(((worldX * 16.0f) / TILE_SIZE) + (128.0f * 16.0f / 2.0f)) / 16.0f;
	*gridY = (int)(((worldY * 16.0f) / -TILE_SIZE) + (32.0f * 16.0f / 2.0f)) / 16.0f;
}

// simple collision checks
// if colliding, move player to diff location, in this case, right next to the tile
void SideScroller::collisionX(Entity *entity) {
	int tileX;
	int tileY;

	// left
	worldToTileCoordinates(entity->x - entity->width*0.5, entity->y, &tileX, &tileY);
	if (isSolid(levelData[tileY][tileX])) {
		entity->x = -6.3f;
		entity->collidedLeft = true;
	}
	else {
		entity->collidedLeft = false;
	}

	// right
	worldToTileCoordinates(entity->x + entity->width*0.5, entity->y, &tileX, &tileY);
	if (isSolid(levelData[tileY][tileX])) {
		entity->x = 9.1f;
		entity->collidedRight = true;
	}
	else {
		entity->collidedRight = false;
	}
}

void SideScroller::collisionY(Entity *entity) {
	int tileX;
	int tileY;
	worldToTileCoordinates(entity->x, entity->y - entity->height*0.5, &tileX, &tileY);

	if (isSolid(levelData[tileY][tileX])) {
		entity->y = -2.5f;
		entity->collidedBottom = true;

		if (isShaking == false){
			screenShake = 10;
		}

		isShaking = true;
	}
}
