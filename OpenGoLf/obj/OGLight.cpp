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

void OGLight::enable(){
    glEnable(number);
}

void OGLight::disable(){
    glDisable(number);
}

void OGLight::set(){
    glLightfv(number, GL_POSITION, position);
    glLightfv(number, GL_SPOT_DIRECTION, direction);
}