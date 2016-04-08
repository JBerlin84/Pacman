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

namespace GameEngine {
    Input::Input(HWND hwnd) {
        window = hwnd;
    
        //create DirectInput object
        DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
            IID_IDirectInput8, (void**)&di, NULL );
    
        //initialize keyboard
        di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
        keyboard->SetDataFormat( &c_dfDIKeyboard );
        keyboard->SetCooperativeLevel( window, 
            DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
        keyboard->Acquire();
    }
    
    Input::~Input() {
        di->Release();
        keyboard->Release();
    }
    
    void Input::Update() {
        //poll state of the keyboard
        keyboard->Poll();
        if (!SUCCEEDED(keyboard->GetDeviceState(256,(LPVOID)&keyState)))
        {
            //keyboard device lost, try to re-acquire
            keyboard->Acquire();
        }
    }
}
