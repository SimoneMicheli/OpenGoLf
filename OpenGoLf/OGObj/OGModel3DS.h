//
//  OGModel3DS.h
//  OpenGoLf
//
//  Created by Simone Micheli on 12/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "OGObject.h"
#include "../lib/lib3ds/lib3ds.h"
#include <assert.h>
typedef float Lib3dsVector[3];

#ifndef OpenGoLf_OGModel3DS_h
#define OpenGoLf_OGModel3DS_h

class  OGModel3DS : public OGObject{
    Lib3dsFile* f;
    Lib3dsVector *normals, *vertex;
    GLuint objDL;
    unsigned int nfaces;
    
    void createDL();
    unsigned int getFaces();
    Vector3d scale;
    double rotation[4];
    Vector3d position;
    
public:
    OGModel3DS(std::string fileName);
    void draw();
    void setScale(double x,double y, double z);
    void setScale(Vector3d v);
    Vector3d getScale();
    void setRotation(double angle,double x,double y,double z);
    double* getRotation();
    void setPosition(double x, double y, double z);
    void setPosition(Vector3d v);
    Vector3d getPosition();
};

#endif
