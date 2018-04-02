#ifndef __RTVECTOR_H_INCLUDED__
#define __RTVECTOR_H_INCLUDED__

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;
const double PI = 3.14159265;

class RTVector {
    private:
        double x, y, z;
    public:
        double getX() const {return x;}
        double getY() const {return y;}
        double getZ() const {return z;}
        double getTheta() const {return acos(z/sqrt(x*x+y*y+z*z));}
        double getPhi() const;
        double dot(RTVector &v) const {return x*v.getX() + y*v.getY() + z*v.getZ();}
        double norm(){return sqrt(x*x+y*y+z*z);}

        RTVector operator+(const RTVector& v) const;
        RTVector operator-(const RTVector& v) const;
        RTVector operator*(double c) const;

        void normalize() {double r = sqrt(x*x+y*y+z*z); x = x/r; y = y/r; z = z/r;}
        void scale(double s) {x = x*s; y = y*s; z = z*s;}
        void set(RTVector &v) {setXYZ(v.getX(), v.getY(), v.getZ());}
        void setXYZ(double x_, double y_, double z_) {x = x_; y = y_; z = z_;}

        RTVector() {}
        RTVector(double x_, double y_, double z_) {setXYZ(x_, y_, z_);}

        RTVector getCopy() const;

        void rotateBy(double dphi, double dtheta_);

        void print() {cout << "x: " << x << " y: " << y << " z: " << z  << " phi: " << acos(x/sqrt(x*x+y*y)) << " theta: " << acos(z/sqrt(x*x + y*y + z*z)) << endl;};
};
#endif
