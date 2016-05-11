--timeIntervals = {i = 1, intervals={7, 20, 7, 20, 5, 20, 5, math.maxinteger}}
timeIntervals = {i = 1, intervals={2, 3, 2, 3, 2, 3, 2, math.maxinteger}}

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
		return true --should be true
	end
	return false --should be false
end


------------------------------------------------------------------------
------------------------------------------------------------------------
--trying yield
--require "socket"


-- test function
function start()
	local chase = false
	restartTimer()

	while true do
		if stopWatch(timeIntervals.intervals[timeIntervals.i]) then
			restartTimer()
			timeIntervals.i = timeIntervals.i+1
			print("now after stopwatch interval")
--			if chase then
--				chase = false
--				print("Chase")
--			else
--				chase = true
--				print("Scatter")
--			end
		end
	end
end

