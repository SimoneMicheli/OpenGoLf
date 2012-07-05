//
//  OGBall.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGBall_h
#define OpenGoLf_OGBall_h

#include "Vector3d.h"

class OGBall{
private:
    double radius,mass;
    Vector3d position, speed;
    
public:
    void setPosition(double,double,double);
    void setPosition(Vector3d);
    void setSpeed(Vector3d);
    double getMass();
    double getRadius();
    Vector3d getPosition();
    Vector3d getSpeed();
    void draw();
}

#endif
