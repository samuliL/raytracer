#include <iostream>
#include <cmath>
#include <fstream>
#include "rtvector.h"

using namespace std;

RTVector RTVector::getCopy() const  {
    RTVector c(x, y, z);
    return c;
}

double RTVector::getPhi() const {
    double phi;
    if(x == 0.0 && y == 0.0)
        phi = 0.0;
    else
        phi = acos(x/sqrt(x*x+y*y));

    if(y < 0.0)
        phi = 2*PI - phi;
    return phi;
}

void RTVector::rotateBy(double dphi, double dtheta){
    // rotates the vector in spherical coordinates

    double theta, phi;
    double r = sqrt(x*x+y*y+z*z);

    if(r == 0.0)
        return;

    theta = acos(z/r);

    if(x == 0.0 && y == 0.0)
        phi = 0.0;
    else
        phi = acos(x/sqrt(x*x+y*y));

    if(y < 0.0)
        phi = 2*PI - phi;

    theta += dtheta; phi += dphi;

    if(theta < 0.0) phi += PI;

    if(theta >= PI) theta = 2*PI - theta;
    if(theta < 0.0) theta *= -1;

    while(phi >= 2*PI) phi -= 2*PI;
    while(phi < 0.0) phi += 2*PI;

    x = r*sin(theta)*cos(phi);
    y = r*sin(theta)*sin(phi);
    z = r*cos(theta);
}

RTVector RTVector::operator+(const RTVector& v) const{
    return RTVector(getX() + v.getX(), getY() + v.getY(), getZ() + v.getZ());
}

RTVector RTVector::operator-(const RTVector& v) const{
    return RTVector(getX() - v.getX(), getY() - v.getY(), getZ() - v.getZ());
}

RTVector RTVector::operator*(double c) const{
    return RTVector(getX()*c, getY()*c, getZ()*c);
}

