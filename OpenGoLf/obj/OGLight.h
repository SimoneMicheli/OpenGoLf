//
//  OGLight.h
//  OpenGoLf
//
//  Created by Simone Micheli on 06/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGLight_h
#define OpenGoLf_OGLight_h

#include "OGObject.h"

enum OGLightType{DIRECTIONAL, SPOT};

class OGLight: public OGObject{
    
public:
    float ambient[3],diffuse[3],specular[3],position[4]={10.0f,10.0f,10.0f,0.0f},direction[3];
    OGLightType type;
    
    OGLight();
    ~OGLight();
    void set();
    void enable();
    void disable();
    bool getStatus();
    int getNumber();
private:
    unsigned short int number;
    bool enabled;
    static unsigned short int next; //first available light number
};

#endif
