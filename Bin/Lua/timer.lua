--gameStateIntervals = {i = 1, intervals={7, 20, 7, 20, 5, 20, 5, math.maxinteger}}
gameStateIntervals = {i = 1, intervals={2, 30, 2, 30, 2, 30, 2, math.maxinteger}}

-- restart the timer
function restartTimer()
	timer = os.clock()
end

-- returns true if the given time in seconds have passed.
-- resolution in windows machine is in seconds.
-- (and since this is a course in game programming in windows,
--	i use it with good conscience)
function stopWatch(time)
	local t = os.clock()
	local diff = t - timer
	if diff > time then
		return true
	end
	return false
end
