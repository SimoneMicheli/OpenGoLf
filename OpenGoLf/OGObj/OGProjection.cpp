//
//  OGProjection.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 09/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGProjection.h"
#include <stdio.h>

void OGProjection::setPerspective(float angle, float aspect, float p_near, float p_far){
    this->angle = angle;
    this->aspect = aspect;
    this->p_near = p_near;
    this->p_far = p_far;
    this->type = PERSPECTIVE;
    init();
}

void OGProjection::setOrtho(float p_near,float p_far ,float left ,float right, float top, float bottom){
    this->p_near = p_near;
    this->p_far = p_far;
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
    printf("%f\n",p_far);
    if (type == PERSPECTIVE) {
        gluPerspective(angle, aspect, p_near, p_far);
    }else{
        glOrtho(left, right, bottom, top,p_near, p_far);
    }
    glMatrixMode(GL_MODELVIEW);
}
