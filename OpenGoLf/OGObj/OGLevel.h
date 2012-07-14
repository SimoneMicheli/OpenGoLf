//
//  OGLevel.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGLevel_h
#define OpenGoLf_OGLevel_h

#include "OGAmbient.h"
#include <string.h>
#include <stdlib.h>
#include "OGLight.h"
#include "OGBall.h"
#include "OGProjection.h"
#include "OGPhysic.h"
#include "OGObject.h"
#include "OGLevel.h"
#include <sys/time.h>
#include <vector>
#include <pthread.h>
#include <stream.h>

class OGLevel : public OGAmbient {
    
public:
    
    OGLevel();
    ~OGLevel();
    void init(string);

    void static mousePassiveMotionFunction(int, int);
    void static launchDisplay();
    void static followDisplay();
    void static mouseClickFunction(int,int,int,int);
    
private:
    static OGLevel* activeLevel;
    OGTerrain *terrain;
    OGBall *ball, *oldBall;
    OGProjection *projection;
    OGPhysic *physic;
    vector<OGLight*> lights;
    static struct timeval before,now,launchTime;
    double static time_diff(timeval before, timeval now);
    
    Vector3d oldMousePos;
    
    void drawMap();
};


#endif
