--[[
collection of variables, just to get an overview.

MAZE : holds the complete maze in a single array, this is changed in function init to be a two dimensional matrix. 1=wall 0=walkable area
MAZE_WIDTH : number of columns in the original maze
MAZE_HEIGHT : number of rows in the original maze

CURRENT_STATE : the current state of enemies/game
NEW_STATE : the new state of the enemies/game

blinky : holds the state for blinky
inky : holds the state for inky
pinky : holds the state for pinky
clyde : holds the state for clyde

ENEMY_TYPE_BLINKY : Check wether it is enemy type blinky
ENEMY_TYPE_INKY : Check wether it is enemy type inky
ENEMY_TYPE_PINKY : Check wether it is enemy type pinky
ENEMY_TYPE_CLYDE : Check wether it is enemy type clyde

Moving directions that can be used with moving direction from the code.
DIRECTION_NONE
DIRECTION_UP
DIRECTION_DOWN
DIRECTION_LEFT
DIRECTION_RIGHT

consumedDots : the number of dots pacman has consumed.
frightened : is it hunting time for ghosts or not?
]]

require "data"
require "a_star"
require "timer"
MAZE = {}

--------------------------------------------------------------------------------
-- initializations functions
--------------------------------------------------------------------------------

-- Call this function in the beginning of the program to initialize the
-- LUA environment.
function init()
	MAZE = formatMaze(MAZE)
	gameState = GAME_STATE_SCATTER

	-- wander coordinates
	blinky.scatter = {x=2, y=2}
	pinky.scatter = {x=MAZE_WIDTH-1, y=2}
	inky.scatter = {x=2, y=MAZE_HEIGHT-1}
	clyde.scatter = {x=MAZE_WIDTH-1, y=MAZE_HEIGHT-1}

	restartTimer()

	math.randomseed(os.time())
end

-- format single array of map to twodimensional matrix of map
function formatMaze(MAZE_in)
	local temp = {}
	for y=1, MAZE_HEIGHT, 1 do
		temp[y] = {}
		for x=1, MAZE_WIDTH, 1 do
			local index = MAZE_WIDTH * (y-1) + (x-1)
			temp[y][x] = MAZE_in[index]
		end
	end
	return temp
end


-- test function.
-- used to see wether it creates the correct data
-- stores a debug.txt in binary folder.
function printToFile(message)
	local file = io.open("debug.txt", "a")
	io.output(file)
	io.write(message)
	io.close(file)
end

--------------------------------------------------------------------------------
-- update functions
--------------------------------------------------------------------------------
function updateLuaState()
	if stopWatch(gameStateIntervals.intervals[gameStateIntervals.i]) then
		restartTimer()
		gameStateIntervals.i = gameStateIntervals.i+1
		if (gameState - GAME_STATE_CHASE) == 0 then
			gameState = GAME_STATE_SCATTER
		elseif (gameState - GAME_STATE_SCATTER) == 0 then
			gameState = GAME_STATE_CHASE
		end
	end
end

-- update player in lua state
function updatePlayerState(x, y, direction)
	player.x = x + 1	-- index in lua start with 1, in c 0
	player.y = y + 1	-- index in lua start with 1, in c 0
	player.direction = direction

	return true;
end

-- enemy is the enemy type.
-- distribute call to correct ai-function
function getNextMoveById(enemy, x, y)
	if(enemy == ENEMY_TYPE_BLINKY) then
		blinky.x = x + 1	-- index in lua start with 1, in c 0
		blinky.y = y + 1	-- index in lua start with 1, in c 0
		if gameState == GAME_STATE_SCATTER then
			return scatter_ai(blinky)
		elseif frightened then
			return frightened_ai(blinky)
		else
			return blinky_ai()
		end
	elseif(enemy == ENEMY_TYPE_PINKY) then
		pinky.x = x + 1
		pinky.y = y + 1
		if gameState == GAME_STATE_SCATTER then
			return scatter_ai(pinky)
		elseif frightened then
			return frightened_ai(pinky)
		else
			return pinky_ai()
		end
	elseif(enemy == ENEMY_TYPE_INKY and consumedDots > 30) then
		inky.x = x + 1
		inky.y = y + 1
		if gameState == GAME_STATE_SCATTER then
			return scatter_ai(inky)
		elseif frightened then
			return frightened_ai(inky)
		else
			return inky_ai()
		end
		-- clyde should not move until a third of the dots are eaten.
	elseif(enemy == ENEMY_TYPE_CLYDE and consumedDots > 82) then
		clyde.x = x + 1
		clyde.y = y + 1
		if gameState == GAME_STATE_SCATTER then
			return scatter_ai(clyde)
		elseif frightened then
			return frightened_ai(clyde)
		else
			return clyde_ai()
		end
	end
end

--------------------------------------------------------------------------------
-- ai functions
--------------------------------------------------------------------------------

function blinky_ai()  -- red ghost
	local nextTile = getNextTile(blinky, player, blinky.direction)
	local newDirection = findDirection(blinky, nextTile)
	blinky.direction = newDirection

	return newDirection
end

