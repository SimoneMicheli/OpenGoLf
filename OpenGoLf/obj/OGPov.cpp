//
//  OGPov.cpp
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGPov.h"

#ifdef WIN32

#include <GL/gl.h>
#include <GL/glu.h>

#include <GL/glut.h>

#else

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <GLUT/glut.h>
#endif

OGPov::OGPov(){
    alpha = 0;
    beta = 0;
    pos = Vector3d();
    setRotation(0, 0);
}

OGPov::OGPov(Vector3d p){
    pos = p;
    setRotation(0, 0);
}

OGPov::OGPov(double x, double y, double z){
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

void OGPov::setRotation(double a,double b){
    a = a * M_PI / 180;
    b = (b + 90) * M_PI / 180;
    
    if (a + alpha < M_PI / 2 && a + alpha > -M_PI / 2){
        alpha = a;
    }
    beta = b;
    
    look[0] = (mod * cos(alpha) * cos(beta));
    look[1] = (mod * sin(alpha));
    look[2] = (mod * cos(alpha) * sin(beta));
    
    look += pos;
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
    
    look += pos;
}

//imposta la pov corrente
void OGPov::lookAt(){    
    gluLookAt(pos.x, pos.y, pos.z, 
              look.x, look.y, look.z, 
              0.0f, 1.0f, 0.0f);
}