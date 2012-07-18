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
#include "OGPov.h"
//#include "OGGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef OpenGoLf_OGPhysic_h
#define OpenGoLf_OGPhysic_h

class OGPhysic : public OGObject{
public:
    OGPhysic(OGBall*, OGTerrain*, OGPov*);
    void update(double time);
    void shoot(float power, int angle);
    bool edgeCollision();
    bool holeCollision();
    Vector3d getWind();
    
private:
    float viscosity, friction, gravity, elasticity;
    Vector3d wind;
    OGBall *ball;
    OGTerrain *terrain;
    OGPov *pov;
    //bool oldCollision;
    
    bool terrainCollision(Vector3d &vertex, Vector3d &normal);
    bool terrainEdge();
    bool waterCollision();
    bool objCollision();
};

#endif