function pinky_ai()  -- pink ghost
	-- target tile for pinky is four tiles ahead of pacman
	-- TODO: Clamp this to world coord
	local targetTile = {}
	if player.direction == DIRECTION_UP then
		targetTile = {x=player.x, y=player.y-4}
	elseif player.direction == DIRECTION_DOWN then
		targetTile = {x=player.x, y=player.y+4}
	elseif player.direction == DIRECTION_LEFT then
		targetTile = {x=player.x-4, y=player.y}
	else
		targetTile = {x=player.x+4, y=player.y}
	end

	local tempX, tempY = clampToMazeCoord(targetTile.x, targetTile.y)
	if not validCoord(tempX, tempY) then
		tempX, tempY = findClosestValidCoord(tempX, tempY)
	end
	targetTile = {x = tempX, y = tempY}

	local nextTile = getNextTile(pinky, targetTile, pinky.direction)
	local newDirection = findDirection(pinky, nextTile)
	pinky.direction = newDirection

	return newDirection
end

function inky_ai()  -- blue ghost
	-- TODO: Clamp this to world coord
	local targetTile = {}
	if player.direction == DIRECTION_UP then
		targetTile = {x=player.x, y=player.y-2}
	elseif player.direction == DIRECTION_DOWN then
		targetTile = {x=player.x, y=player.y+2}
	elseif player.direction == DIRECTION_LEFT then
		targetTile = {x=player.x-2, y=player.y}
	else
		targetTile = {x=player.x+2, y=player.y}
	end
	printToFile("inky ai after targetTile\n")

	local deltaX = (blinky.x - targetTile.x) * 2
	local deltaY = (blinky.y - targetTile.y) * 2
	printToFile("inky ai after delta pos\n")
	targetTile = {x = (inky.x + deltaX), y = (inky.y + deltaY)}

	local tempX, tempY = clampToMazeCoord(targetTile.x, targetTile.y)
	if not validCoord(tempX, tempY) then
		tempX, tempY = findClosestValidCoord(tempX, tempY)
	end
	targetTile = {x = tempX, y = tempY}

	printToFile("inky ai after total targetTile\n")
	printToFile("target: (" .. targetTile.x .. ":" .. targetTile.y .. ")")

	local nextTile = getNextTile(inky, targetTile, inky.direction)
	local newDirection = findDirection(inky, nextTile)
	inky.direction = newDirection

	return newDirection
end

function clyde_ai()  -- orange ghost
	local distance = math.abs(clyde.x - player.x) + math.abs(clyde.y - player.y)

	local targetTile = {}
	if distance < 9 then
		targetTile = {x = player.x, y = player.y}
	else
		targetTile = clyde.scatter
	end

	local nextTile = getNextTile(clyde, targetTile, clyde.direction)
	local newDirection = findDirection(clyde, nextTile)
	clyde.direction = newDirection

	return newDirection
end

-- find the next direction according to scatter function
function scatter_ai(e)
	local nextTile = getNextTile(e, e.scatter, e.direction)
	local newDirection = findDirection(e, nextTile)
	e.direction = newDirection

	return newDirection
end

-- find the next direction when you are frightened
function frightened_ai(e)
	local randX = 0
	local randY = 0

	-- TODO: we have to fix this. make it faster
	repeat
		randX = math.random(2,MAZE_WIDTH-1)
		randY = math.random(2,MAZE_HEIGHT-1)
	until(validCoord(randX, randY))

	local nextTile = getNextTile(e, {x=randX, y=randY}, e.direction)
	local newDirection = findDirection(e, nextTile)
	e.direction = newDirection

	return newDirection
end

--------------------------------------------------------------------------------
-- Helper functions
--------------------------------------------------------------------------------

-- check if it is possible to walk on given tile
function validCoord(xCoord, yCoord)
	return MAZE[yCoord][xCoord] == 0
end

-- clamp coord to maze dimensions.
function clampToMazeCoord(xCoord, yCoord)
	local newX = xCoord
	local newY = yCoord
	if newX < 2 then
		newX = 2;
	end
	if newX >= MAZE_WIDTH then
		newX = MAZE_WIDTH - 1;
	end
	if newY < 2 then
		newY = 2
	end
	if newY >= MAZE_HEIGHT then
		newY = MAZE_HEIGHT - 1;
	end

	return newX, newY
end

-- find an approximation of the closest coord that is valid
function findClosestValidCoord(xCoord, yCoord)
	for i=-1, 1, 1 do
		for j=-1, 1, 1 do
			if MAZE[yCoord + i][xCoord + j] == 0 then
				return (xCoord + dx), (yCoord + dy)
			end
		end
	end
end

-- return direction constant between the two tiles.
function findDirection(position, nextTile)
	  -- find the new direction
	  local newDirection = 0
	  if nextTile.x < position.x then
	    newDirection = DIRECTION_LEFT
	  elseif nextTile.x > position.x then
	    newDirection = DIRECTION_RIGHT
	  elseif nextTile.y < position.y then
	    newDirection = DIRECTION_UP
	  elseif nextTile.y > position.y then
	    newDirection = DIRECTION_DOWN
	  else
	    newDirection = DIRECTION_NONE
	  end

	  return newDirection
end
