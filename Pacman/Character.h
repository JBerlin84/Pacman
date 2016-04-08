#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "../Engine/Timer.h"

using GameEngine::Timer;

class Character {
protected:
	int lerpSpeed;
	int lerpAmmount;
	Direction movingDirection;
	Direction nextMove;
	Timer huntingtime;
	bool isHunting;
	
	int lerp(int start, int end, int t) {
		return start + (end - start) * t;
	}

public:
	int x, y;
	int ix;
	int iy;
	int points;
	int lives;

	Character(int x, int y, int lives) {
		this->x = x;
		this->y = y;
		this->lives = lives;
		points = 0;

		ix = 0;
		iy = 0;

		movingDirection = Direction_right;
		nextMove = Direction_none;

		lerpSpeed = PLAYER_SPEED;
		lerpAmmount = 0;

		huntingtime.reset();
		isHunting = false;
	}

	Direction getMovingDirection() {
		return movingDirection;
	}

	void moveUp() {
		nextMove = Direction_up;
	}

	void moveDown() {
		nextMove = Direction_down;
	}

	void moveLeft() {
		nextMove = Direction_left;
	}

	void moveRight() {
		nextMove = Direction_right;
	}

	bool isHuntingTime() {
		if(isHunting) {
			if(huntingtime.stopwatch(HUNTING_TIME))
				isHunting = false;
		}

		return isHunting;
	}

	void startHuntingTime() {
		isHunting = true;
		huntingtime.reset();
	}

	int getCurrentSprite(int version) {
		int currentDirection = 0;
		if (movingDirection == Direction_up) {
			if (version == 1)
				currentDirection = CHARACTER_PLAYER_UP_1;
			else 
				currentDirection = CHARACTER_PLAYER_UP_2;
		}
		else if (movingDirection == Direction_down) {
			if (version == 1)
				currentDirection = CHARACTER_PLAYER_DOWN_1;
			else
				currentDirection = CHARACTER_PLAYER_DOWN_2;
		}
		else if (movingDirection == Direction_left) {
			if (version == 1)
				currentDirection = CHARACTER_PLAYER_LEFT_1;
			else
				currentDirection = CHARACTER_PLAYER_LEFT_2;
		}
		else { // Default state is right
			if (version == 1)
				currentDirection = CHARACTER_PLAYER_RIGHT_1;
			else 
				currentDirection = CHARACTER_PLAYER_RIGHT_2;
		}

		return currentDirection;
	}

	// Make sure we switch direction immediately if possible and that we dont stop when we try to walk into a wall.
	void update() {
		// Change direction of player at the correct position.
		if (lerpAmmount == 0) {
			if (nextMove == Direction_up && maze[y-1][x] != Tile_wall) {
				movingDirection = nextMove;
			}
			else if (nextMove == Direction_down && maze[y+1][x] != Tile_wall) {
				movingDirection = nextMove;
			}
			else if (nextMove == Direction_left && maze[y][x-1] != Tile_wall) {
				movingDirection = nextMove;
			}
			else if (nextMove == Direction_right && maze[y][x+1] != Tile_wall) {
				movingDirection = nextMove;
			}
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
};

#endif