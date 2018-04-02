#include <vector>
#include <iostream>

#include "rtscene.h"

using namespace std;

RTScene::RTScene(){
    renderFunction = &RTScene::renderPlanar;
    object_list.push_back(new RTGround());
}

RTScene::~RTScene(){
    for(vector<RTObject*>::iterator it = object_list.begin(); it != object_list.end(); ++it)
        delete (*it);

    object_list.clear();
}

void RTScene::addObject(RTObject* rto){
    object_list.push_back(rto);
}

void RTScene::setCamera(RTVector &loc, RTVector &dir){
    camera.setLocation(loc);
    camera.setDirection(dir);
}

RTCanvas RTScene::renderPlanar(int xSize, int ySize){
    /*
     *  Renders the scene and returns an RTCanvas of size xSize, ySize. Places a plane with normal vector camera.direction to camera.location and shoots rays from a rectangle laying on the plane.
     *  Prefer this method over the function renderSpherical, which behaves strangely on certain situations.
     */
    vector<RTVector> basis = camera.getLensBasis();

    RTCanvas canv(xSize, ySize);

    // the "physical" size of the lens
    double hSize = camera.getLensSize();
    double vSize = camera.getLensSize();

    // make sure that there is no unwanted distortion in the lens
    if(xSize > ySize)
        vSize = vSize/xSize*ySize;
    if(ySize > xSize)
        hSize = hSize/ySize*xSize;

    double hStep = hSize/xSize;
    double vStep = vSize/ySize;

    RTVector loop_loc = camera.getLocation();
    RTVector loop_dir = camera.getDirection();

    // basis vectors of the lens "plane"
    RTVector db1 = basis.at(0)*vStep;
    RTVector db2 = basis.at(1)*hStep;

    // initialize the loop_loc vector to the corner of the lens

    loop_loc = loop_loc + basis.at(0)*(vSize/2);
    loop_loc = loop_loc - basis.at(1)*(hSize/2);

    // calculate the longest distance on the lens to the center of the lens
    double maxDist = (loop_loc - camera.getLocation()).norm();

    // the first basis vector is needed in the loop for re-positioning the loop_loc vector
    basis.at(0) = basis.at(0)*vSize;

    for(int x = 0; x < xSize; x++){
        for(int y = 0; y < ySize; y++){
            // calculate the direction of the ray from the weighted difference of the camera location and the current location on the lens
            RTVector diff = loop_loc - camera.getLocation();
            double dist = diff.norm();
            diff.normalize();

            // point the next ray to a direction determined by the location on the lens and camera direction
            loop_dir = diff*(dist/maxDist*camera.getLensCurvature()) + camera.getDirection()*(1-camera.getLensCurvature());
            loop_dir.normalize();

            RGBValue r = shootRay(loop_loc, loop_dir);
            canv.setCol(x, y, r);
            loop_loc = loop_loc - db1;
        }
        loop_loc = loop_loc + basis.at(0);
        loop_loc = loop_loc + db2;
    }

    return canv;

}

RTCanvas RTScene::renderSpherical(int xSize, int ySize) {
    /*
     *  Renders the scene and returns an RTCanvas of size xSize, ySize. Uses spherical coordinates and behaves strangely when camera direction is closely aligned with the z-axis.
     *  This function was made for test purposes only and should not be used.
     */

    double LVert = 1.0;
    double LHoriz = 1.0;


    RTCanvas canv(xSize, ySize);

    RTVector loop_direction = camera.getDirection();
    RTVector loop_position = camera.getLocation();

    double dphi = LHoriz/xSize;
    double dtheta = LVert/ySize;

    // take into account if theta angle goes over the 0 angle, which is important in the loop
    if(loop_direction.getTheta()-LVert/2 < 0)
        dtheta *= -1;

    loop_direction.rotateBy(-LHoriz/2, -LVert/2);

    // loop through the points in the canvas (lens)
    for(int y = 0; y < ySize; y++){
        for(int x = 0; x < xSize; x++){

            // shoot a ray from the lens pixel given by x, y
            RGBValue r = shootRay(loop_position, loop_direction);
            canv.setCol(x, y, r);

            loop_direction.rotateBy(dphi, 0);
        }

        if(loop_direction.getTheta() + dtheta < 0)
            dtheta *= -1;
        if(loop_direction.getTheta() + dtheta > PI)
            dtheta *= -1;

        loop_direction.rotateBy(0, dtheta);

        loop_direction.rotateBy(-LHoriz, 0);
    }
    return canv;
}

RGBValue RTScene::shootRay(RTVector &loc, RTVector &dir){
    /*
     * Shoots a ray from loc towards dir. The ray is checked for collision with the objects in the scene and if collision(s) happen, a corresponding color is returned.
     */

    // vector refl contains the location and direction of the ray. This form is compatible with RTObject::getReflection(...)
    vector<RTVector> refl;
    refl.push_back(loc);
    refl.push_back(dir);

    // variable to determine the closest object
    double minRange = 100000.0;
    // counts the number of reflections
    int numRef = 0;

    // loop until ray does not hit an object
    while(true){
        int idx = -1, i = 0;

        for(vector<RTObject*>::iterator it = object_list.begin(); it != object_list.end(); ++it){
            double range = (*it)->checkCollision(refl.at(0), refl.at(1));
            if(range > 0.0 && range < minRange){
                minRange = range;
                idx = i;
            }
            i++;
        }

        if(numRef > 100){ return RGBValue(0, 0, 0);}

        numRef ++;

        if(idx == -1){
            // if ray does not hit any objects

            double th = refl.at(1).getTheta();
            if(th < PI/2){
                // if the ray is above horizon return a shade of blue depending on the angle of the ray
                unsigned char c = (PI/2-th)/(PI/2)*255;
                return RGBValue(255-c, 255-c, 255);
            } else {
                // if the ray is below horizon - this part of the code is not used anymore that RTGround object is implemented
                // to be removed
                return RGBValue(255, 0, 0);
            }

        } else {
            // if ray reflects from an object
            RTVector l1 = refl.at(0).getCopy();
            RTVector d1 = refl.at(1).getCopy();
            refl.clear();

            if (object_list.at(idx)->isReflective())
                refl = object_list.at(idx)->getReflection(l1, d1);
            else return object_list.at(idx)->getColor(l1, d1);

            minRange = 100000;
        }
    }
}
