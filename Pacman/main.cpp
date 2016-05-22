/************************************************************************
Author: Jimmy Berlin
Last modified: 2016-02-01
Project: Assignment 2, Maze

desc:
main functions used with library GameEngine.
************************************************************************/

#include "main.h"

using namespace GameEngine;

GameObject *mazeSprite;
GameObject *characterSprite;
Font *font;
Timer *animationTimer;
int currentSpriteVersion;

bool game_preload() {
    engine->setAppTitle(APPLICATION_TITLE);
    engine->setFullscreen(FULLSCREEN);
    engine->setScreenWidth(MAZE_WIDTH * TILE_WIDTH);
	engine->setScreenHeight(MAZE_HEIGHT * TILE_HEIGHT + TILE_HEIGHT);
	engine->setColorDepth(COLOR_DEPTH);

	// Lua test
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	int luaError = luaL_dofile(luaState, "ai.lua");
	if (luaError) {
		std::stringstream ss;
		ss << "Error loading LUA file: " << lua_tostring(luaState, -1);
		engine->fatalerror(ss.str());
		return false;
	}

    return true;
}

bool initializeLuaEnvironment() {
	// Push world values
	lua_pushnumber(luaState, MAZE_HEIGHT);
	lua_setglobal(luaState, "MAZE_HEIGHT");
	lua_pushnumber(luaState, MAZE_WIDTH);
	lua_setglobal(luaState, "MAZE_WIDTH");
	
	// Push directions
	lua_pushnumber(luaState, Direction_none);
	lua_setglobal(luaState, "DIRECTION_NONE");
	lua_pushnumber(luaState, Direction_up);
	lua_setglobal(luaState, "DIRECTION_UP");
	lua_pushnumber(luaState, Direction_down);
	lua_setglobal(luaState, "DIRECTION_DOWN");
	lua_pushnumber(luaState, Direction_left);
	lua_setglobal(luaState, "DIRECTION_LEFT");
	lua_pushnumber(luaState, Direction_right);
	lua_setglobal(luaState, "DIRECTION_RIGHT");

	// Push random global variables
	lua_pushnumber(luaState, 0);
	lua_setglobal(luaState, "consumed_dots");

	// Push enemy types
	lua_pushnumber(luaState, ENEMY_BLINKY);
	lua_setglobal(luaState, "ENEMY_TYPE_BLINKY");
	lua_pushnumber(luaState, ENEMY_INKY);
	lua_setglobal(luaState, "ENEMY_TYPE_INKY");
	lua_pushnumber(luaState, ENEMY_PINKY);
	lua_setglobal(luaState, "ENEMY_TYPE_PINKY");
	lua_pushnumber(luaState, ENEMY_CLYDE);
	lua_setglobal(luaState, "ENEMY_TYPE_CLYDE");
	
	// Transfer world matrix to lua as single array
	// TODO: create a transfer of matrix here?
	lua_newtable(luaState);
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			//int index = MAZE_HEIGHT*j + i;
			int index = j + MAZE_WIDTH*i;
			lua_pushnumber(luaState, index);
			int value = maze[i][j] == 1 ? 1 : 0;
			lua_pushnumber(luaState, value);
			lua_settable(luaState, -3);
		}
	}
	// push maze to variable MAZE
	lua_setglobal(luaState, "MAZE");
	
	// Runs initial function of lua
	lua_getglobal(luaState, "init");
	lua_pcall(luaState, 0, 0, 0);

	return true;
}

bool game_init(HWND) {
	
	mazeSprite = new GameObject();
	if (!mazeSprite->loadImage("Resources\\Maze.png", D3DCOLOR_XRGB(0, 0, 0))) {
		engine->message("Error loading maze sprite");
		return false;
	}
	mazeSprite->setColumns(10);
	mazeSprite->setTotalFrames(30);
	mazeSprite->setSize(TILE_WIDTH, TILE_HEIGHT);
	mazeSprite->setScale(1);

	characterSprite = new GameObject();
	if (!characterSprite->loadImage("Resources\\Characters.png", D3DCOLOR_XRGB(0, 0, 0))) {
		engine->message("Error loading character sprite");
		return false;
	}
	characterSprite->setColumns(14);
	characterSprite->setTotalFrames(56);
	characterSprite->setSize(TILE_WIDTH, TILE_HEIGHT);

	font = new Font();
	if (!font->loadImage("resources\\Arcade.tga")) {
		engine->message("Error loading font");
	}
	font->setColumns(16);
	font->setTotalFrames(256);
	font->setCharSize(64, 63);
	font->setScale(0.5);
	if (!font->loadWidthData("resources\\Arcade.dat")) {
		engine->message("Error loading font data");
	}

	// Count the number of fruits.
	for (int x = 0; x < MAZE_WIDTH; x++) {
		for (int y = 0; y < MAZE_HEIGHT; y++) {
			if (maze[y][x] == 2 || maze[y][x] == 3) {
				++numberOfFruitsLeft;
			}
		}
	}
	maxNumberOfFruits = numberOfFruitsLeft;

	player = Character(START_POSITION_X, START_POSITION_Y, 3);

	enemies[0] = new Enemy(14,14, ENEMY_BLINKY);
	enemies[1] = new Enemy(13,14, ENEMY_PINKY);
	enemies[2] = new Enemy(14,15, ENEMY_INKY);
	enemies[3] = new Enemy(13,15, ENEMY_CLYDE);

	animationTimer = new Timer();
	animationTimer->reset();
	
	initializeLuaEnvironment();

    return true;
}

