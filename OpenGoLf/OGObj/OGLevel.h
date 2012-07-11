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
#include "OGGlobal.h"
#include "OGBall.h"
#include "OGProjection.h"
#include "OGPhysic.h"
#include <sys/time.h>
#include <vector>

class OGLevel : public OGAmbient {
    
public:
    
    OGLevel();
    void init(string);

    void mousePassiveMotionFunction(int, int);
    void static wrapperMousePassiveMotionFunction(int, int);
    void static launchDisplay();
    void static followDisplay();
    void static mouseClickFunction(int,int,int,int);
    
private:
    static OGLevel* activeLevel;
    OGTerrain *terrain;
    OGBall *ball;
    OGProjection *projection;
    OGPhysic *physic;
    vector<OGLight*> lights; 
    static struct timeval before,now;
    double static time_diff(timeval before, timeval now);
    
    Vector3d oldMousePos;
    
    void drawMap();
};


#endif
