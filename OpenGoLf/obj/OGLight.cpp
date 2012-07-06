//
//  OGLight.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 06/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGLight.h"

unsigned short int OGLight::next=0;

OGLight::OGLight(){
    number = next;
    next++;
}

OGLight::~OGLight(){
    next--;
}

void OGLight::enable(){
    glEnable(GL_LIGHT0);
}

void OGLight::disable(){
    glDisable(GL_LIGHT0);
}

void OGLight::set(){
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
}