// warp the player from left corridor to the right corridor.
void warpPlayer() {
	if (player.x < 0) {
		player.x = MAZE_WIDTH-1;
	}

	if (player.x == MAZE_WIDTH) {
		player.x = 0;
	}
}

void startHuntingTime() {
	player.startHuntingTime();
	for(int i=0;i<4;i++) {
		enemies[i]->startHuntingTime();
	}
}

void checkMapEntries() {
	int currentMazeTile = maze[player.y][player.x];

	if (currentMazeTile == 2) {
		player.points += 10;
		--numberOfFruitsLeft;
	}
	else if (currentMazeTile == 3) {
		player.points += 30;
		--numberOfFruitsLeft;
		startHuntingTime();
	}

	maze[player.y][player.x] = 0;
}

void checkVictoryCondition() {
	// If all the fruits is taken, we have won.
	if (numberOfFruitsLeft <= 0) {
		engine->Shutdown();
	}
}

// TODO: Create a real game over state
void checkGameOver() {
	// if the player is on the same tile as the enemy, he dies
	if(player.lives <= 0)
		engine->Shutdown();
}

void checkEnemyPlayerCollision() {
	if(!player.isHuntingTime()) {
		for(int i=0;i<4;i++) {
			if(player.x == enemies[i]->x && player.y == enemies[i]->y) {
				--player.lives;
				player.x = START_POSITION_X;
				player.y = START_POSITION_Y;
			}
		}
	}
}

void updateLuaState() {
	// update variables in lua state
	lua_pushnumber(luaState, maxNumberOfFruits - numberOfFruitsLeft);
	lua_setglobal(luaState, "consumed_dots");
	lua_pushboolean(luaState, player.isHuntingTime());
	lua_setglobal(luaState, "huntingTime");

	// update state
	lua_getglobal(luaState, "updateLuaState");
	if (lua_pcall(luaState, 0, 0, 0)) {
		std::stringstream ss;
		ss << "Error updating lua-state: " << lua_tostring(luaState, -1);
		engine->message(ss.str());
	}

	// Update the player state in lua.
	lua_getglobal(luaState, "updatePlayerState");
	lua_pushnumber(luaState, player.x);
	lua_pushnumber(luaState, player.y);
	lua_pushnumber(luaState, player.getMovingDirection());
	lua_pcall(luaState, 3, 1, 0);
	bool succeeded = lua_toboolean(luaState, -1);
	lua_pop(luaState, -1);
	
	if (!succeeded) {
		engine->message("update lua state failed.");
	}
}

void game_update() {
	// Update the lua state
	updateLuaState();
	// Fetch enemy ai
	for(int i=0;i<4;i++) {
		enemies[i]->update();
	}

	checkEnemyPlayerCollision();
	player.update();
	warpPlayer();
	checkMapEntries();
	checkVictoryCondition();
	checkGameOver();

	// Switch between 1 and 2 as there is only two different versions to each character sprite
	if (animationTimer->stopwatch(ANIMATION_SPEED)) {
		currentSpriteVersion = (currentSpriteVersion == 2 ? 1 : 2);
		animationTimer->reset();
	}
}

void game_keyPress(int key) {
	if (key == DIK_UPARROW) {
		player.moveUp();
	}
	if (key == DIK_DOWNARROW) {
		player.moveDown();
	}
	if (key == DIK_LEFTARROW) {
		player.moveLeft();
	}
	if (key == DIK_RIGHTARROW) {
		player.moveRight();
	}
	if (key == DIK_ESCAPE) {
		gameover = true;
	}
}

