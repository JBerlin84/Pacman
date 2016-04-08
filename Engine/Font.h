/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-11-15
Project: Game Engine for Game programming for windows II

desc:
Font class for game engine
Note:
Based on the information given in "Advanced 2D game deveolement"
Greatly simplified and personalized to be practical in this course.
************************************************************************/

#include "GameEngine.h"

#ifndef FONT_H_
#define FONT_H_

namespace GameEngine {
	class Font : public GameObject {
    private:
        int widths[256];

    public:
        Font();
        virtual ~Font(void) { }

        void Print(int x, int y, std::string text, int color = 0xFFFFFFFF );

        int getCharWidth() { return this->width; }
        int getCharHeight() { return this->height; }
        void setCharWidth(int width) { this->width = width;    }
        void setCharSize(int width, int height) {
            setCharWidth( width );
            this->height = height;
        }

        bool loadWidthData(std::string filename);
    };
}

#endif