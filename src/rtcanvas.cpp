/*
 * classes.cpp - contains the implementations of RTCamera and RTCanvas
 */

#include <iostream>
#include <cmath>
#include <fstream>

#include "rtcanvas.h"

using namespace std;

RTCanvas::RTCanvas(const int xS, const int yS){
    xSize = xS; ySize = yS;

    colors = new RGBValue[xS*yS];

    for(int i = 0; i < xS*yS; i++)
        colors[i].setRGB(0, 0, 0);
}

RTCanvas::~RTCanvas(){
    delete[] colors;
}

void RTCanvas::setCol(int x, int y, RGBValue c){
    if(x < 0 || x > xSize){
        x = 0;
        cout << "Attempting to access negative/too large values on canvas. Defaulting to 0." << endl;
    }
    if(y < 0 || y > ySize){
        y = 0;
        cout << "Attempting to access negative/too large values on canvas. Defaulting to 0." << endl;
    }

    colors[y*xSize + x].setRGB(c.getRed(), c.getGreen(), c.getBlue());
}

RGBValue* RTCanvas::getCol(int x, int y){
    if(x < 0 || x > xSize){
        x = 0;
        cout << "Attempting to access negative/too large values on canvas. Defaulting to 0." << endl;
    }
    if(y < 0 || y > ySize){
        y = 0;
        cout << "Attempting to access negative/too large values on canvas. Defaulting to 0." << endl;
    }
    return &colors[y*xSize + x];
}

void RTCanvas::saveCanvas(const char *filename){
    ofstream file;
    file.open(filename, ios::out | ios::binary); // open the file in binary output mode

    file << "P6 " << xSize <<  " " << ySize << " " << 255 << " ";
    for(int j = 0; j < ySize; j++){
        for(int i = 0; i < xSize; i++){
            RGBValue *c = getCol(i, j);
            file << c->getRed() << c->getGreen() << c->getBlue();
        }
    }
    file.close();
}