void game_keyRelease(int key) {
}

void drawPlayer() {
	characterSprite->setCurrentFrame(player.getCurrentSprite(currentSpriteVersion));
	characterSprite->setPosition(player.x * TILE_WIDTH + player.ix, player.y * TILE_HEIGHT + player.iy);
	characterSprite->draw();

	// if player is warping, draw player on both sides.
	if (player.x == 0 && player.getMovingDirection() == Direction_left) {	// player going left through warp
		characterSprite->setPosition(MAZE_WIDTH * TILE_WIDTH + player.ix, player.y * TILE_HEIGHT + player.iy);
		characterSprite->draw();
	}
	else if (player.x == MAZE_WIDTH - 1 && player.getMovingDirection() == Direction_right) {		// player going right through warp
		characterSprite->setPosition(-1 * TILE_WIDTH + player.ix, player.y * TILE_HEIGHT + player.iy);
		characterSprite->draw();
	}
}

void drawEnemies() {
	for(int i=0;i<4;i++) {
		characterSprite->setCurrentFrame(enemies[i]->getCurrentSprite(currentSpriteVersion));
		characterSprite->setPosition(enemies[i]->x * TILE_WIDTH + enemies[i]->ix, enemies[i]->y * TILE_HEIGHT + enemies[i]->iy);
		characterSprite->draw();
	}
}

// The mother of all if statements!!!!
// Fetch the correct sprite from the spritesheet
int fetchMazeTileType(int x, int y) {
	bool up, down, left, right;					// straight
	up = down = left = right = false;
	bool upRight, upLeft, downRight, downLeft;  // diagonal
	upRight = upLeft = downRight = downLeft = false;
	bool oobLeft, oobRight, oobUp, oobDown;		// out of bounds.
	oobLeft = oobRight = oobUp = oobDown = false;

	// Check if -1 or +1 is out of range.
	oobUp = y - 1 < 0;
	oobDown = y + 1 >= MAZE_HEIGHT;
	oobLeft = x - 1 < 0;
	oobRight = x + 1 >= MAZE_WIDTH;

	// Check which sides are a wall
	if (!up && !oobUp)
		up = maze[y - 1][x] == 1;
	if (!down && !oobDown)
		down = maze[y + 1][x] == 1;
	if(!left && !oobLeft)
		left = maze[y][x - 1] == 1;
	if (!right && !oobRight)
		right = maze[y][x + 1] == 1;
	if (!upLeft && !oobUp && !oobLeft)
		upLeft = maze[y - 1][x - 1] == 1;
	if (!upRight && !oobUp && !oobRight)
		upRight = maze[y - 1][x + 1] == 1;
	if (!downLeft && !oobDown && !oobLeft)
		downLeft = maze[y + 1][x - 1] == 1;
	if (!downRight && !oobDown && !oobRight)
		downRight = maze[y + 1][x + 1] == 1;

	//Part inner corners
	////////////////////////////////////////////////////////////////////// two more
	if (left && right && !up && down && !upRight && !downLeft)
		return TILE_TOP_SIDE_LEFT_INNER;
	else if (left && right && !up && down && !upLeft && !downRight)
		return TILE_TOP_SIDE_RIGHT_INNER;
	else if (up && down && !left && right && !upLeft && !upRight)
		return TILE_LEFT_SIDE_UPPER_INNER;
	else if (up && down && !left && right && !downLeft && !downRight)
		return TILE_LEFT_SIDE_LOWER_INNER;
	else if (up && down && left && !right && !upLeft && !upRight)
		return TILE_RIGHT_SIDE_UPPER_INNER;
	else if (up && down && left && !right && !downLeft && !downRight)
		return TILE_RIGHT_SIDE_LOWER_INNER;

	// special case for "warp zone"
	else if ((x == 0 || x == 27) && (y == 13 || y == 15))
		return TILE_HORIZONTAL_SINGLE;

	// Single lane pieces
	else if (up && down && !left && !right)
		return TILE_VERTICAL_SINGLE;
	else if (!up && !down && left && right)
		return TILE_HORIZONTAL_SINGLE;
	// Single corner pieces
	else if (up && !down && left && !right && !upLeft)
		return TILE_LOWER_RIGHT_SINGLE;
	else if (up && !down && !left && right && !upRight)
		return TILE_LOWER_LEFT_SINGLE;
	else if (!up && down && !left && right && !downRight)
		return TILE_UPPER_LEFT_SINGLE;
	else if (!up && down && left && !right && !downLeft)
		return TILE_UPPER_RIGHT_SINGLE;
	// Single line endings
	else if (up && !down && !left && !right)
		return TILE_LOWER_SINGLE;
	else if (!up && down && !left && !right)
		return TILE_UPPER_SINGLE;
	else if (!up && !down && left && !right)
		return TILE_RIGHT_SINGLE;
	else if (!up && !down && !left && right)
		return TILE_LEFT_SINGLE;

	// Straight pieces filled on both the other side
	else if (up && down && left && !right)
		return TILE_MIDDLE_RIGHT_FILLED;
	else if (up && down && !left && right)
		return TILE_MIDDLE_LEFT_FILLED;
	else if (up && !down && left && right)
		return TILE_LOWER_MIDDLE_FILLED;
	else if (!up && down && left && right)
		return TILE_UPPER_MIDDLE_FILLED;

	// Corner pieces filled on other sides
	else if (up && !down && left && !right)
		return TILE_LOWER_RIGHT_FILLED;
	else if (up && !down && !left && right)
		return TILE_LOWER_LEFT_FILLED;
	else if (!up && down && left && !right)
		return TILE_UPPER_RIGHT_FILLED;
	else if (!up && down && !left && right)
		return TILE_UPPER_LEFT_FILLED;

	// Inner corners
	else if (left && down && upLeft && upRight && downLeft && !downRight)
		return TILE_UPPER_LEFT_INNER;
	else if (right && down && upLeft && upRight && !downLeft && downRight)
		return TILE_UPPER_RIGHT_INNER;
	else if (left && up && downLeft && downRight && upLeft && !upRight)
		return TILE_LOWER_LEFT_INNER;
	else if (right && up && downLeft && downRight && !upLeft && upRight)
		return TILE_LOWER_RIGHT_INNER;

	else
		return TILE_MIDDLE_MIDDLE_FILLED;
}

