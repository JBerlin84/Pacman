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

-- A* function and other helper functions
function distance_between(start, goal)
	return math.sqrt(math.pow(goal.x-start.y,2) + math.pow(goal.y-start.y, 2))
end

function lowest_f_score(set, f_score)
	lowest, best = 1/0, nil
	for i, node in ipairs(set) do
		local score = f_score[node]
		if score < lowest then
			lowest, bestNode = score, node
		end
	end
	return bestNode
end

function remove_node(set, node)
	for i, n in ipairs(set) do
		if n == node then
			set[i] = set[#set]
			set[#set] = nil
			break
		end
	end
end

function in_set(set, node)
	for i, n in ipairs(set) do
		if n == node then
			return true
		end
	end
end

function unwind(path, map, currentNode)
	if map[currentNode] then
		table.insert(path, 1, map[currentNode])
		return unwind(path, map, map[currentNode])
	else
		return path
	end
end

function neighbors(current)
	local neighbors = {}
	if(MAZE[current.x-1][current.y] == 0) then
		local n = {x=current.x-1, y=current.y}
		neighbors.insert(n)
	end
	if(MAZE[current.x+1][current.y] == 0) then
		local n = {x=current.x+1, y=current.y}
		neighbors.insert(n)
	end
	if(MAZE[current.x][current.y-1] == 0) then
		local n = {x=current.x, y=current.y-1}
		neighbors.insert(n)
	end
	if(MAZE[current.x][current.y+1] == 0) then
		local n = {x=current.x, y=current.y+1}
		neighbors.insert(n)
	end

	return neighbors
end

function aStar(start, goal)
	local file = io.open("aStar.txt", "w")
	io.output(file)

	io.write("inside aStar")
	io.close(file)

	local closedSet = {}
	local openSet = {start}
	local cameFrom = {}

	local g_score = {}
	local f_score = {}
	g_score[start] = 0
	f_score[start] = 0 + distance_between(start, goal)

	while #openSet > 0 do
		local current = lowest_f_score(openSet, f_score)
		if current == goal then
			local path = unwind({}, cameFrom, goal)
			return path
		end

		remove_node(openset, current)
		closedSet.insert(current)	-- table.insert(closedSet, current)

		local neighbors = neighbors(current)
		for i, neighbor in ipairs(neighbors) do
			if not in_set(closedSet, neighbor) then
				local tentative_g_score = g_score[current] + distance_between(current, neighbor)

				if not in_set(openSet, neighbor) or tentative_g_score < g_score[neighbor] then
					came_from[neighbor] = current
					g_score[neighbor] = tentative_g_score
					f_score[neighbor] = g_score[neighbor] + distance_between(neighbor, goal)
					if not in_set(openSet, neighbor) then
						openSet.insert(neighbor)
					end
				end
			end
		end
	end


end


--[[
function lowest_f_score(set, f_score)
	lowest, best = 1/0, nil
	for i, node in ipairs(set) do
		local score = f_score[node]
		if score < lowest then
			lowest, bestNode = score, node
		end
	end
	return bestNode
end

function aStar(from, to)
	--local closedSet = {}
	local openSet = {from}
	local cameFrom = {}
	local fScore = {}

	while #openSet > 0 do
		local current = lowest_f_score(openSet, fScore)
	end
end
]]

-- enemy is the enemy type. 0 = Blinky; 2 = Inky; 4 = Pinky; 6 = Clyde
function ai(enemy, x, y)
	if(enemy == ENEMY_TYPE_BLINKY) then
		blinky.x = x;
		blinky.y = y;
		return blinky_ai()
	elseif(enemy == ENEMY_TYPE_INKY) then
		inky.x = x;
		inky.y = y;
		return inky_ai()
	elseif(enemy == ENEMY_TYPE_PINKY) then
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

	aStar(inky, player)

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
