//
//  OGPov.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGPov.h"

OGPov::OGPov(){
    alpha = 0;
    beta = 0;
    mod = 1;
    pos = Vector3d();
    look = Vector3d();
    setRotation(0, 0);
}

OGPov::OGPov(const OGPov &pov){
    alpha = pov.alpha;
    beta = pov.beta;
    mod = pov.mod;
    pos = pov.pos;
    look = pov.look;
}

OGPov & OGPov::operator=(const OGPov& pov){
    if (this != &pov) {
        alpha = pov.alpha;
        beta = pov.beta;
        mod = pov.mod;
        pos = pov.pos;
        look = pov.look;
    }
    return *this;
}

OGPov::OGPov(Vector3d p){
    alpha = 0;
    beta = 0;
    mod = 1;
    look = Vector3d();
    pos = p;
    setRotation(0, 0);
}

OGPov::OGPov(double x, double y, double z){
    alpha = 0;
    beta = 0;
    mod = 1;
    look = Vector3d();
    pos = Vector3d(x,y,z);
    setRotation(0, 0);
}


void OGPov::setPosition(double x, double y, double z){
    pos = Vector3d(x,y,z);
}

void OGPov::setPosition(Vector3d p){
    pos = p;
}

Vector3d OGPov::getPosition(){
    return pos;
}

void OGPov::setLook(double x, double y, double z){
    look.z=x;
    look.y=y;
    look.z=z;
}

void OGPov::setLook(Vector3d v){
    look = v;
}

Vector3d OGPov::getLook(){
    return look;
}

void OGPov::setDirection(double x, double y, double z){
    setDirection(Vector3d(x,y,z));
}

void OGPov::setDirection(Vector3d direction){
    direction = direction.getNormalized() * mod;
    look = direction;
    alpha = look.alfa();
    beta = look.beta();
}

Vector3d OGPov::getDirection(){
    return look;
}

double OGPov::getAlpha(){
    return alpha;
}

double OGPov::getBeta(){
    return beta;
}

void OGPov::setRotation(double a,double b){
    a = a * M_PI / 180;
    b = (b + 90) * M_PI / 180;

    if (a > M_PI / 2 || a < -M_PI / 2){
        return;
    }
    alpha = a;
    beta = b;

    look[0] = (mod * cos(alpha) * cos(beta));
    look[1] = (mod * sin(alpha));
    look[2] = (mod * cos(alpha) * sin(beta));
}

void OGPov::addRotation(double a, double b){
    a = a * M_PI / 180;
    b = b * M_PI / 180;

    if (a + alpha < M_PI / 2 && a + alpha > -M_PI / 2){
        alpha += a;
    }
    beta += b;

    look[0] = (mod * cos(alpha) * cos(beta));
    look[1] = (mod * sin(alpha));
    look[2] = (mod * cos(alpha) * sin(beta));
}

//imposta la pov corrente
void OGPov::lookAt(){
    Vector3d pov = pos - look;
    gluLookAt(pov.x, pov.y, pov.z,
              pos.x, pos.y, pos.z,
              0.0f, 1.0f, 0.0f);
}
