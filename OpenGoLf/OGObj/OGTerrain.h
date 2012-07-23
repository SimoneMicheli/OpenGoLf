//
//  OGTerrain.h
//  OpenGoLf
//
//  Created by Simone Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGTerrain_h
#define OpenGoLf_OGTerrain_h

#include "OGObject.h"
#include "OGModel3DS.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../texture/TXgrass.h"
#include "../texture/TXwater.h"
#include "../texture/TXtree.h"

using namespace std;

struct OGHole{
    double radius;
    Vector3d pos;
};

class OGTerrain : public OGObject{
private:
    struct BMPHeader{
        DWORD   fsize;
        DWORD   reserved;
        DWORD   data_offset;
        DWORD   hlen;
        DWORD   width;
        DWORD   height;
        WORD    nplanes;
        WORD    bpp;
        DWORD   compression;
        DWORD   size;
        DWORD   hres;
        DWORD   vres;
        DWORD   used_color;
        DWORD   imp_color;
    };

    GLuint terrainDL;
    GLuint modelsDL, holeDL;
    int H_SCALE;
    int V_SCALE;

    BMPHeader header;
    OGHole hole;

    GLuint texture[2], waterTex;

    int terrainFromImage(const char *filename, BMPHeader &header ,Vector3d* &vertex, Vector3d* &normals);
    GLuint createTerrainDL(BMPHeader &header,Vector3d* &vertex, Vector3d* &normals);
    void computeNormals(BMPHeader &header, Vector3d* &vertex, Vector3d* &normals);
    void initTexture();
    GLuint createModelsDL();
    GLuint createHoleDL();
    void readModelsFromFile(string);
    void initWaterTexture();
    void initModelsTexture();
    void loadModel(char type, double x, double z, double angle);
    float yOffset;

public:
    int getHScale();
    OGTerrain(string, string);
    ~OGTerrain();
    void draw();
    unsigned int getTerrainWidth();
    unsigned int getTerrainHeight();
    float getYOffset();
    OGHole getHole();
    Vector3d *vertex, *normals;
    vector<OGModel3DS*> models;
    Vector3d modelInitPosition(double x, double z, double offset);
};


#endif
