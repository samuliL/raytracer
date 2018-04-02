#ifndef __RTCANVAS_H_INCLUDED__
#define __RTCANVAS_H_INCLUDED__

#include "rgbvalue.h"

class RTCanvas{
    private:
        int xSize, ySize;
        RGBValue *colors;

    public:
        int getXSize() {return xSize;}
        RTCanvas(const int xS, const int yS);
        ~RTCanvas();
        RGBValue *getCol(int x, int y);

        void setCol(int x, int y, RGBValue c);
        void saveCanvas(const char *filename);
};

#endif
