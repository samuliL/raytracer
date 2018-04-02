#ifndef __RTOBJECTS_H_INCLUDED__
#define __RTOBJECTS_H_INCLUDED__

#include "rtvector.h"
#include "rgbvalue.h"

class RTObject{
    protected:
        RTVector location;
        double reflectionRate; // value between 0.0 and 1.0, 0.0 - does not reflect light, 1.0 reflects everything
        RGBValue color;

    public:
        RTObject() {}
        RTObject(double x, double y, double z) {location.setXYZ(x, y, z);}
        virtual ~RTObject() {};

        virtual double checkCollision(RTVector &rayLoc, RTVector &rayDir) = 0;// {return -1.0;}
        virtual bool isReflective() = 0;
        virtual vector<RTVector> getReflection(RTVector &loc, RTVector &dir) = 0;
        virtual RGBValue getColor(RTVector &loc, RTVector &dir) {return RGBValue(0, 0, 0);};

};

class RTCube : public RTObject{
    // implementation of this class is incomplete and cannot be used
    private:
        double side;
        vector<RTVector> vertices;
        vector<double> sideConst; // each side of the cube is defined by an equation <normal, point> = sideconstant
        vector<RTVector> normals; // normal vectors of the sides
        // keep track where ray hits the cube when checking collision
        int sideRayHit;
        RTVector pointRayHit;

    public:
        static const int sides[6][4]; // array that associates vertices of the cube with the sides of the cube

        ~RTCube() {return;};
        RTCube(RTVector &location, double side_length);
        double getSideLength() {return side;}

        void initializeSides();
        double checkCollision(RTVector &rayLoc, RTVector &rayDir);
        bool insideTriangle(RTVector point, RTVector v1, RTVector v2, RTVector v3);

        bool isReflective() {return false;}
        RGBValue getColor(RTVector &loc, RTVector &dir);
        virtual vector<RTVector> getReflection(RTVector &loc, RTVector &dir) {return vector<RTVector>();}
};

class RTSphere : public RTObject{
    private:
        double radius;

    public:
        RTSphere(RTVector &location, double radius);
        ~RTSphere() {return;};
        double getRadius(){return radius;}

        double checkCollision(RTVector &rayLoc, RTVector &rayDir);
        vector<RTVector> getReflection(RTVector &loc, RTVector &dir);
        bool isReflective() {return true;}
};


class RTGround : public RTObject{
    double checkCollision(RTVector &rayLoc, RTVector &rayDir);
    vector<RTVector> getReflection(RTVector &loc, RTVector &dir) {vector<RTVector> r; return r;};
    bool isReflective() {return false;}
    RGBValue getColor(RTVector &loc, RTVector &dir);
};



#endif
