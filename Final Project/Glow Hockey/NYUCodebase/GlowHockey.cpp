//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		GlowHockey
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GlowHockey.h"

float GlowHockey::lerp(float v0, float v1, float t) {
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
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("GlowHockey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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
	testSheet = LoadTexture("sprites.png");

	// setting state
	state = STATE_MAIN_MENU;

	Init();
}

GlowHockey::~GlowHockey() {
	SDL_Quit();
}

void GlowHockey::Init() {
	// insert player as the first entity
	SpriteSheet player1Sprite = SpriteSheet(testSheet, 178.0f / 498.0f, 348.0f / 498.0f, 100.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet glow1Sprite = SpriteSheet(testSheet, 353.0f / 498.0f, 0.0f / 498.0f, 142.0f / 498.0f, 142.0f / 498.0f);
	player1 = new Entity();
	player1->sprite = player1Sprite;
	player1->glow = glow1Sprite;
	player1->glowing = true;
	player1->scale_x = 0.6f;
	player1->scale_y = 0.6f;
	player1->x = 0.0f;
	player1->y = 0.5f;
	player1->friction_x = 3.0f;
	player1->friction_y = 3.0f;
	player1->isPlayer1 = true;

	// insert player 2 as second entity
	SpriteSheet player2Sprite = SpriteSheet(testSheet, 281.0f / 498.0f, 348.0f / 498.0f, 100.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet glow2Sprite = SpriteSheet(testSheet, 178.0f / 498.0f, 203.0f / 498.0f, 142.0f / 498.0f, 142.0f / 498.0f);
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

	SpriteSheet puckSprite = SpriteSheet(testSheet, 384.0f / 498.0f, 290.0f / 498.0f, 100.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet puckGlowSprite = SpriteSheet(testSheet, 353.0f / 498.0f, 145.0f / 498.0f, 142.0f / 498.0f, 142.0f / 498.0f);
	puck = new Entity();
	puck->sprite = puckSprite;
	puck->glow = puckGlowSprite;
	puck->glowing = true;
	puck->scale_x = 0.5f;
	puck->scale_y = 0.5f;
	puck->x = 0.0f;
	puck->y = 0.0f;
	puck->velocity_x = -1.0f;
	puck->velocity_y = -0.8f;
	puck->friction_x = 1.0f;
	puck->friction_y = 1.0f;
	puck->isPuck = true;

	// set up board
	SpriteSheet redSprite = SpriteSheet(testSheet, 143.0f / 498.0f, 451.0f / 498.0f, 100.0f / 498.0f, 20.0f / 498.0f);
	SpriteSheet redGlowSprite = SpriteSheet(testSheet, 0.0f / 498.0f, 409.0f / 498.0f, 140.0f / 498.0f, 40.0f / 498.0f);
	SpriteSheet yellowSprite = SpriteSheet(testSheet, 143.0f / 498.0f, 474.0f / 498.0f, 100.0f / 498.0f, 20.0f / 498.0f);
	SpriteSheet yellowGlowSprite = SpriteSheet(testSheet, 0.0f / 498.0f, 452.0f / 498.0f, 140.0f / 498.0f, 40.0f / 498.0f);

	// center
	SpriteSheet centerSprite = SpriteSheet(testSheet, 0.0f / 498.0f, 0.0f / 498.0f, 350.0f / 498.0f, 200.0f / 498.0f);

	// player circles
	SpriteSheet bottomCircleSprite = SpriteSheet(testSheet, 0.0f / 498.0f, 306.0f / 498.0f, 175.0f / 498.0f, 100.0f / 498.0f);
	SpriteSheet topCircleSprite = SpriteSheet(testSheet, 0.0f / 498.0f, 203.0f / 498.0f, 175.0f / 498.0f, 100.0f / 498.0f);

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
	bottomCircle->scale_x = 0.95f;
	bottomCircle->scale_y = 0.95f;
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
	entities.push_back(bottom2);
}

void GlowHockey::Update(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_D]) {
		player2->velocity_x = 1.0f;
	}

	if (keys[SDL_SCANCODE_W]) {
		player2->velocity_y = 1.0f;
	}
	else if (keys[SDL_SCANCODE_S]) {
		player2->velocity_y = -1.0f;
	}
	else if (keys[SDL_SCANCODE_A]){
		player2->velocity_x = -1.0f;
	}

	player1->Update(elapsed);
	player2->Update(elapsed);
	puck->Update(elapsed);
}


void GlowHockey::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {
		// collision
		if (entities[i]->isWall) {
			collision(puck, entities[i]);
		}
	}

	// apply gravity
	if (!player1->isStatic) {
		player1->velocity_x += gravity_x * FIXED_TIMESTEP;
		player1->velocity_y += gravity_y * FIXED_TIMESTEP;
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

	// apply gravity
	if (!player2->isStatic) {
		player2->velocity_x += gravity_x * FIXED_TIMESTEP;
		player2->velocity_y += gravity_y * FIXED_TIMESTEP;
	}

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

	// apply gravity
	if (!puck->isStatic) {
		puck->velocity_x += gravity_x * FIXED_TIMESTEP;
		puck->velocity_y += gravity_y * FIXED_TIMESTEP;
	}

	// apply friction
	//puck->velocity_x = lerp(puck->velocity_x, 0.0f, FIXED_TIMESTEP * puck->friction_x);
	//puck->velocity_y = lerp(puck->velocity_y, 0.0f, FIXED_TIMESTEP * puck->friction_y);

	// apply velocity
	puck->velocity_x += puck->acceleration_x * FIXED_TIMESTEP;
	puck->velocity_y += puck->acceleration_y * FIXED_TIMESTEP;

	// update x values
	puck->x += puck->velocity_x * FIXED_TIMESTEP;

	// update y values
	puck->y += puck->velocity_y * FIXED_TIMESTEP;

	if (puck->y > 0.85f){
		puck->x = 0.0f;
		puck->y = 0.0f;
		// p2 score
	}
	if (puck->y < -0.85f){
		puck->x = 0.0f;
		puck->y = 0.0f;
		// p1 score
	}
}

void GlowHockey::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}
	player1->Render();
	player2->Render();
	puck->Render();

	SDL_GL_SwapWindow(displayWindow);
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

	if (e1 == puck){
		if (e2->up)
			puck->velocity_y = puck->velocity_y * -1.0;
		else if (e2->down)
			puck->velocity_y = puck->velocity_y * -1.0;
		else if (e2->left)
			puck->velocity_x = puck->velocity_x * -1.0;
		else if (e2->right)
			puck->velocity_x = puck->velocity_x * -1.0;
	}
	// end
	return true;
}
