//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		GlowHockey
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GlowHockey.h"

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void GlowHockey::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
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

float GlowHockey::generateRandomNumber(float min, float max) {
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

GlowHockey::GlowHockey() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	displayWindow = SDL_CreateWindow("GlowHockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	Init();
}

GlowHockey::~GlowHockey() {
	SDL_JoystickClose(playerOneController);
	SDL_Quit();
}

void GlowHockey::Init() {
	// clearing vector
	entities.clear();

	// general values	
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	// setting gravity
	gravity_x = 0.0f;
	gravity_y = 0.0f;

	// loading textures
	glowSheet = LoadTexture("sprites.png", false);
	fontSheetTexture = LoadTexture("pixel_font.png", true);

	// setting state
	state = STATE_MAIN_MENU;

	// setting score
	player1Score = 0;
	player2Score = 0;

	// particles
	particleCount = 15;
	p = ParticleSystem(particleCount);

	// ai
	AIDifficulty = 1;
	aiOn = true;

	// menu control
	menuControl = 1;

	// controller
	playerOneController = SDL_JoystickOpen(0);

	// music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	hitWall = Mix_LoadWAV("HitWall.wav");
	hitPuck = Mix_LoadWAV("HitPuck.wav");
	moveMenu = Mix_LoadWAV("MoveMenu.wav");
	selectMenu = Mix_LoadWAV("SelectMenu.wav");
	cancelMenu = Mix_LoadWAV("CancelMenu.wav");
	goal = Mix_LoadWAV("Goal.wav");

	volumeLevels = 20;

	Mix_VolumeChunk(hitWall, volumeLevels);
	Mix_VolumeChunk(hitPuck, volumeLevels);
	Mix_VolumeChunk(moveMenu, volumeLevels);
	Mix_VolumeChunk(selectMenu, volumeLevels);
	Mix_VolumeChunk(cancelMenu, volumeLevels);
	Mix_VolumeChunk(goal, (volumeLevels - 10));

	// insert player as the first entity
	SpriteSheet player1Sprite = SpriteSheet(glowSheet, 178.0f / 498.0f, 348.0f / 498.0f, 100.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet glow1Sprite = SpriteSheet(glowSheet, 353.0f / 498.0f, 0.0f / 498.0f, 142.0f / 498.0f, 142.0f / 498.0f);
	player1 = new Entity();
	player1->sprite = player1Sprite;
	player1->glow = glow1Sprite;
	player1->glowing = true;
	player1->scale_x = 0.6f;
	player1->scale_y = 0.6f;
	player1->x = 0.0f;
	player1->y = 0.75f;
	player1->friction_x = 3.0f;
	player1->friction_y = 3.0f;
	player1->isPlayer1 = true;
	player1->collided = false;
	// 1 is green
	player1->cColor = 1;

	// insert player 2 as second entity
	SpriteSheet player2Sprite = SpriteSheet(glowSheet, 281.0f / 498.0f, 348.0f / 498.0f, 100.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet glow2Sprite = SpriteSheet(glowSheet, 178.0f / 498.0f, 203.0f / 498.0f, 142.0f / 498.0f, 142.0f / 498.0f);
	player2 = new Entity();
	player2->sprite = player2Sprite;
	player2->glow = glow2Sprite;
	player2->glowing = true;
	player2->scale_x = 0.6f;
	player2->scale_y = 0.6f;
	player2->x = 0.0f;
	player2->y = -0.5f;
	player2->friction_x = 3.0f;
	player2->friction_y = 3.0f;
	player2->isPlayer2 = true;
	player2->collided = false;
	// 2 is purple
	player2->cColor = 2;

	SpriteSheet puckSprite = SpriteSheet(glowSheet, 384.0f / 498.0f, 290.0f / 498.0f, 100.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet puckGlowSprite = SpriteSheet(glowSheet, 353.0f / 498.0f, 145.0f / 498.0f, 142.0f / 498.0f, 142.0f / 498.0f);
	puck = new Entity();
	puck->sprite = puckSprite;
	puck->glow = puckGlowSprite;
	puck->glowing = true;
	puck->scale_x = 0.5f;
	puck->scale_y = 0.5f;
	puck->x = 0.0f;
	puck->y = 0.0f;
	puck->velocity_x = 0.0f;
	puck->velocity_y = 0.0f;
	puck->friction_x = 0.7f;
	puck->friction_y = 0.7f;
	puck->isPuck = true;
	puck->collided = false;

	controlIndicator = new Entity();
	controlIndicator->sprite = player2Sprite;
	controlIndicator->glow = glow2Sprite;
	controlIndicator->glowing = true;
	controlIndicator->scale_x = 0.5f;
	controlIndicator->scale_y = 0.5f;
	controlIndicator->x = 0.0f;
	controlIndicator->y = 0.0f;
	controlIndicator->friction_x = 3.0f;
	controlIndicator->friction_y = 3.0f;
	controlIndicator->isPlayer2 = true;
	controlIndicator->collided = false;

	// set up board
	SpriteSheet redSprite = SpriteSheet(glowSheet, 143.0f / 498.0f, 451.0f / 498.0f, 100.0f / 498.0f, 20.0f / 498.0f);
	SpriteSheet redGlowSprite = SpriteSheet(glowSheet, 0.0f / 498.0f, 409.0f / 498.0f, 140.0f / 498.0f, 40.0f / 498.0f);
	SpriteSheet yellowSprite = SpriteSheet(glowSheet, 143.0f / 498.0f, 474.0f / 498.0f, 100.0f / 498.0f, 20.0f / 498.0f);
	SpriteSheet yellowGlowSprite = SpriteSheet(glowSheet, 0.0f / 498.0f, 452.0f / 498.0f, 140.0f / 498.0f, 40.0f / 498.0f);

	// center
	SpriteSheet centerSprite = SpriteSheet(glowSheet, 0.0f / 498.0f, 0.0f / 498.0f, 350.0f / 498.0f, 200.0f / 498.0f);

	// player circles
	SpriteSheet bottomCircleSprite = SpriteSheet(glowSheet, 0.0f / 498.0f, 306.0f / 498.0f, 175.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet topCircleSprite = SpriteSheet(glowSheet, 0.0f / 498.0f, 203.0f / 498.0f, 175.0f / 498.0f, 100.0f / 498.0f);

	Entity* center = new Entity();
	center->sprite = centerSprite;
	center->scale_x = 0.95f;
	center->scale_y = 0.95f;
	center->x = 0.0f;
	center->y = 0.0f;
	center->friction_x = 3.0f;
	center->friction_y = 3.0f;
	center->isBackground = true;
	entities.push_back(center);

	Entity* topCircle = new Entity();
	topCircle->sprite = topCircleSprite;
	topCircle->scale_x = 0.8f;
	topCircle->scale_y = 0.8f;
	topCircle->x = 0.0f;
	topCircle->y = 0.7f;
	topCircle->friction_x = 3.0f;
	topCircle->friction_y = 3.0f;
	topCircle->isBackground = true;
	entities.push_back(topCircle);

	Entity* bottomCircle = new Entity();
	bottomCircle->sprite = bottomCircleSprite;
	bottomCircle->scale_x = 0.8f;
	bottomCircle->scale_y = 0.8f;
	bottomCircle->x = 0.0f;
	bottomCircle->y = -0.7f;
	bottomCircle->friction_x = 3.0f;
	bottomCircle->friction_y = 3.0f;
	bottomCircle->isBackground = true;
	entities.push_back(bottomCircle);

	Entity* top1 = new Entity();
	top1->sprite = redSprite;
	top1->glow = redGlowSprite;
	top1->glowing = true;
	top1->scale_x = 0.95f;
	top1->scale_y = 0.5f;
	top1->x = -0.35f;
	top1->y = 0.8f;
	top1->friction_x = 3.0f;
	top1->friction_y = 3.0f;
	top1->isWall = true;
	top1->up = true;
	// 3 is red
	top1->cColor = 3;
	entities.push_back(top1);

	Entity* top2 = new Entity();
	top2->sprite = yellowSprite;
	top2->glow = yellowGlowSprite;
	top2->glowing = true;
	top2->scale_x = 0.95f;
	top2->scale_y = 0.5f;
	top2->x = 0.35f;
	top2->y = 0.8f;
	top2->friction_x = 3.0f;
	top2->friction_y = 3.0f;
	top2->isWall = true;
	top2->up = true;
	// 4 is yellow
	top2->cColor = 4;
	entities.push_back(top2);

	Entity* rightside1 = new Entity();
	rightside1->sprite = yellowSprite;
	rightside1->glow = yellowGlowSprite;
	rightside1->glowing = true;
	rightside1->scale_x = 1.4f;
	rightside1->scale_y = 0.5f;
	rightside1->rotation = 1.57f;
	rightside1->x = 0.55f;
	rightside1->y = 0.4f;
	rightside1->friction_x = 3.0f;
	rightside1->friction_y = 3.0f;
	rightside1->isWall = true;
	rightside1->right = true;
	// 4 is yellow
	rightside1->cColor = 4;
	entities.push_back(rightside1);

	Entity* rightside2 = new Entity();
	rightside2->sprite = redSprite;
	rightside2->glow = redGlowSprite;
	rightside2->glowing = true;
	rightside2->scale_x = 1.4f;
	rightside2->scale_y = 0.5f;
	rightside2->rotation = 1.57f;
	rightside2->x = 0.55f;
	rightside2->y = -0.4f;
	rightside2->friction_x = 3.0f;
	rightside2->friction_y = 3.0f;
	rightside2->isWall = true;
	rightside2->right = true;
	// 3 is red
	rightside2->cColor = 3;
	entities.push_back(rightside2);


	Entity* leftside1 = new Entity();
	leftside1->sprite = redSprite;
	leftside1->glow = redGlowSprite;
	leftside1->glowing = true;
	leftside1->scale_x = 1.4f;
	leftside1->scale_y = 0.5f;
	leftside1->rotation = 1.57f;
	leftside1->x = -0.55f;
	leftside1->y = 0.4f;
	leftside1->friction_x = 3.0f;
	leftside1->friction_y = 3.0f;
	leftside1->isWall = true;
	leftside1->left = true;
	// 3 is red
	leftside1->cColor = 3;
	entities.push_back(leftside1);

	Entity* leftside2 = new Entity();
	leftside2->sprite = yellowSprite;
	leftside2->glow = yellowGlowSprite;
	leftside2->glowing = true;
	leftside2->scale_x = 1.4f;
	leftside2->scale_y = 0.5f;
	leftside2->rotation = 1.57f;
	leftside2->x = -0.55f;
	leftside2->y = -0.4f;
	leftside2->friction_x = 3.0f;
	leftside2->friction_y = 3.0f;
	leftside2->isWall = true;
	leftside2->left = true;
	// 4 is yellow
	leftside2->cColor = 4;
	entities.push_back(leftside2);

	Entity* bottom1 = new Entity();
	bottom1->sprite = yellowSprite;
	bottom1->glow = yellowGlowSprite;
	bottom1->glowing = true;
	bottom1->scale_x = 0.95f;
	bottom1->scale_y = 0.5f;
	bottom1->x = -0.35f;
	bottom1->y = -0.8f;
	bottom1->friction_x = 3.0f;
	bottom1->friction_y = 3.0f;
	bottom1->isWall = true;
	bottom1->down = true;
	// 4 is yellow
	bottom1->cColor = 4;
	entities.push_back(bottom1);

	Entity* bottom2 = new Entity();
	bottom2->sprite = redSprite;
	bottom2->glow = redGlowSprite;
	bottom2->glowing = true;
	bottom2->scale_x = 0.95f;
	bottom2->scale_y = 0.5f;
	bottom2->x = 0.35f;
	bottom2->y = -0.8f;
	bottom2->friction_x = 3.0f;
	bottom2->friction_y = 3.0f;
	bottom2->isWall = true;
	bottom2->down = true;
	// 3 is red
	bottom2->cColor = 3;
	entities.push_back(bottom2);

}

void GlowHockey::Update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		UpdateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		UpdateGameLevel(elapsed);
		break;
	case STATE_GAME_OVER:
		UpdateGameOver(elapsed);
		break;
	}
}

void GlowHockey::UpdateMainMenu(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W || 
				event.jbutton.button == 0) {
				if (menuControl <= 4 && menuControl > 1){
					menuControl--;
					Mix_PlayChannel(-1, moveMenu, 0);
					break;
				}
				if (menuControl <= 13 && menuControl > 11){
					menuControl--;
					Mix_PlayChannel(-1, moveMenu, 0);
					break;
				}
				if (menuControl <= 22 && menuControl > 21){
					menuControl--;
					Mix_PlayChannel(-1, moveMenu, 0);
					break;
				}
				break;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_S ||
				event.jbutton.button == 1) {
				if (menuControl < 4 && menuControl >= 1){
					menuControl++;
					Mix_PlayChannel(-1, moveMenu, 0);
					break;
				}
				if (menuControl < 13 && menuControl >= 11){
					menuControl++;
					Mix_PlayChannel(-1, moveMenu, 0);
					break;
				}
				if (menuControl < 22 && menuControl >= 21){
					menuControl++;
					Mix_PlayChannel(-1, moveMenu, 0);
					break;
				}
				break;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_Z || event.key.keysym.scancode == SDL_SCANCODE_RETURN ||
				event.jbutton.button == 10) {
				if (menuControl == 1){
					menuControl = 11;
					Mix_PlayChannel(-1, selectMenu, 0);
					break;
				}
				if (menuControl == 2){
					menuControl = 21;
					Mix_PlayChannel(-1, selectMenu, 0);
					break;
				}
				if (menuControl == 3){
					menuControl = 31;
					Mix_PlayChannel(-1, selectMenu, 0);
					break;
				}
				if (menuControl == 4){
					done = true;
					break;
				}
				if (menuControl == 11){
					Mix_PlayChannel(-1, selectMenu, 0);
					AIDifficulty = 1.5f;
					state = STATE_GAME_LEVEL;
					break;
				}
				if (menuControl == 12){
					Mix_PlayChannel(-1, selectMenu, 0);
					AIDifficulty = 2.0f;
					state = STATE_GAME_LEVEL;
					break;
				}
				if (menuControl == 13){
					Mix_PlayChannel(-1, selectMenu, 0);
					AIDifficulty = 3.0f;
					state = STATE_GAME_LEVEL;
					break;
				}
				if (menuControl == 21){
					Mix_PlayChannel(-1, selectMenu, 0);
					aiOn = false;
					state = STATE_GAME_LEVEL;
					break;
				}
				if (menuControl == 22){
					break;
				}
				break;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_X || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||
				event.jbutton.button == 11) {
				if (menuControl > 5){
					menuControl = 1;
					Mix_PlayChannel(-1, cancelMenu, 0);
					break;
				}
				break;
			}
		}
	}

}

