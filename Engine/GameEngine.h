/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Main class for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <dxerr.h>
#include <fstream>
#include "winmain.h"
#include "Timer.h"
#include "Texture.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Input.h"
#include "Font.h"
#include "DirectSound.h"

#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080

//macro to read the keyboard asynchronously
#define KEY_DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000)?1:0)

//external variables and functions
extern bool gameover;

extern bool game_preload();
extern bool game_init(HWND);
extern void game_update();
extern void game_end();
extern void game_render();
extern void game_keyPress(int key);
extern void game_keyRelease(int key);

namespace GameEngine {
	class Engine {
	private:
		HWND windowHandle;
		LPDIRECT3D9 d3d;
		LPDIRECT3DDEVICE9 device;
		LPDIRECT3DSURFACE9 backbuffer;
		LPD3DXSPRITE sprite_handler;
		std::string apptitle;
		bool fullscreen;
		int screenwidth;
		int screenheight;
		int colordepth;
		float screenScale;
		bool pauseMode;
		D3DCOLOR ambientColor;
		bool maximizeProcessor;
		Timer coreTimer;
		long frameCount_core;
		long frameRate_core;
		Timer realTimer;
		long frameCount_real;
		long frameRate_real;
		Input *input;
		void UpdateKeyboard();
		CSoundManager *directSound;

	public:
		Engine();
		virtual ~Engine();
		bool Init(int width, int height, int colordepth, bool fullscreen);
		void Close();
		void Update();
		void message(std::string message, std::string title = "ADVANCED 2D");
		void fatalerror(std::string message, std::string title = "FATAL ERROR");
		void Shutdown();
		void ClearScene(D3DCOLOR color);
		void SetAmbient(D3DCOLOR colorvalue);		
		bool Render_Start();
		bool Render_Stop();

		// getters and setters
		bool isPaused() { return this->pauseMode; }
		void setPaused(bool value) { this->pauseMode = value; }

		LPDIRECT3DDEVICE9 getDevice() { return this->device; }

		LPDIRECT3DSURFACE9 getBackBuffer() { return this->backbuffer; }

		LPD3DXSPRITE getSpriteHandler() { return this->sprite_handler; }

		void setWindowHandle(HWND hwnd) { this->windowHandle = hwnd; }
		HWND getWindowHandle() { return this->windowHandle; }

		std::string getAppTitle() { return this->apptitle; }
		void setAppTitle(std::string value) { this->apptitle = value; }

		long getFrameRate_core() { return this->frameRate_core; };
		long getFrameRate_real() { return this->frameRate_real; };

		int getScreenWidth() { return this->screenwidth; }
		void setScreenWidth(int value) { this->screenwidth = value; }
		int getScreenHeight() { return this->screenheight; }
		void setScreenHeight(int value) { this->screenheight = value; }
		int getColorDepth() { return this->colordepth; }
		void setColorDepth(int value) { this->colordepth = value; }
		bool getFullscreen() { return this->fullscreen; }
		void setFullscreen(bool value) { this->fullscreen = value; }
		float getScreenScale() { return this->screenScale; }
		void setScreenScale(float value) { this->screenScale = value; }

		bool getMaximizeProcessor() { return this->maximizeProcessor; }
		void setMaximizeProcessor(bool value) { this->maximizeProcessor = value; }

		CSound *LoadSound(std::string filename);
		void playSound(CSound *sound);
		void LoopSound(CSound *sound);
		void StopSound(CSound *sound);
	};
}

//define the global engine object (visible everywhere!)
extern GameEngine::Engine *engine;

#endif

