--[[
collection of variables, just to get an overview.

MAZE : holds the complete maze in a single array, this is changed in function init to be a two dimensional matrix.
MAZE_WIDTH : number of columns in the original maze
MAZE_HEIGHT : number of rows in the original maze
blinky : holds the state for blinky
inky : holds the state for inky
pinky : holds the state for pinky
clyde : holds the state for clyde
]]--

require"data"
MAZE = {}
-- 0 = Blinky; 2 = Inky; 4 = Pinky; 6 = Clyde
--blinky = createEnemy("Blinky", 14, 12)
--blinky = createEnemy{"Blinky", 14, 12}
--inky = createEnemy("Inky", 14,14)
--pinky = createEnemy("Pinky", 14, 16)
--clyde = createEnemy("Clyde", 13, 16)

--function init(MAZE_in, MAZE_HEIGHT_in, MAZE_WIDTH_in)
--function init(MAZE_in)
function init()
	--MAZE_WIDTH = MAZE_WIDTH_in
	--MAZE_HEIGHT = MAZE_HEIGHT_in
	MAZE = formatMaze(MAZE)
end

-- format single array of map to map matrix
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


-- enemy is the enemy type. 0 = Blinky; 2 = Inky; 4 = Pinky; 6 = Clyde
-- up, down, left, right are tiles to the given direction from enemy
function ai(enemy, up, down, left, right)
	if(enemy == 0) then
		return blinky_ai(up, down, left, right)
	elseif(enemy == 2) then
		return inky_ai(up, down, left, right)
	elseif(enemy == 4) then
		return pinky_ai(up, down, left, right)
	else
		return clyde_ai(up, down, left, right)
	end
end

function blinky_ai(up, down, left, right)
	return inky_ai(up, down, left, right)
end

function inky_ai(up, down, left, right)
	newX = 0
	newY = 0

	repeat
		randomDirection = math.random(4)
		if(randomDirection == 1) then
			newX = -1
			newY = 0
		elseif(randomDirection == 2) then
			newX = 1
			newY = 0
		elseif(randomDirection == 3) then
			newX = 0
			newY = -1
		else
			newX = 0
			newY = 1
		end
	until validDirection(newX, newY, up, down, left, right)

	return newX, newY
end

function pinky_ai(up, down, left, right)
	return inky_ai(up, down, left, right)
end

function clyde_ai(up, down, left, right)
	return inky_ai(up, down, left, right)
end

-- Check if the direction is valid to travel in.
function validDirection(x, y, up, down, left, right)
	if(x<0 and left~=1) then
		return true
	elseif(x>0 and right~=1) then
		return true
	elseif(y<0 and up~=1) then
		return true
	elseif(y>0 and down~=1) then
		return true
	else
		return false
	end
end
