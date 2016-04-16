#ifndef ENEMY_H_
#define ENEMY_H_

#include "Character.h"

class Enemy : public Character {
private:
	int enemyType;

public:
	Enemy(int x, int y, int enemyType) : Character(x, y, INT_MAX){
		this->enemyType = enemyType;
	}

	void update() {
		// If lerpAmmount is 0 the character is directly over a tile and new calculations can be done.
		if(lerpAmmount == 0) {
			// Lua
			lua_getglobal(luaState, "ai");
			lua_pushnumber(luaState, enemyType);	// which enemy are we?
			lua_pushnumber(luaState, x);			// which x-coord do we have?
			lua_pushnumber(luaState, y);			// which y-coord do we have?
			lua_pcall(luaState, 3, 1, 0);			// Tar tre argument, returnerar tv�, ingen felhantering
			Direction newDirection = (Direction)(int)lua_tonumber(luaState, -1);		// for some reason lua_tointeger wont work, even if i send integer.
			lua_pop(luaState, 1);

			/*
			std::stringstream ss;			
			if (newDirection == Direction_up)
				ss << "up";
			else if(newDirection == Direction_down)
				ss << "down";
			else if(newDirection == Direction_left)
				ss << "left";
			else if (newDirection == Direction_right)
				ss << "right";
			else
				ss << "Something went terribly wrong: newDirection = " << newDirection << " inewDirection: " << newDirection;
				*/

			//ss << "\nnew y: " << newy;
			//engine->message(ss.str());

			/*
			if(newy==-1.0)
				movingDirection = Direction_up;
			else if(newy==1.0)
				movingDirection = Direction_down;
			else if(newx==-1.0)
				movingDirection = Direction_left;
			else if(newx==1.0)
				movingDirection = Direction_right;
				*/
			movingDirection = newDirection;
		}

		lerpAmmount = lerpSpeed + lerpAmmount;
		
		int xDir = 0;
		int yDir = 0;
		
		if (movingDirection == Direction_up && maze[y-1][x] != Tile_wall) {
			--yDir;
		}
		else if (movingDirection == Direction_down && maze[y+1][x] != Tile_wall) {
			++yDir;
		}
		else if (movingDirection == Direction_left && maze[y][x-1] != Tile_wall) {
			--xDir;
		}
		else if (movingDirection == Direction_right && maze[y][x+1] != Tile_wall) {
			++xDir;
		}
		else if (movingDirection == Direction_left && x == 0) {
			--xDir;
		}
		else if (movingDirection == Direction_right && x == MAZE_WIDTH - 1) {
			++xDir;
		}

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

		currentSprite += version-1;
		return currentSprite;
	}
};

#endif