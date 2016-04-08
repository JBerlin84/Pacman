/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Timer class for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#include "GameEngine.h"

namespace GameEngine {
	Timer::Timer(void) {
		timer_start = timeGetTime();
		reset();
	}

	void Timer::sleep(int ms) {
		DWORD start = getTimer();
		while (start + ms > getTimer());
	}

	void Timer::reset() {
		stopwatch_start = getTimer();
	}

	bool Timer::stopwatch(int ms) {
		if ( timeGetTime() > stopwatch_start + ms ) {
			stopwatch_start = getTimer();
			return true;
		}
		else return false;
	}
}
