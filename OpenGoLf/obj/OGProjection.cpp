//
//  OGProjection.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 09/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGProjection.h"

void OGProjection::setPerspective(float angle, float aspect, float near, float far){
    this->angle = angle;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
    this->type = PERSPECTIVE;
    init();
}

void OGProjection::setOrtho(float near,float far ,float left ,float right, float top, float bottom){
    this->near = near;
    this->far = far;
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->type = ORTHO;
    init();
}

void OGProjection::init(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (type == PERSPECTIVE) {
        gluPerspective(angle, aspect, near, far);
    }else{
        glOrtho(left, right, bottom, top,near, far);
    }
    glMatrixMode(GL_MODELVIEW);    
}