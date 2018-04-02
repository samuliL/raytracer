#include <vector>

#include "rtobjects.h"

using namespace std;

const int RTCube::sides[6][4] = {{0, 1, 2, 4}, {0, 1, 3, 5}, {0, 2, 3, 6}, {3, 5, 6, 7}, {2, 4, 6, 7}, {1, 4, 5, 7}};

double RTGround::checkCollision(RTVector &rayLoc, RTVector &rayDir){
    // ground is just a plane satisfying the equation z = 0
    if(rayDir.getZ() > 0) return -1.0;
    return -rayLoc.getZ()/rayDir.getZ();
}

RGBValue RTGround::getColor(RTVector &loc, RTVector &dir){

    double t =-loc.getZ()/dir.getZ();
    RTVector p = loc + dir*t;

    int r =((int)(sqrt(p.getX()*p.getX() + p.getY()*p.getY()))*10)%100;
    return RGBValue(0, 255-r, 0);
}

RTCube::RTCube(RTVector &loc, double side_length){
    side = side_length;
    location.setXYZ(loc.getX(), loc.getY(), loc.getZ());

    RTVector v(loc.getX()+side/2, loc.getY()+side/2, loc.getZ()+side/2); // 0 + + +
    vertices.push_back(v);

    v.setXYZ(loc.getX()-side/2, loc.getY()+side/2, loc.getZ()+side/2); // 1 - + +
    vertices.push_back(v);

    v.setXYZ(loc.getX()+side/2, loc.getY()-side/2, loc.getZ()+side/2); // 2 + - +
    vertices.push_back(v);

    v.setXYZ(loc.getX()+side/2, loc.getY()+side/2, loc.getZ()-side/2); // 3 + + -
    vertices.push_back(v);

    v.setXYZ(loc.getX()-side/2, loc.getY()-side/2, loc.getZ()+side/2); // 4 - - +
    vertices.push_back(v);

    v.setXYZ(loc.getX()-side/2, loc.getY()+side/2, loc.getZ()-side/2); // 5 - + -
    vertices.push_back(v);

    v.setXYZ(loc.getX()+side/2, loc.getY()-side/2, loc.getZ()-side/2); // 6 + - -
    vertices.push_back(v);

    v.setXYZ(loc.getX()-side/2, loc.getY()-side/2, loc.getZ()-side/2); // 7 - - -
    vertices.push_back(v);

    initializeSides();
}

void RTCube::initializeSides(){
    for(int i = 0; i < 6; i++){
        // in the case of cube it is easy to calculate the normal vector corresponding to a side
        RTVector p = vertices.at(sides[i][0])+vertices.at(sides[i][1])+vertices.at(sides[i][2])+vertices.at(sides[i][3]);
        p = p*0.25;
        RTVector n = p - location;
        n.normalize();
        normals.push_back(n);
        sideConst.push_back(n.dot(p));
    }
}

double RTCube::checkCollision(RTVector &rayLoc, RTVector &rayDir){

    double minDist = 100000;
    bool contact = false;

    for(int i = 0; i < 6; i++){
        double den = normals.at(i).dot(rayDir);
        if(den != 0.0){
            double t = (sideConst.at(i) - normals.at(i).dot(rayLoc))/den;
            if(t > 0){
                RTVector contactPoint = rayLoc + rayDir*t;
                // check if contactPoint lies in the convex combination of the vertices
                if(insideTriangle(contactPoint, vertices.at(sides[i][0]), vertices.at(sides[i][1]), vertices.at(sides[i][2]))){
                  // || insideTriangle(contactPoint, vertices.at(sides[i][0]), vertices.at(sides[i][1]), vertices.at(sides[i][3]))){
                    if(minDist > t){
                        minDist = t;
                        contact = true;
                        sideRayHit = i;
                        pointRayHit = contactPoint.getCopy();
                    }
                }
            }
        }
    }
    if(contact){
        return minDist;
    }

    return -1.0;
}

