-- enemy is the enemy type. 0 = Blinky; 2 = Inky; 4 = Pinky; 6 = Clyde
-- up, down, left, right are tiles to the given direction from enemy
function ai(enemy, up, down, left, right)
	if(enemy == 0) then
		return blinky(up, down, left, right)
	elseif(enemy == 2) then
		return inky(up, down, left, right)
	elseif(enemy == 4) then
		return pinky(up, down, left, right)
	else
		return clyde(up, down, left, right)
	end
end

function blinky(up, down, left, right)
	return inky(up, down, left, right)
end

function inky(up, down, left, right)
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

function pinky(up, down, left, right)
	return inky(up, down, left, right)
end

function clyde(up, down, left, right)
	return inky(up, down, left, right)
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