// Get which tile to be drawn.
int getMazeTile(int x, int y) {
	int tileType = maze[y][x];

	switch (tileType) {
	case 0:
		return TILE_EMPTY;
	case 1:
		return fetchMazeTileType(x, y);
	case 2:
		return TILE_SMALL_FOOD;
	case 3:
		return TILE_LARGE_FOOD;
	case 4:
		return TILE_EMPTY;
	default:
		engine->message("Something went wrong, trying to fetch invalid tyletype");
		return TILE_EMPTY;
	}
}

// Draw the maze on the screen
void drawMaze() {
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			mazeSprite->setPosition(x * mazeSprite->getWidth(), y * mazeSprite->getHeight());
			mazeSprite->setCurrentFrame(getMazeTile(x,y));
			// If there is a valid tile, draw it. -1 == empty tile.
			if (mazeSprite->getCurrentFrame() != -1)
				mazeSprite->draw();
		}
	}
}

void drawGUI() {
	// Draw string
	std::stringstream ss;
	ss << "LIVES:                     SCORE: " << player.points;
	font->Print(10, MAZE_HEIGHT * mazeSprite->getHeight(), ss.str());
	
	// Draw lives
	characterSprite->setCurrentFrame(CHARACTER_PLAYER_RIGHT_2);
	for (int i = 0; i < player.lives; i++) {
		characterSprite->setPosition(130 + i*(characterSprite->getWidth() + 5), MAZE_HEIGHT * characterSprite->getHeight());
		characterSprite->draw();
	}
}

void drawDebugInterface() {
	std::stringstream ss;
	ss << "Maze: " << player.x << " : " << player.y << "   Screen: " << player.x * TILE_WIDTH + player.ix << " : " << player.y * TILE_WIDTH + player.iy << "   ";
	ss << "Maze[x,y]: " << maze[player.y][player.x];

	font->Print(10, 10, ss.str());

	ss = std::stringstream("");
	ss << "is hunting: " << player.isHuntingTime();
	font->Print(10, 50, ss.str());

	for(int i=0;i<4;i++) {
		ss = std::stringstream("");
		ss << "Enemy" << i << ": " << enemies[i]->x << ": " << enemies[i]->y << " hunted: " << enemies[i]->isHuntingTime();
		font->Print(10, 100+50*i, ss.str());
	}
}

void game_render() {
	engine->ClearScene(D3DCOLOR_XRGB(0, 0, 0));

	drawMaze();
	drawEnemies();
	drawPlayer();
	drawGUI();

	if (DEBUG_BUILD)
		drawDebugInterface();
}

void game_end() {
	delete font;
	delete animationTimer;
	delete mazeSprite;
	delete characterSprite;

	// Shut down lua environment
	lua_close(luaState);
}