void GlowHockey::UpdateGameOver(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE || event.jbutton.button == 11) {
				Init();
				state = STATE_MAIN_MENU;
				break;
			}
		}
	}

}

void GlowHockey::UpdateGameLevel(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_T) {
				stats = !stats;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_G) {
				state = STATE_GAME_OVER;
			}
		}
		else if (event.type == SDL_JOYBUTTONDOWN){
			if (event.jbutton.button == 8){
				stats = !stats;
			}
			if (event.jbutton.button == 5){
				state = STATE_GAME_OVER;
			}
		}
		else if (event.type == SDL_JOYAXISMOTION){
			if (event.jaxis.which == 0){
				//If the X axis changed
				if (event.jaxis.axis == 0)
				{
					//If the X axis is neutral
					if ((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
					{
						player2->joystick_x = 0.0f;
					}
					//If not
					else
					{
						//Adjust the velocity
						if (event.jaxis.value < 0)
						{
							player2->joystick_x = (float)(event.jaxis.value)/32767.0f;
						}
						else
						{
							player2->joystick_x = (float)(event.jaxis.value)/32767.0f;
						}
					}
				}
				//If the Y axis changed
				if (event.jaxis.axis == 1)
				{
					//If the Y axis is neutral
					if ((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
					{
						player2->joystick_y = 0;
					}
					//If not
					else
					{
						//Adjust the velocity
						if (event.jaxis.value < 0)
						{
							player2->joystick_y = (float)(event.jaxis.value * -1.0f) / 32767.0f;
						}
						else
						{
							player2->joystick_y = (float)(event.jaxis.value * -1.0f) / 32767.0f;
						}
					}
				}
				if (!aiOn){
					//If the X axis changed
					if (event.jaxis.axis == 2)
					{
						//If the X axis is neutral
						if ((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
						{
							player1->joystick_x = 0.0f;
						}
						//If not
						else
						{
							//Adjust the velocity
							if (event.jaxis.value < 0)
							{
								player1->joystick_x = (float)(event.jaxis.value) / 32767.0f;
							}
							else
							{
								player1->joystick_x = (float)(event.jaxis.value) / 32767.0f;
							}
						}
					}
					//If the Y axis changed
					if (event.jaxis.axis == 3)
					{
						//If the Y axis is neutral
						if ((event.jaxis.value > -8000) && (event.jaxis.value < 8000))
						{
							player1->joystick_y = 0;
						}
						//If not
						else
						{
							//Adjust the velocity
							if (event.jaxis.value < 0)
							{
								player1->joystick_y = (float)(event.jaxis.value * -1.0f) / 32767.0f;
							}
							else
							{
								player1->joystick_y = (float)(event.jaxis.value * -1.0f) / 32767.0f;
							}
						}
					}
				}
			}
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) {
		player2->velocity_y = 1.0f;
	}

	if (keys[SDL_SCANCODE_A]){
		player2->velocity_x = -1.0f;
	}

	if (keys[SDL_SCANCODE_S]) {
		player2->velocity_y = -1.0f;
	}

	if (keys[SDL_SCANCODE_D]) {
		player2->velocity_x = 1.0f;
	}

	if (keys[SDL_SCANCODE_UP]) {
		player1->velocity_y = 1.0f;
	}

	if (keys[SDL_SCANCODE_DOWN]) {
		player1->velocity_y = -1.0f;
	}

	if (keys[SDL_SCANCODE_LEFT]){
		player1->velocity_x = -1.0f;
	}

	if (keys[SDL_SCANCODE_RIGHT]) {
		player1->velocity_x = 1.0f;
	}

	if (keys[SDL_SCANCODE_R]){
		puck->x = 0.0f;
		puck->y = 0.0f;
		puck->velocity_x = 0.0f;
		puck->velocity_y = 0.0f;
	}

	if (keys[SDL_SCANCODE_1]){
		particleCount++;
	}

	if (keys[SDL_SCANCODE_2]){
		particleCount--;
	}

	player1->Update(elapsed);
	player2->Update(elapsed);
	puck->Update(elapsed);
	p.Update(elapsed);
}


void GlowHockey::FixedUpdate() {
	if (state == STATE_GAME_LEVEL){
		if (puck->y > 0.8f){
			puck->x = 0.0f;
			puck->y = 0.0f;
			puck->velocity_x = 0.0f;
			puck->velocity_y = 0.0f;
			player2Score++;
			Mix_PlayChannel(-1, goal, 0);
			// p2 score
		}
		if (puck->y < -0.8f){
			puck->x = 0.0f;
			puck->y = 0.0f;
			puck->velocity_x = 0.0f;
			puck->velocity_y = 0.0f;
			player1Score++;
			Mix_PlayChannel(-1, goal, 0);
			// p1 score
		}

		if (player1Score == 7 || player2Score == 7){
			state = STATE_GAME_OVER;
		}

		for (size_t i = 0; i < entities.size(); i++) {
			// collision
			if (entities[i]->isWall) {
				collision(puck, entities[i]);
			}
		}
		collision(player1, puck);
		collision(player2, puck);
		if (aiOn){
			if (puck->velocity_y < 0.0f && puck->y < 0){
				player1->velocity_x = (player1->x * -1.0f) * 3.0f * AIDifficulty;
				player1->velocity_y = (0.8f - player1->y) * 2.0f * AIDifficulty;
				aiState = "Returning";
			}
			else
			{
				tempSpeedX = puck->x - player1->x;
				tempSpeedY = puck->y - player1->y;
				player1->velocity_x = tempSpeedX * 3.0f * AIDifficulty;
				player1->velocity_y = tempSpeedY * 2.0f * AIDifficulty;
				aiState = "Tracking";
			}
		}
		if (!aiOn){
			// apply joystick
			player1->velocity_x = 1.2f * player1->joystick_x;
			player1->velocity_y = 1.2f * player1->joystick_y;
		}

		// apply friction
		player1->velocity_x = lerp(player1->velocity_x, 0.0f, FIXED_TIMESTEP * player1->friction_x);
		player1->velocity_y = lerp(player1->velocity_y, 0.0f, FIXED_TIMESTEP * player1->friction_y);

		// apply velocity
		player1->velocity_x += player1->acceleration_x * FIXED_TIMESTEP;
		player1->velocity_y += player1->acceleration_y * FIXED_TIMESTEP;

		// update x values
		player1->x += player1->velocity_x * FIXED_TIMESTEP;

		// update y values
		player1->y += player1->velocity_y * FIXED_TIMESTEP;

		// apply joystick
		player2->velocity_x = 1.2f * player2->joystick_x;
		player2->velocity_y = 1.2f * player2->joystick_y;

		// apply friction
		player2->velocity_x = lerp(player2->velocity_x, 0.0f, FIXED_TIMESTEP * player2->friction_x);
		player2->velocity_y = lerp(player2->velocity_y, 0.0f, FIXED_TIMESTEP * player2->friction_y);

		// apply velocity
		player2->velocity_x += player2->acceleration_x * FIXED_TIMESTEP;
		player2->velocity_y += player2->acceleration_y * FIXED_TIMESTEP;

		// update x values
		player2->x += player2->velocity_x * FIXED_TIMESTEP;

		// update y values
		player2->y += player2->velocity_y * FIXED_TIMESTEP;

		// apply friction
		puck->velocity_x = lerp(puck->velocity_x, 0.0f, FIXED_TIMESTEP * puck->friction_x);
		puck->velocity_y = lerp(puck->velocity_y, 0.0f, FIXED_TIMESTEP * puck->friction_y);

		// apply velocity
		puck->velocity_x += puck->acceleration_x * FIXED_TIMESTEP;
		puck->velocity_y += puck->acceleration_y * FIXED_TIMESTEP;

		// update x values
		puck->x += puck->velocity_x * FIXED_TIMESTEP;

		// update y values
		puck->y += puck->velocity_y * FIXED_TIMESTEP;
	}
}

void GlowHockey::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	switch (state) {
	case STATE_MAIN_MENU:
		RenderMainMenu();
		break;
	case STATE_GAME_LEVEL:
		RenderGameLevel();
		break;
	case STATE_GAME_OVER:
		RenderGameOver();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void GlowHockey::RenderMainMenu() {
	glMatrixMode(GL_MODELVIEW);
	switch (menuControl){
	case 1:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f/255.0f, 100.0f/255.0f, 255.0f/255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Single Player", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Multiplayer", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "How to Play", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.4f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Quit Game", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = 0.2f;
		controlIndicator->Render();
		break;
	case 2:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Single Player", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Multiplayer", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "How to Play", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.4f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Quit Game", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = 0.0f;
		controlIndicator->Render();
		break;
	case 3:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Single Player", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Multiplayer", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "How to Play", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.4f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Quit Game", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = -0.2f;
		controlIndicator->Render();
		break;
	case 4:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Single Player", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Multiplayer", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "How to Play", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.4f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Quit Game", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = -0.4f;
		controlIndicator->Render();
		break;
	case 11:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Easy", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Normal", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Impossible", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = 0.2f;
		controlIndicator->Render();
		break;
	case 12:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Easy", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Normal", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Impossible", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = 0.0f;
		controlIndicator->Render();
		break;
	case 13:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Easy", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Normal", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Impossible", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = -0.2f;
		controlIndicator->Render();
		break;
	case 21:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Local", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Networked", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = 0.2f;
		controlIndicator->Render();
		break;
	case 22:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Local", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Networked", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		controlIndicator->x = -0.95f;
		controlIndicator->y = 0.0f;
		controlIndicator->Render();
		break;
	case 31:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		glScalef(1.6f, 1.6f, 0.0f);
		DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Controls", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Player 1: WASD", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.2f, 0.0f);
		glScalef(0.8f, 0.8f, 0.0f);
		DrawText(fontSheetTexture, "Player 2: Arrow keys", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.4f, 0.0f);
		glScalef(0.4f, 0.4f, 0.0f);
		DrawText(fontSheetTexture, "Knock the puck into the goal to score.", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.8f, -0.5f, 0.0f);
		glScalef(0.4f, 0.4f, 0.0f);
		DrawText(fontSheetTexture, "First one to seven points wins.", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		break;
	default:
		glLoadIdentity();
		glTranslatef(-0.8f, 0.7f, 0.0f);
		DrawText(fontSheetTexture, "How the heck did you get here???", 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);
		break;
	}
}

void GlowHockey::RenderGameOver() {
	glLoadIdentity();
	glTranslatef(-0.8f, 0.7f, 0.0f);
	glScalef(1.6f, 1.6f, 0.0f);
	DrawText(fontSheetTexture, "Glow Hockey", 0.1f, 0.0f, 255.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f, 1.0f);
	glLoadIdentity();
	glTranslatef(-0.8f, 0.2f, 0.0f);
	glScalef(0.6f, 0.6f, 0.0f);
	DrawText(fontSheetTexture, "Game Over.", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	if (player1Score > player2Score){
		glTranslatef(0.0f, -0.2f, 0.0f);
		DrawText(fontSheetTexture, "Top player wins!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (player2Score > player1Score){
		glTranslatef(0.0f, -0.2f, 0.0f);
		DrawText(fontSheetTexture, "Bottom player wins!", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (player1Score == player2Score){
		glTranslatef(0.0f, -0.2f, 0.0f);
		DrawText(fontSheetTexture, "How the heck did", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, -0.2f, 0.0f);
		DrawText(fontSheetTexture, "you get here???", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	glLoadIdentity();
	glTranslatef(-0.95f, -0.8f, 0.0f);
	glScalef(0.6f, 0.6f, 0.0f);
	DrawText(fontSheetTexture, "Press ESC to go back to main menu.", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	glLoadIdentity();
}

void GlowHockey::RenderGameLevel() {
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		for (size_t i = 0; i < entities.size(); i++) {
			entities[i]->Render();
		}
		player1->Render();
		player2->Render();
		puck->Render();
		p.Render();

		glLoadIdentity();
		glTranslatef(0.65f, 0.25f, 0.0f);
		glRotatef(-90.0f, 0, 0, 1);
		glScalef(0.6f, 0.6f, 0.0f);
		string player1SString = to_string(player1Score);
		DrawText(fontSheetTexture, player1SString, 0.1f, 0.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.8f);

		glLoadIdentity();
		glTranslatef(0.65f, 0.12f, 0.0f);
		glRotatef(-90.0f, 0, 0, 1);
		glScalef(0.6f, 0.6f, 0.0f);
		DrawText(fontSheetTexture, "Score", 0.1f, 0.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.8f);

		glLoadIdentity();
		glTranslatef(0.65f, -0.25f, 0.0f);
		glRotatef(-90.0f, 0, 0, 1);
		glScalef(0.6f, 0.6f, 0.0f);
		string player2SString = to_string(player2Score);
		DrawText(fontSheetTexture, player2SString, 0.1f, 0.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.8f);

		// STAT TRACKING
		if (stats){
			if (aiOn){
				glLoadIdentity();
				glTranslatef(-1.3f, 0.8f, 0.0f);
				glScalef(0.3f, 0.3f, 0.0f);
				string aState = "AI State: " + aiState;
				DrawText(fontSheetTexture, aState, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);
			}
			else {
				glLoadIdentity();
				glTranslatef(-1.3f, 0.8f, 0.0f);
				glScalef(0.3f, 0.3f, 0.0f);
				string aState = "AI State: Off";
				DrawText(fontSheetTexture, aState, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);
			}
			glLoadIdentity();
			glTranslatef(-1.3f, 0.7f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string pX = "Puck X: " + to_string(puck->x);
			DrawText(fontSheetTexture, pX, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.6f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string pY = "Puck Y: " + to_string(puck->y);
			DrawText(fontSheetTexture, pY, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.5f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string vX = "Puck VX: " + to_string(puck->velocity_x);
			DrawText(fontSheetTexture, vX, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.4f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string vY = "Puck VY: " + to_string(puck->velocity_y);
			DrawText(fontSheetTexture, vY, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.3f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p1X = "Player 1 X: " + to_string(player1->x);
			DrawText(fontSheetTexture, p1X, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.2f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p1Y = "Player 1 Y: " + to_string(player1->y);
			DrawText(fontSheetTexture, p1Y, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.1f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p1VX = "Player 1 VX: " + to_string(player1->velocity_x);
			DrawText(fontSheetTexture, p1VX, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, 0.0f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p1VY = "Player 1 VY: " + to_string(player1->velocity_y);
			DrawText(fontSheetTexture, p1VY, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.1f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p2X = "Player 2 X: " + to_string(player2->x);
			DrawText(fontSheetTexture, p2X, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.2f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p2Y = "Player 2 Y: " + to_string(player2->y);
			DrawText(fontSheetTexture, p2Y, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.3f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p2VX = "Player 2 VX: " + to_string(player2->velocity_x);
			DrawText(fontSheetTexture, p2VX, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.4f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string p2VY = "Player 2 VY: " + to_string(player2->velocity_y);
			DrawText(fontSheetTexture, p2VY, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.5f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string pCol;
			if (puck->collided){
				pCol = "Puck collided: true";
			}
			else{
				pCol = "Puck collided: false";
			}
			DrawText(fontSheetTexture, pCol, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.6f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string particleC = "Particle Count: " + to_string(particleCount);
			DrawText(fontSheetTexture, particleC, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.7f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			string aid = "AI Difficulty: " + to_string(AIDifficulty);
			DrawText(fontSheetTexture, aid, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

			glLoadIdentity();
			glTranslatef(-1.3f, -0.8f, 0.0f);
			glScalef(0.3f, 0.3f, 0.0f);
			int num_joy = SDL_NumJoysticks();
			string joySticks = "Joysticks: " + to_string(num_joy);
			DrawText(fontSheetTexture, joySticks, 0.1f, 0.0f, 10.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f, 1.0f);

	}
}



bool GlowHockey::UpdateAndRender() {
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

bool GlowHockey::collision(Entity* e1, Entity* e2) {
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
	if (!(mXA <= e1->sprite.width * e1->scale_x && mxXA >= -e1->sprite.width * e1->scale_x)){
		e1->collided = false;
		e2->collided = false;
		return false;
	}

	float mYA = min(min(min(a1.y, a3.y), a2.y), a4.y);
	float mxYA = max(max(max(a1.y, a3.y), a2.y), a4.y);

	// check y a
	if (!(mYA <= e1->sprite.height * e1->scale_y && mxYA >= -e1->sprite.height * e1->scale_y)){
		e1->collided = false;
		e2->collided = false;
		return false;
	}

	float mXB = min(min(min(b1.x, b3.x), b2.x), b4.x);
	float mxXB = max(max(max(b1.x, b3.x), b2.x), b4.x);

	// check x a
	if (!(mXB <= e2->sprite.width * e2->scale_x && mxXB >= -e2->sprite.width * e2->scale_x)){
		e1->collided = false;
		e2->collided = false;
		return false;
	}

	float mYB = min(min(min(b1.y, b3.y), b2.y), b4.y);
	float mxYB = max(max(max(b1.y, b3.y), b2.y), b4.y);

	if (!(mYB <= e2->sprite.height * e2->scale_y && mxYB >= -e2->sprite.height * e2->scale_y)){
		e1->collided = false;
		e2->collided = false;
		return false;
	}

	if (e1 == puck){
		if (e1->collided == false){
			if (e2->up){
				puck->collided = true;
				p = ParticleSystem(particleCount);
				if (e2->cColor == 3){
					p.red = 255.0f / 255.0f;
					p.blue = 50.0f / 255.0f;
					p.green = 50.0f / 255.0f;
				}
				else if (e2->cColor == 4){
					p.red = 255.0f / 255.0f;
					p.blue = 100.0f / 255.0f;
					p.green = 255.0f / 255.0f;
				}
				p.Reset(3);
				for (unsigned int i = 0; i < p.particles.size(); i++) {
					p.particles[i].position.x = puck->x;
					p.particles[i].position.y = puck->y;
				}
				if (puck->velocity_y > 0){
					puck->velocity_y = puck->velocity_y * -1.0f;
				}
				Mix_PlayChannel(-1, hitWall, 0);
			}
			else if (e2->down){
				puck->collided = true;
				p = ParticleSystem(particleCount);
				if (e2->cColor == 3){
					p.red = 255.0f / 255.0f;
					p.blue = 50.0f / 255.0f;
					p.green = 50.0f / 255.0f;
				}
				else if (e2->cColor == 4){
					p.red = 255.0f / 255.0f;
					p.blue = 100.0f / 255.0f;
					p.green = 255.0f / 255.0f;
				}
				p.Reset(4);
				for (unsigned int i = 0; i < p.particles.size(); i++) {
					p.particles[i].position.x = puck->x;
					p.particles[i].position.y = puck->y;
				}
				if (puck->velocity_y < 0){
					puck->velocity_y = puck->velocity_y * -1.0f;
				}
				Mix_PlayChannel(-1, hitWall, 0);
			}
			else if (e2->left){
				puck->collided = true;
				p = ParticleSystem(particleCount);
				if (e2->cColor == 3){
					p.red = 255.0f / 255.0f;
					p.blue = 50.0f / 255.0f;
					p.green = 50.0f / 255.0f;
				}
				else if (e2->cColor == 4){
					p.red = 255.0f / 255.0f;
					p.blue = 100.0f / 255.0f;
					p.green = 255.0f / 255.0f;
				}
				p.Reset(2);
				for (unsigned int i = 0; i < p.particles.size(); i++) {
					p.particles[i].position.x = puck->x;
					p.particles[i].position.y = puck->y;
				}
				if (puck->velocity_x < 0){
					puck->velocity_x = puck->velocity_x * -1.0f;
				}
				Mix_PlayChannel(-1, hitWall, 0);
			}
			else if (e2->right){
				puck->collided = true;
				p = ParticleSystem(particleCount);
				if (e2->cColor == 3){
					p.red = 255.0f / 255.0f;
					p.blue = 50.0f / 255.0f;
					p.green = 50.0f / 255.0f;
				}
				else if (e2->cColor == 4){
					p.red = 255.0f / 255.0f;
					p.blue = 100.0f / 255.0f;
					p.green = 255.0f / 255.0f;
				}
				p.Reset(1);
				for (unsigned int i = 0; i < p.particles.size(); i++) {
					p.particles[i].position.x = puck->x;
					p.particles[i].position.y = puck->y;
				}
				if (puck->velocity_x > 0){
					puck->velocity_x = puck->velocity_x * -1.0f;
				}
				Mix_PlayChannel(-1, hitWall, 0);
			}
		}
	}
	
	if (e1 == player2){
		if (player2->collided == false){
			puck->collided = true;
			player2->collided = true;
			p = ParticleSystem(particleCount);
			p.red = 255.0f/255.0f;
			p.blue = 255.0f/255.0f;
			p.green = 100.0f/255.0f;
			p.Reset(0);
			for (unsigned int i = 0; i < p.particles.size(); i++) {
				p.particles[i].position.x = puck->x;
				p.particles[i].position.y = puck->y;
			}
			if (player2->velocity_x >= 0)
				puck->velocity_x = player2->velocity_x + 0.2f;
			if (player2->velocity_x <= 0)
				puck->velocity_x = player2->velocity_x - 0.2f;
			if (player2->velocity_y >= 0)
				puck->velocity_y = player2->velocity_y + 0.2f;
			if (player2->velocity_y <= 0)
				puck->velocity_y = player2->velocity_y - 0.2f;

			Mix_PlayChannel(-1, hitPuck, 0);
		}
	}

	if (e1 == player1){
		if (player1->collided == false){
			puck->collided = true;
			player1->collided = true;
			p = ParticleSystem(particleCount);
			p.red = 100.0f/255.0f;
			p.blue = 100.0f/255.0f;
			p.green = 255.0f/255.0f;
			p.Reset(0);
			for (unsigned int i = 0; i < p.particles.size(); i++) {
				p.particles[i].position.x = puck->x;
				p.particles[i].position.y = puck->y;
			}
			if (player1->velocity_x >= 0)
				puck->velocity_x = player1->velocity_x + 0.4f;
			if (player1->velocity_x <= 0)
				puck->velocity_x = player1->velocity_x - 0.4f;
			if (player1->velocity_y >= 0)
				puck->velocity_y = player1->velocity_y + 0.4f;
			if (player1->velocity_y <= 0)
				puck->velocity_y = player1->velocity_y - 0.4f;

			Mix_PlayChannel(-1, hitPuck, 0);
		}
	}

	// end
	return true;
}
