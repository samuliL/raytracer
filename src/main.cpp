#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "rgbvalue.h"
#include "rtvector.h"
#include "rtcamera.h"
#include "rtscene.h"


using namespace std;

void testScene();
void randomScene();
void pyramid();

int main()
{
    pyramid();
    return 0;
}

void randomScene(){
    srand(time(NULL));
    int numObjects = 100;

    RTScene scene;
    RTVector sloc(0, 0, 0);

    for(int i = 0; i < numObjects; i++){
        sloc.setXYZ(rand() % 50 - 20, rand() % 50 - 20, rand() % 50);
        RTObject *r = new RTSphere(sloc, 1);
        scene.addObject(r);
    }

    RTVector rtl(-5.0, 0.0, 5);
    RTVector rtd(1.0, 0.0, -.1);

    scene.setCamera(rtl, rtd);

    RTCanvas r = scene.render(800, 800);
    r.saveCanvas("testcanvas.ppm");
}

void testScene(){

    RTScene scene;

    RTVector rtv(0, 0, 3);
    RTObject *rO1 = new RTSphere(rtv, 1.0);
    scene.addObject(rO1);

    rtv.setXYZ(0, 0, 1);
    RTObject *r02 = new RTCube(rtv, 2.0);
    scene.addObject(r02);

    RTVector rtl(3, 3.0, 3);
    RTVector rtd(-1.0, -1.0, -.50);

    scene.setCamera(rtl, rtd);

    RTCanvas r = scene.render(800, 800);
    r.saveCanvas("testcanvas.ppm");
}

void pyramid(){
    RTScene scene;

    RTVector rtv(0, 0, 0);
    for(int j = 0; j < 6; j++){
        for(int i = -j; i <= j; i++){
            rtv.setXYZ(i, j, 6-j);
            RTObject *rO1 = new RTCube(rtv, 1.0);
            scene.addObject(rO1);

            rtv.setXYZ(i, -j, 6-j);
            RTObject *rO2 = new RTCube(rtv, 1.0);
            scene.addObject(rO2);
        }

        for(int i = -j+1; i <= j-1; i++){
            rtv.setXYZ(j, i, 6-j);
            RTObject *rO1 = new RTCube(rtv, 1.0);
            scene.addObject(rO1);

            rtv.setXYZ(-j, i, 6-j);
            RTObject *rO2 = new RTCube(rtv, 1.0);
            scene.addObject(rO2);
        }
    }

    rtv.setXYZ(0, 0, 7);
    RTObject *o = new RTSphere(rtv, 0.5);
    scene.addObject(o);

    RTVector rtl(7, 6, 7);
    RTVector rtd(-1.0, -.9, -.25);

    scene.setCamera(rtl, rtd);

    RTCanvas r = scene.render(800, 800);
    r.saveCanvas("testcanvas.ppm");

}
