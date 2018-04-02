#include <iostream>
#include <cmath>
#include <fstream>

#include "rtvector.h"
#include "rtcamera.h"

using namespace std;

RTCamera::RTCamera(){
    lensCurvature = 0.6;
    lensSize = 3.0;
};

RTCamera::RTCamera(RTVector &loc, RTVector &dir, double lc, double ls) {
    setLocation(loc);
    setDirection(dir);
    lensCurvature = lc;
    lensSize = ls;
}

vector<RTVector> RTCamera::getLensBasis(){
    /*
     * Find basis vector for the plane perpendicular to the camera direction. The basis is used in traversing points on the lens when rendering the canvas.
     *
     * The basis is created by taking as vector b1 the projection of the (0, 0, 1) vector on the plane and normalizing the result. Basis vector b2 is then
     * obtained by setting b2_x = 1 and solving b1.b2 = 0 and b2.direction = 0.
     */

    vector<RTVector> output;
    RTVector b1(0, 0, 0);
    RTVector b2(0, 0, 0);

    // the degenerate case
    if(direction.getX() == 0.0 && direction.getY() == 0.0 && direction.getZ() != 0.0){
        b1.setXYZ(0, 1, 0);
        b2.setXYZ(1, 0, 0);
        output.push_back(b1);
        output.push_back(b2);
        return output;
    }

    // b1 = e_z - < e_z, direction > * direction, where e_z = (0, 0, 1) and direction is assumed to be normalized
    RTVector ez(0, 0, 1);
    double scalar = direction.dot(ez)/(direction.norm()*direction.norm());

    b1.setXYZ(ez.getX()-scalar*direction.getX(), ez.getY()-scalar*direction.getY(), ez.getZ()-scalar*direction.getZ());
    b1.normalize();

    if(direction.getY()*b1.getZ() - direction.getZ()*b1.getY() != 0.0){
        double D = 1.0/(direction.getY()*b1.getZ() - direction.getZ()*b1.getY());
        b2.setXYZ(1, D*(-b1.getZ()*direction.getX() + direction.getZ()*b1.getX()), D*(direction.getX()*b1.getY() - direction.getY()*b1.getX()));
    } else {
        // the degenerate case
        double D = 1.0/(direction.getX()*b1.getZ() - direction.getZ()*b1.getX());
        b2.setXYZ(0, 1, D*(direction.getX()*b1.getY() - direction.getY()*b1.getX()));
    }
    b2.normalize();
    output.push_back(b1);
    output.push_back(b2);
    return output;
}

