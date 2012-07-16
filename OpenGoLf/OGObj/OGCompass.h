//
//  OGCompass.h
//  OpenGoLf
//
//  Created by Simone Micheli on 15/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "OGObject.h"

#ifndef OpenGoLf_OGCompass_h
#define OpenGoLf_OGCompass_h

class OGCompass : public OGObject{
public:
    
    OGCompass(int x, int y, int width, int height);
    void drawWind(Vector3d v);
    void drawMap(float angle);
    
private:
    int width, height, x,y;
    GLuint circleDL, arrowDL;
    GLboolean depth, light, texture;
    
    GLuint createCircleDL(float r, int num_segments);
    GLuint createArrowDL();
};

#endif
