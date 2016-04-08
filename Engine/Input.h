/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Input class for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#include "GameEngine.h"

#ifndef INPUT_H_
#define INPUT_H_

namespace GameEngine {
    class Input {
    private:
        HWND window; 
        IDirectInput8 *di; 
        IDirectInputDevice8 *keyboard;
        char keyState[256]; 

    public:
        Input( HWND window );
        virtual ~Input();
        void Update();

        char GetKeyState(int key) { return keyState[key]; }
    };
}

#endif