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

#include "GameEngine.h"

namespace GameEngine {
    Engine::Engine() {
        srand((unsigned int)time(NULL));
        maximizeProcessor = false;
        frameCount_core = 0;
        frameRate_core = 0;
        frameCount_real = 0;
        frameRate_real = 0;
        ambientColor = D3DCOLOR_RGBA(255,255,255, 0);
        windowHandle = 0;
        pauseMode = false;
        
        //set default values
        this->setAppTitle("Advanced2D");
        this->setScreenWidth(640);
        this->setScreenHeight(480);
        this->setColorDepth(32);
        this->setFullscreen(false);
        
        //window handle must be set later on for DirectX!
        this->setWindowHandle(0);
    
    }
    
    Engine::~Engine() {
		if (this->directSound) delete this->directSound;
		if (this->device) this->device->Release();
		if (this->d3d) this->d3d->Release();
    }
    
    void Engine::message(std::string message, std::string title) {
        MessageBox(0, message.c_str(), title.c_str(), 0);
    }
    
    void Engine::fatalerror(std::string message, std::string title) {
        this->message(message,title);
        Shutdown();
    }
    
    bool Engine::Init(int width, int height, int colordepth, bool fullscreen) {
       // Initialize Direct3D
        this->d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (this->d3d == NULL)
            return false;
    
        // Get system desktop color depth
        D3DDISPLAYMODE dm;
        this->d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
    
        // Set configuration options for Direct3D
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = (!fullscreen);
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
        d3dpp.BackBufferFormat = dm.Format;
        d3dpp.BackBufferCount = 1;
        d3dpp.BackBufferWidth = width;
        d3dpp.BackBufferHeight = height;
        d3dpp.hDeviceWindow = windowHandle;
    
        // Create Direct3D device
        this->d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            this->windowHandle,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &d3dpp,
            &this->device);
    
        if (this->device == NULL)
			return false;
    
        // Clear the backbuffer to black
        this->ClearScene(D3DCOLOR_XRGB(0,0,0));
    
        // Create pointer to the back buffer
        this->device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->backbuffer);

        // Initialize renderer
        HRESULT result = D3DXCreateSprite(this->device, &this->sprite_handler);
        if (result != D3D_OK)
			return false;
        
		// Initialize DirectSound
		directSound = new CSoundManager();
		result = directSound->Initialize(this->getWindowHandle(), DSSCL_PRIORITY);
		if (result != DS_OK) {
			return false;
		}
		result = directSound->SetPrimaryBufferFormat(2, 22050, 16);
		if (result != DS_OK) {
			return false;
		}

        // Call game initialization extern function
        if (!game_init(this->getWindowHandle()))
            return false;

        // Initialize DirectInput
        input = new Input(this->getWindowHandle());

        return true;
    }    
    
	CSound *Engine::LoadSound(std::string filename) {
		HRESULT result;
		CSound *wave = NULL;

		char s[255];
		sprintf_s(s, "%s", filename.c_str());
		result = directSound->Create(&wave, s);
		if (result != DS_OK) {
			wave = NULL;
		}
		return wave;
	}

	void Engine::playSound(CSound *sound) {
		sound->Play();
	}

	void Engine::LoopSound(CSound *sound) {
		sound->Play(0, DSBPLAY_LOOPING);
	}

	void Engine::StopSound(CSound *sound) {
		sound->Stop();
	}

    void Engine::ClearScene(D3DCOLOR color) {
        this->device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    }
    
    void Engine::SetAmbient(D3DCOLOR colorvalue) {
        this->ambientColor = colorvalue;
        this->device->SetRenderState(D3DRS_AMBIENT, this->ambientColor);
    }

    bool Engine::Render_Start() {
		if (!this->device) return false;
		if (this->device->BeginScene() != D3D_OK) return false;

        if (sprite_handler->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
            return false;
        else
            return true;
    }
    
    bool Engine::Render_Stop() {
       sprite_handler->End();

	   if (!this->device) return false;
	   if (this->device->EndScene() != D3D_OK) return false;
	   if (device->Present(NULL, NULL, NULL, NULL) != D3D_OK) return false;

       return true;
    }

    void Engine::Shutdown() {
        gameover = true;
    }
    
    void Engine::UpdateKeyboard() {
        static char old_keys[256];
        for (int n=0; n<255; n++) 
        {
            //check for key press
            if (input->GetKeyState(n) & 0x80) {
                game_keyPress(n);
                old_keys[n] = input->GetKeyState(n);
            } 
            //check for release
            else if (old_keys[n] & 0x80) {
                game_keyRelease(n);
                old_keys[n] = input->GetKeyState(n);
            }
        }
    }

    void Engine::Update() {
        static Timer timedUpdate;
    
        //calculate core framerate
        frameCount_core++;
        if (coreTimer.stopwatch(999)) {
            frameRate_core = frameCount_core;
            frameCount_core = 0;
        }

        //update input devices
        input->Update();
        this->UpdateKeyboard();
    
        //fast update with no timing
		//game_update();	// Don't have any delta time yet, dont use.
		
        //update with 60fps timing
        if (!timedUpdate.stopwatch(14)) {
            if (!this->getMaximizeProcessor()) 
            {
                Sleep(1);
            }
        }
        else {
            //calculate real framerate
            frameCount_real++; 
            if (realTimer.stopwatch(999)) {
                frameRate_real = frameCount_real;
                frameCount_real = 0;
            }
			game_update();		// Temporary until delta time is created.

            Render_Start();
            game_render();
            Render_Stop();
        }
    
    }
}
