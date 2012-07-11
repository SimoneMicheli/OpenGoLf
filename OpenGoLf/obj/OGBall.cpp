//
//  OGBall.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGBall.h"

OGBall::OGBall(){
    radius = 0.021;
    mass = 0.4555555;
    position = Vector3d();
    speed =Vector3d();
}

OGBall::OGBall(Vector3d pos){
    radius = 0.021;
    mass = 0.4555555;
    position = pos;
    speed =Vector3d();
}

OGBall::OGBall(double x, double y, double z){
    radius = 0.021;
    mass = 0.4555555;
    position = Vector3d(x,y,z);
    speed =Vector3d();
}

void OGBall::setPosition(double x, double y, double z){
    position = Vector3d(x,y,z);
}

void OGBall::setPosition(Vector3d v){
    position = v;
}

void OGBall::setSpeed(double x, double y, double z){
    speed = Vector3d(x,y,z);
}

void OGBall::setSpeed(Vector3d s){
    speed = s;
}

double OGBall::getMass(){
    return mass;
}

double OGBall::getRadius(){
    return radius;
} 

Vector3d OGBall::getPosition(){
    return position;
}

Vector3d OGBall::getSpeed(){
    return speed;
}

void OGBall::draw(){
    glPushMatrix();
    glColor3b(0.333, 0.0, 0.0);
    glTranslated(position.x,position.y, position.z);
    glutSolidSphere(radius, 15, 15);
    glPopMatrix();
}
