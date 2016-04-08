/************************************************************************
Author: Jimmy Berlin
Last modified: 2016-02-01
Project: Assignment 1, Maze

desc:
definitions for program
contains character class that will be moved to new file in next assignment.
************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

#include "..\Engine\GameEngine.h"

// Script language stuff
// LUA
extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
}
lua_State* luaState;	// lua environment


// Engine params
#define WIDTH DEFAULT_WIDTH		// Default width 1920
#define HEIGHT DEFAULT_HEIGHT	// Default height 1080
#define COLOR_DEPTH 32
#define FULLSCREEN false
#define APPLICATION_TITLE "Pacman"

#define DEBUG_BUILD true

// Sprite sheet params
#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define ANIMATION_SPEED 150

// player sprites
#define CHARACTER_PLAYER_RIGHT_1 10
#define CHARACTER_PLAYER_RIGHT_2 11
#define CHARACTER_PLAYER_DOWN_1  24
#define CHARACTER_PLAYER_DOWN_2  25
#define CHARACTER_PLAYER_LEFT_1  38
#define CHARACTER_PLAYER_LEFT_2  39
#define CHARACTER_PLAYER_UP_1    52
#define CHARACTER_PLAYER_UP_2    53

// Enemy sprites
#define ENEMY_RIGHT 0
#define ENEMY_DOWN 14
#define ENEMY_LEFT 28
#define ENEMY_UP 42
// blinky first from left
// clyde second from left
// pinky third from left
// inky forth from left
#define ENEMY_BLINKY 0
#define ENEMY_INKY 2
#define ENEMY_PINKY 4
#define ENEMY_CLYDE 6
#define ENEMY_SCARED 12

// Combined params for player and enemy
#define HUNTING_TIME 10000

// Player params
#define START_POSITION_X 13
#define START_POSITION_Y 23
#define PLAYER_SPEED 2

// Defines the position in the sprite sheet of the different tile types.
#define TILE_SMALL_FOOD				29
#define TILE_LARGE_FOOD				9
#define TILE_LARGE_FOOD_SHINING		19

#define TILE_EMPTY					-1
#define TILE_UPPER_SINGLE			15
#define TILE_LOWER_SINGLE			36
#define TILE_RIGHT_SINGLE			8
#define TILE_LEFT_SINGLE			6
#define TILE_VERTICAL_SINGLE		26
#define TILE_HORIZONTAL_SINGLE		7

#define TILE_UPPER_LEFT_SINGLE		3
#define TILE_UPPER_RIGHT_SINGLE		5
#define TILE_LOWER_LEFT_SINGLE		23
#define TILE_LOWER_RIGHT_SINGLE		25

#define	TILE_UPPER_LEFT_FILLED		0
#define TILE_UPPER_MIDDLE_FILLED	1
#define TILE_UPPER_RIGHT_FILLED		2
#define	TILE_MIDDLE_LEFT_FILLED		10
#define TILE_MIDDLE_MIDDLE_FILLED	11
#define TILE_MIDDLE_RIGHT_FILLED	12
#define	TILE_LOWER_LEFT_FILLED		20
#define TILE_LOWER_MIDDLE_FILLED	21
#define TILE_LOWER_RIGHT_FILLED		22

#define	TILE_UPPER_LEFT_INNER		17
#define TILE_UPPER_RIGHT_INNER		18
#define	TILE_LOWER_LEFT_INNER		27
#define TILE_LOWER_RIGHT_INNER		28

#define TILE_LEFT_SIDE_LOWER_INNER		31
#define TILE_LEFT_SIDE_UPPER_INNER		30
#define TILE_RIGHT_SIDE_LOWER_INNER		35
#define TILE_RIGHT_SIDE_UPPER_INNER		34
#define TILE_TOP_SIDE_LEFT_INNER		32
#define TILE_TOP_SIDE_RIGHT_INNER		33

// Maze dimensions
#define MAZE_WIDTH 28
#define MAZE_HEIGHT 31
/*
0 = empty
1 = wall
2 = small bite
3 = large bite
4 = spawn area
*/
////////////////////////////////////////////
// IMPORTANT: first agrument is y, second x
////////////////////////////////////////////
int maze[MAZE_HEIGHT][MAZE_WIDTH] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
		{ 1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1, },
		{ 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
		{ 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, },
		{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
		{ 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 4, 4, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
		{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
		{ 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, },
		{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
		{ 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
		{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
		{ 1, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 3, 1, },
		{ 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, },
		{ 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, },
		{ 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, },
		{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, },
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};
int numberOfFruitsLeft;
enum TileTypesEnum { Tile_empty, Tile_wall, Tile_small, Tile_large, Tile_spawn };
enum Direction { Direction_none, Direction_up, Direction_down, Direction_left, Direction_right };

#include "Character.h"
#include "Enemy.h"

Character player(START_POSITION_X, START_POSITION_Y, 3);
Enemy *enemies[4];

#endif