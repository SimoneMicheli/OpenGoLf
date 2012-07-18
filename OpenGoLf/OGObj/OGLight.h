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
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float position[4]; //{1.0f,-1.0,1.0f,0.0f};
    float direction[3];

    OGLight(int);
    OGLight(int,double,double,double,double);
    void setPosition(double,double,double,double);
    void setDirection(double,double,double);
    void enable();
    void disable();
    void set();
    bool getStatus();
    unsigned short int getNumber();
private:
    unsigned short int number;
    bool enabled;
};

#endif
