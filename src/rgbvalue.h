#ifndef __RGBVALUE_H_INCLUDED__
#define __RGBVALUE_H_INCLUDED__

using namespace std;

class RGBValue {
    private:
        unsigned char red, green, blue;

    public:
        unsigned char getRed() {return red;}
        unsigned char getGreen() {return green;}
        unsigned char getBlue() {return blue;}

        RGBValue(unsigned char r, unsigned char g, unsigned char b) { setRGB(r, g, b); }
        RGBValue() {setRGB(0, 0, 0);}

        void setRGB(unsigned char r, unsigned char g, unsigned char b) { red = r; green = g; blue = b;}

};

#endif
