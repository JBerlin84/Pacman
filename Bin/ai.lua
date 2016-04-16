--[[
collection of variables, just to get an overview.

MAZE : holds the complete maze in a single array, this is changed in function init to be a two dimensional matrix.
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
DIRECTION_UP
DIRECTION_DOWN
DIRECTION_LEFT
DIRECTION_RIGHT
]]--

require"data"
MAZE = {}

-- Call this function in the beginning of the program to initialize the
-- LUA environment.
function init()
	MAZE = formatMaze(MAZE)
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
function test()
	local file = io.open("debug.txt", "w")
	io.output(file)

	io.write("MAZE_WIDTH: ")
	io.write(MAZE_WIDTH)
	io.write("\nMAZE_HEIGHT: ")
	io.write(MAZE_HEIGHT)
	io.write("\nTotal to be moved: ")
	io.write(MAZE_WIDTH * MAZE_HEIGHT)
	io.write("\nTotal in array: ")
	io.write(#MAZE)
	io.write("\n\n\n")

	io.write("actual MAZE_WIDTH: ")
	io.write(#MAZE[1])
	io.write("\nactual MAZE_HEIGHT: ")
	io.write(#MAZE)
	io.write("\n")

	io.write("\nBlinky: " .. blinky.name .. " - " .. blinky.x .. ":" .. blinky.y)
	io.write("\ninky: " .. inky.name .. " - " .. inky.x .. ":" .. inky.y)
	io.write("\nClyde: " .. clyde.name .. " - " .. clyde.x .. ":" .. clyde.y)
	io.write("\nPinky: " .. pinky.name .. " - " .. pinky.x .. ":" .. pinky.y)

	io.write("\n\n\n")

	-- print out the matrix so we see its correct
	for y=1, MAZE_HEIGHT, 1 do
		for x=1, MAZE_WIDTH, 1 do
			io.write(MAZE[y][x])
		end
		io.write("\n")
	end

	io.close(file)

	-- return "this is from the lua test function"
	return maze[11]
end

-- update lua state
function updatePlayerState(x, y, direction)
	player.x = x
	player.y = y
	player.direction = direction

	return true;
end

-- A* function
function aStar()

end

-- enemy is the enemy type. 0 = Blinky; 2 = Inky; 4 = Pinky; 6 = Clyde
function ai(enemy, x, y)
	if(enemy == 0) then
		blinky.x = x;
		blinky.y = y;
		return blinky_ai()
	elseif(enemy == 2) then
		inky.x = x;
		inky.y = y;
		return inky_ai()
	elseif(enemy == 4) then
		pinky.x = x;
		pinky.y = y;
		return pinky_ai()
	else
		clyde.x = x;
		clyde.y = y;
		return clyde_ai()
	end
end

function blinky_ai()
	return 0, 0
end

function inky_ai()
	repeat
		randomDirection = math.random(4)
	until validDirection(randomDirection, inky.x, inky.y)

	return math.floor(randomDirection)
end

function pinky_ai()
	return 0, 0
end

function clyde_ai()
	return 0, 0
end

-- Check if the direction is valid to travel in.
function validDirection(direction, x, y)
	left = MAZE[x-1][y]
	right = MAZE[x+1][y]
	up = MAZE[x][y-1]
	down = MAZE[x][y+1]

	if(direction == DIRECTION_LEFT and left~=1) then
		return true
	elseif(direction == DIRECTION_RIGHT and right~=1) then
		return true
	elseif(direction == DIRECTION_UP and up~=1) then
		return true
	elseif(direction == DIRECTION_DOWN and down~=1) then
		return true
	else
		return false
	end
end
