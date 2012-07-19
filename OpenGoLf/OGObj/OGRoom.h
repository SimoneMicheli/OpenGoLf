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
    void static keyPress(unsigned char, int, int);

    vector<OGModel3DS*> models;

private:
    static OGRoom* activeRoom;
    GLuint modelsDL;
    OGPov* pov;
    GLuint createModelsDL();
    OGProjection *projection;
    vector<OGLight*> lights;
    bool locked;
    Vector3d oldMousePos;
    void materialWall();
    void drawRoom();


    //object in room
    void loadVase(double x, double y, double z, double angle);
    void loadArmchair(double x, double y, double z, double angle);
    void loadCabinet(double x, double y, double z, double angle);
    void loadDoor(double x, double y, double z, double angle);

};

#endif
