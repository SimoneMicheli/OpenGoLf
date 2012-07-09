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

class OGLight: public OGObject{
    
public:
    float ambient[3],diffuse[3],specular[3],position[4]={1.0f,-1.0,1.0f,0.0f},direction[3];
    
    OGLight(int);
    ~OGLight();
    void set();
    void enable();
    void disable();
    bool getStatus();
    int getNumber();
private:
    unsigned short int number;
    bool enabled;
};

#endif
