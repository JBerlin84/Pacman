#ifndef ENEMY_H_
#define ENEMY_H_

#include "Character.h"

class Enemy : public Character {
private:
	int enemyType;
	bool dead;

public:
	Enemy(int x, int y, int enemyType) : Character(x, y, INT_MAX){
		this->enemyType = enemyType;
		dead = false;
	}

	void setDead(bool value) {
		dead = value;
	}

	bool getDead() {
		return dead;
	}

	void update() {
		// If lerpAmmount is 0 the character is directly over a tile and new calculations can be done.
		if(lerpAmmount == 0) {
			// Lua
			lua_getglobal(luaState, "getNextMoveById");
			lua_pushnumber(luaState, enemyType);	// which enemy are we?
			lua_pushnumber(luaState, x);			// which x-coord do we have?
			lua_pushnumber(luaState, y);			// which y-coord do we have?
			lua_pushnumber(luaState, dead);		// is the enemy dead?
			lua_pcall(luaState, 4, 1, 0);			// Tar tre argument, returnerar två, ingen felhantering
			Direction newDirection = (Direction)(int)lua_tonumber(luaState, -1);		// for some reason lua_tointeger won't work, even if i send integer.
			lua_pop(luaState, 1);

			movingDirection = newDirection;
		}

		lerpAmmount = lerpSpeed + lerpAmmount;
		
		int xDir = 0;
		int yDir = 0;
		
		if (movingDirection == Direction_up) {// && maze[y-1][x] != Tile_wall) {
			--yDir;
		}
		else if (movingDirection == Direction_down) {// && maze[y+1][x] != Tile_wall) {
			++yDir;
		}
		else if (movingDirection == Direction_left) {// && maze[y][x-1] != Tile_wall) {
			--xDir;
		}
		else if (movingDirection == Direction_right) { // && maze[y][x+1] != Tile_wall) {
			++xDir;
		}
		// Special case for warp.
		else if (movingDirection == Direction_left && x == 0) {
			--xDir;
		}
		else if (movingDirection == Direction_right && x == MAZE_WIDTH - 1) {
			++xDir;
		}

		// Lerp between tiles.
		ix = lerp(1, 1 + xDir, lerpAmmount) - 1;
		iy = lerp(1, 1 + yDir, lerpAmmount) - 1;

		// lerpammount should be lower than tile size
		if (lerpAmmount < TILE_WIDTH) {
			return;
		}

		x = x + xDir;
		y = y + yDir;

		ix = 0;
		iy = 0;
		lerpAmmount = 0;

		if (dead && x == 11 && y == 15) {
			dead = false;
		}
	}

	int getCurrentSprite(int version) {
		int currentSprite = -1;
		if (movingDirection == Direction_up) {
			currentSprite = ENEMY_UP;
		}
		else if (movingDirection == Direction_down) {
			currentSprite =	ENEMY_DOWN;
		}
		else if (movingDirection == Direction_left) {
			currentSprite = ENEMY_LEFT;
		}
		else { // Default state is right
			currentSprite = ENEMY_RIGHT;
		}

		currentSprite += enemyType;
		
		if(isHuntingTime()) {
			currentSprite = ENEMY_SCARED;
		}
		if (dead) {
			currentSprite = ENEMY_DEAD;
		}

		currentSprite += version-1;
		return currentSprite;
	}
};

#endif