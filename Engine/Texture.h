/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Texture class for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#include "GameEngine.h"

#ifndef TEXTURE_H_
#define TEXTURE_H_

namespace GameEngine {
	class Texture {
	public:
		Texture();
		~Texture() { Release(); }
		bool Load(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0,0,0));
		void Release();
		LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
		int getWidth() { return info.Width; }
		int getHeight() { return info.Height; }

	private:
		LPDIRECT3DTEXTURE9 texture;
		D3DXIMAGE_INFO info;
	};
}

#endif
