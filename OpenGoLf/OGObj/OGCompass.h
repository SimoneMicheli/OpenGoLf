//
//  OGCompass.h
//  OpenGoLf
//
//  Created by Simone Micheli on 15/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "OGObject.h"
#include <sstream>

#ifndef OpenGoLf_OGCompass_h
#define OpenGoLf_OGCompass_h

class OGCompass : public OGObject{
public:
    
    OGCompass(int x, int y, int width, int height);
    void draw(float angle, float speed = -1);
    void setPosition(int, int);
    
private:
    int width, height, x,y;
    GLuint circleDL, arrowDL, directionDL;
    GLboolean depth, light, texture;
    
    GLuint createCircleDL(float r, int num_segments);
    GLuint createArrowDL();
    GLuint createDirectionDL();
};

#endif
