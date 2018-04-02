#ifndef __RTSCENE_H_INCLUDED__
#define __RTSCENE_H_INCLUDED__

#include "rtcamera.h"
#include "rtcanvas.h"
#include "rtobjects.h"

class RTScene{
    private:
        vector<RTObject*> object_list;
        //vector<RTLightSource> lights_list;

        RTCamera camera;

    public:
        RTScene();
        ~RTScene();

        void setCamera(RTVector &loc, RTVector &dir);

        void addObject(RTObject *rto);

        // implementations of two different rendering systems
        RTCanvas renderSpherical(int xSize, int ySize);
        RTCanvas renderPlanar(int xSize, int ySize);

        // function pointer to the actual rendering function
        RTCanvas (RTScene::*renderFunction)(int, int);
        RTCanvas render(int xSize, int ySize){ return (*this.*renderFunction)(xSize, ySize); }

        RGBValue shootRay(RTVector &loc, RTVector &dir);
};

#endif
