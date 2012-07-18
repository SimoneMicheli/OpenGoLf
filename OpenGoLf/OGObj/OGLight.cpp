//
//  OGLight.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 06/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGLight.h"

OGLight::OGLight(int num){
    number = num;
}

OGLight::OGLight(int num,double x, double y, double z, double t){
    number = num;
    position[0]=x;
    position[1]=y;
    position[2]=z;
    position[3]=t;
}

void OGLight::setPosition(double x, double y, double z, double t){
    position[0]=x;
    position[1]=y;
    position[2]=z;
    position[3]=t;
}

void OGLight::setDirection(double x, double y, double z){
    direction[0]=x;
    direction[1]=y;
    direction[2]=z;
}

void OGLight::set(){
    glLightfv(number, GL_POSITION, position);
    glLightfv(number,GL_SPOT_DIRECTION,direction);
}


void OGLight::enable(){
    glEnable(number);
    enabled = true;
}

void OGLight::disable(){
    glDisable(number);
    enabled = false;
}

bool OGLight::getStatus(){
    return enabled;
}

unsigned short int OGLight::getNumber(){
    return number;
}
