//
//  OGPhysic.h
//  OpenGoLf
//
//  Created by Simone Micheli on 11/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "OGObject.h"
#include "Vector3d.h"
#include <math.h>
#include "OGBall.h"
#include "OGTerrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef OpenGoLf_OGPhysic_h
#define OpenGoLf_OGPhysic_h

class OGPhysic : public OGObject{
public:
    OGPhysic(OGBall*, OGTerrain*);
    void update(double time);
    void shoot(float power, Vector3d direction);
    
private:
    float viscosity, friction, gravity, elasticity;
    Vector3d wind;
    OGBall *ball;
    OGTerrain *terrain;
    bool oldCollision;
    
    bool terrainCollision(Vector3d &normal);
    bool objCollision();
};

#endif
