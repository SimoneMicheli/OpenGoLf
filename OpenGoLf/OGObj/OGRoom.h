//
//  OGRoom.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGRoom_h
#define OpenGoLf_OGRoom_h

#include "OGLevel.h"
#include "OGAmbient.h"
#include "OGProjection.h"
#include "OGModel3DS.h"
#include <vector>
#include "OGLight.h"

class OGRoom : public OGAmbient{
public:

    OGRoom();
    ~OGRoom();
    void init();
    void static mousePassiveMotionFunction(int, int);
    void static mouseMotionFunction(int,int);
    void static roomDisplay();
    void static resize(int,int);
    void static mouseClickFunction(int,int,int,int);
    //void static keyPress(unsigned char, int, int);

    vector<OGModel3DS*> models;

private:
    static OGRoom* activeRoom;
    OGPov* pov;
    GLuint createModelsDL();
    OGProjection *projection;
    vector<OGLight*> lights;
    Vector3d oldMousePos;
    void materialWall();
    void drawBall(double x, double y, double z, double angle);

};

#endif
