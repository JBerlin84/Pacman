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

#ifndef TIMER_H_
#define TIMER_H_

namespace GameEngine {
	class Timer {
	private:
		DWORD timer_start;
		DWORD stopwatch_start;

	public:
		Timer();
		DWORD getTimer() { return (DWORD)(timeGetTime()); }
		DWORD getStartTimeMillis() { return (DWORD)(timeGetTime() - timer_start); }
	
		void sleep(int ms);
		void reset();
		bool stopwatch(int ms);
	};
}

#endif
