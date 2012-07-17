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
    initTexture();
}

OGBall::OGBall(const OGBall &ball){
    radius = 0.021;
    mass = 0.4555555;
    position = ball.position;
    speed = ball.speed;
    initTexture();
}

OGBall & OGBall::operator=(const OGBall &ball){
    if (this != &ball) {
        this->radius = ball.radius;
        this->mass = ball.mass;
        this->position = ball.position;
        this->speed = ball.speed;
    }
    return *this;
}

OGBall::OGBall(Vector3d pos){
    radius = 0.021;
    mass = 0.4555555;
    position = pos;
    speed =Vector3d();
    initTexture();
}

OGBall::OGBall(double x, double y, double z){
    radius = 0.021;
    mass = 0.4555555;
    position = Vector3d(x,y,z);
    speed =Vector3d();
    initTexture();
}

void OGBall::initTexture(){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TXball.width, TXball.height, 0, GL_RGB, GL_UNSIGNED_BYTE, TXball.pixel_data);
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
    //glDisable(GL_TEXTURE_2D);
    glPushMatrix();

    glTranslated(position.x,position.y + radius, position.z);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    
    //glEnable(GL_TEXTURE_2D);
    
    GLUquadricObj *sphere = gluNewQuadric();
    gluQuadricDrawStyle( sphere, GLU_FILL);
    gluQuadricNormals( sphere, GLU_SMOOTH);
    gluQuadricOrientation( sphere, GLU_OUTSIDE);
    gluQuadricTexture( sphere, GL_TRUE);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glColor3f(0.0, 1.0, 0.0);
    gluSphere( sphere, radius, 20, 20);
    
    
    //glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}
