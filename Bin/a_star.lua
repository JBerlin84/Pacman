-- start lua53.exe
-- dofile("a_star.lua")
-- start()

--require "test"
--[[
function start()
  local start = {x=2, y=2}
  local goal = {x=8, y=6}

  local nextDirection = getNextDirection(start, goal, DIRECTION_RIGHT)
end]]

function getNextTile(position, target, direction)
  -- fetch all possible tiles.
  local allNodes = createNodeList(position, target)

  -- remove node behind enemy, we are not allowed to change to opposite direction unless state change.
  local nodeToRemove={x=0, y=0}
  if direction == DIRECTION_UP then
    nodeToRemove={x=position.x, y=position.y+1}
  elseif direction == DIRECTION_DOWN then
    nodeToRemove={x=position.x, y=position.y-1}
  elseif direction == DIRECTION_LEFT then
    nodeToRemove={x=position.x+1, y=position.y}
  elseif direction == DIRECTION_RIGHT then
    nodeToRemove={x=position.x-1, y=  position.y}
  end
  remove_node(allNodes, nodeToRemove)

  -- get the next tile
  local path = aStar(position, target, allNodes)

  local file = io.open("debug.txt", "a")
  io.output(file)
  for _, n in ipairs(path) do
    io.write("(" .. n.x .. ":" .. n.y .. ")\n")
  end
  io.write("\n\n\n\n\n\n\n")
  io.close(file)



  return path[2]  -- the second tile is the next one to step on.
end


function aStar ( start, goal, nodes )
  local closedset = {}
  local openset = { start }
  local came_from = {}

  local g_score, f_score = {}, {}
  g_score [ start ] = 0
  f_score [ start ] = g_score [ start ] + heuristic_cost_estimate ( start, goal )

  while #openset > 0 do
    local current = fetch_lowest_fscore ( openset, f_score )
    if current == goal then
      local path = unwindPath ( {}, came_from, goal )
      table.insert ( path, goal )
      return path
    end

    remove_node ( openset, current )
    table.insert ( closedset, current )

    local neighbors = getNeighbors ( current, nodes )
    for _, neighbor in ipairs ( neighbors ) do
      if not inSet ( closedset, neighbor ) then

        local tentative_g_score = g_score [ current ] + heuristic_cost_estimate ( current, neighbor )

        if not inSet ( openset, neighbor ) or tentative_g_score < g_score [ neighbor ] then
          came_from   [ neighbor ] = current
          g_score   [ neighbor ] = tentative_g_score
          f_score   [ neighbor ] = g_score [ neighbor ] + heuristic_cost_estimate ( neighbor, goal )
          if not inSet ( openset, neighbor ) then
            table.insert ( openset, neighbor )
          end
        end
      end
    end
  end
end

function createNodeList(start, goal)
  --local nodeList = {start, goal}
  local nodeList = {}
  for iy=1, #MAZE do
    for ix=1, #MAZE[iy] do
      if MAZE[iy][ix] ~= 1 then
        local n={x=ix, y=iy}
        if equals(n, start) then
          table.insert(nodeList, start)
          --nodeList[#nodeList+1] = start
        elseif equals(n, goal) then
          table.insert(nodeList, goal)
          --nodeList[#nodeList+1] = goal
        else
          table.insert(nodeList, n)
          --nodeList[#nodeList+1] = n
        end
      end
    end
  end
  return nodeList
end

-- Helper functions
function equals(n1, n2)
  if not n1 then
    return false
  elseif not n2 then
    return false
  else
    local x = n1.x == n2.x
    local y = n1.y == n2.y
    return x and y
  end
end

function heuristic_cost_estimate(start, goal)
  -- heuristic cost estimate using manhattan distance.
  local dx = math.abs(start.x - goal.x)
  local dy = math.abs(start.y - goal.y)
  return dx + dy
end

function fetch_lowest_fscore(set, f_score)
  local lowestScore = 1000
  local bestNode = nil

  for _, node in ipairs(set) do
    local score = f_score[node]
    if score < lowestScore then
      lowestScore = score
      bestNode = node
    end
  end
  return bestNode
end

function remove_node(set, node)
  for q, n in ipairs(set) do
    if equals(n,node) then
      set[q] = set[#set]
      set[#set] = nil
      break
    end
  end
end

function getNeighbors(node, nodes)
  local neighbors = {}
  local nx = node.x
  local ny = node.y

  for _, n in ipairs(nodes) do
    if (n.x == nx and (n.y-1 == ny or n.y+1 == ny)) or
        (n.y == ny and (n.x-1 == nx or n.x+1 == nx)) then
        table.insert(neighbors, n)
    end
  end

  -- add left to right wrap.
  if nx == 1 and ny==15 then
    temp = {x=28, y=15}
    for _, n in ipairs(nodes) do
      if equals(temp, n) then
        table.insert(neighbors, n)
        break
      end
    end
  end
  -- add right to left wrap.
  if nx == 28 and ny==15 then
    temp = {x=1, y=15}
    for _, n in ipairs(nodes) do
      if equals(temp, n) then
        table.insert(neighbors, n)
        break
      end
    end
  end

  return neighbors
end

function inSet(set, node)
  for _, n in ipairs(set) do
    if equals(n,node) then
      return true
    end
  end
  return false
end

function unwindPath(path, set, current)
  if set[current] then
    table.insert(path, 1, set[current])
    return unwindPath(path, set, set[current])
  else
    return path
  end
end