RGBValue RTCube::getColor(RTVector &loc, RTVector &dir){
    RTVector light(1, -5, -10);
    light.normalize();
    double dprod = (1-light.dot(normals.at(sideRayHit)))/2;

    RTVector r = location + normals.at(sideRayHit)*(side/2);
    double dist = (pointRayHit - r).norm()/(side*1.41/2);
    dist = 0.1*(int)(dist*10);
    int c = dist*255*dprod;

    return RGBValue(c, c, 0);
}


bool RTCube::insideTriangle(RTVector point, RTVector v1, RTVector v2, RTVector v3){
    RTVector w1 = v1-point;
    RTVector w2 = v1-v2;
    RTVector w3 = v1-v3;
    double d1 = w1.dot(w2);
    double d2 = w1.dot(w3);
    if(d1 >= 0 && sqrt(d1) <= w2.norm() && d2 >= 0 && sqrt(d2) <= w3.norm() ){
        return true;
    }
    return false;
}

RTSphere::RTSphere(RTVector &loc, double r){
    radius = r;
    location.setXYZ(loc.getX(), loc.getY(), loc.getZ());
}

double RTSphere::checkCollision(RTVector &rayLoc, RTVector &rayDir){

    double x0 = location.getX(), y0 = location.getY(), z0 = location.getZ();
    double x1 = rayLoc.getX(), y1 = rayLoc.getY(), z1 = rayLoc.getZ();

    double x2 = x1 + rayDir.getX(), y2 = y1 + rayDir.getY(), z2 = z1 + rayDir.getZ();

    double t = -((x1-x0)*(x2-x1) + (y1-y0)*(y2-y1) + (z1-z0)*(z2-z1)) / ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

    double d = ((x1-x0) + (x2-x1)*t)*((x1-x0) + (x2-x1)*t) + ((y1-y0) + (y2-y1)*t)*((y1-y0) + (y2-y1)*t) + ((z1-z0) + (z2-z1)*t)*((z1-z0) + (z2-z1)*t);

    double t_ref = t - sqrt(radius*radius-d);

    if(abs(t_ref) < 0.00001) return -1.0;

    d = sqrt(d);

    if (d <= radius)
        return t_ref;


    return -1.0;
}

vector<RTVector> RTSphere::getReflection(RTVector &loc, RTVector &dir){
    vector<RTVector> r;

    dir.normalize();

    double x0 = location.getX(), y0 = location.getY(), z0 = location.getZ();
    double x1 = loc.getX(), y1 = loc.getY(), z1 = loc.getZ();
    double x2 = x1 + dir.getX(), y2 = y1 + dir.getY(), z2 = z1 + dir.getZ();

    double t = -((x1-x0)*(x2-x1) + (y1-y0)*(y2-y1) + (z1-z0)*(z2-z1)) / ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

    double d = ((x1-x0) + (x2-x1)*t)*((x1-x0) + (x2-x1)*t) + ((y1-y0) + (y2-y1)*t)*((y1-y0) + (y2-y1)*t) + ((z1-z0) + (z2-z1)*t)*((z1-z0) + (z2-z1)*t);

    double t_ref = t - sqrt(radius*radius-d);

    RTVector r_loc(x1 + t_ref*dir.getX(), y1 + t_ref*dir.getY(), z1 + t_ref*dir.getZ());

    RTVector r_dir(0, 0, 0);

    RTVector v_normal(x0-r_loc.getX(), y0-r_loc.getY(), z0-r_loc.getZ());

    v_normal.normalize();

    v_normal.scale(v_normal.getX()*dir.getX() + v_normal.getY()*dir.getY() + v_normal.getZ()*dir.getZ());
    r_dir.setXYZ(dir.getX() - 2*v_normal.getX(), dir.getY() - 2*v_normal.getY(), dir.getZ() - 2*v_normal.getZ());

    r_dir.normalize();

    r.push_back(r_loc);
    r.push_back(r_dir);

    return r;
}



