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

namespace GameEngine {
	Font::Font() : GameObject() {
        // Set character widths to default
        memset(&widths, 0, sizeof(widths));
    }
    
    void Font::Print(int x, int y, std::string text, int color) { 
        float fx = (float)x;
        float fy = (float)y;

        // Set font color
        this->setColor( color );

        // Draw each character of the string
        for (unsigned int n=0; n<text.length(); n++)
        {
            int frame = (int)text[n];
            this->setCurrentFrame( frame );
            this->setX( fx );
            this->setY( fy );
            this->draw();

            // Use proportional width if available
            if (widths[frame] == 0) widths[frame] = this->width;
            fx += (float)(widths[frame] * this->scaling);
        }
    }

    bool Font::loadWidthData(std::string filename) {
        unsigned char buffer[512];
    
        // Open font width data file
        std::ifstream infile;
        infile.open(filename.c_str(), std::ios::binary);
        if (!infile) return false;
    
        // Read 512 bytes (2 bytes per character)
        infile.read( (char *)(&buffer), 512 );
        if (infile.bad()) return false;
        infile.close();        
    
        // Convert raw data to proportional width data
        for (int n=0; n<256; n++) {
            widths[n] = (int)buffer[n*2];
        }
    
        return true;
    }
}
