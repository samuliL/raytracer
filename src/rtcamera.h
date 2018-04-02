#ifndef __RTCAMERA_H_INCLUDED__
#define __RTCAMERA_H_INCLUDED__

#include "rtvector.h"

class RTCamera{
    private:
        RTVector location;
        RTVector direction;
        double lensCurvature; // value between 0.0 and 1.0, 0.0 no curvature, 1.0 maximal curvature
        double lensSize;

    public:
        RTCamera();
        RTCamera(RTVector &loc, RTVector &dir, double lc, double ls);

        void setLensCurvature(double d) {lensCurvature = d;}
        double getLensCurvature() {return lensCurvature;}

        void setLensSize(double ls) {lensSize = ls;}
        double getLensSize() {return lensSize;}

        void setLocation(RTVector &loc) {location.set(loc);}
        void setDirection(RTVector &dir) {dir.normalize(); direction.set(dir);}
        RTVector getLocation() {return location.getCopy();}
        RTVector getDirection() {return direction.getCopy();}

        // returns orthogonal basis vectors for the plane perpendicular to the camera direction
        vector<RTVector> getLensBasis();
};

#endif
