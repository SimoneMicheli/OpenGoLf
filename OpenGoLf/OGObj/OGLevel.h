//
//  OGLevel.h
//  OpenGoLf
//
//  Created by Simone Micheli on 04/07/12.
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
#include "OGClub.h"
#include "OGCompass.h"
#include "OGRoom.h"
#include <sys/time.h>
#include <vector>

#define TIMER_LOOP 10

class OGLevel : public OGAmbient {

public:

    OGLevel();
    ~OGLevel();
    void init(string,string);

    void static mouseMotionFunction(int,int);
    void static launchDisplay();
    void static followDisplay();
    void static resize(int,int);
    void static mouseClickFunction(int,int,int,int);
    void static keyPress(unsigned char, int, int);
    void static timer(int);

private:
    static OGLevel* activeLevel;
    OGPov* pov, *oldPov, *eagle;
    OGTerrain *terrain;
    OGBall *ball, *oldBall;
    OGProjection *projection;
    OGPhysic *physic;
    vector<OGLight*> lights;
    OGCompass *wind,*map;
    OGClub club;
    float launchPower;
    bool count;
    static struct timeval before,now,launchTime;
    double static time_diff(timeval before, timeval now);
    bool static shooting;
    bool enabledEagleView;
    
    void shoot();
    void restoreLaunch();
    void drawPower(float power, const char* str);
    void toggleEagleView();

    Vector3d oldMousePos;

    //fog property
    GLboolean fogStatus;
    GLfloat fogColor[4],*skyColor,fogDensity,fogStart,fogEnd;
};


#endif
