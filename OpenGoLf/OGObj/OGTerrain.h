//
//  OGTerrain.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
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

using namespace std;

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
    GLuint modelsDL;
    int H_SCALE;
    int V_SCALE;

    BMPHeader header;

    //vector<OGTexture*> texture;
    GLuint texture0, waterTex;

    int terrainFromImage(const char *filename, BMPHeader &header ,Vector3d* &vertex, Vector3d* &normals);
    GLuint createTerrainDL(BMPHeader &header,Vector3d* &vertex, Vector3d* &normals);
    void computeNormals(BMPHeader &header, Vector3d* &vertex, Vector3d* &normals);
    void initTexture();
    GLuint createModelsDL();
    void readModelsFromFile();
    void initWaterTexture();
    void loadModel(char type, double x, double z, double angle);
    Vector3d modelInitPosition(double x, double z, double offset);
    float yOffset = 0.003;

public:
    int getHScale();
    OGTerrain(string);
    ~OGTerrain();
    void draw();
    unsigned int getTerrainWidth();
    unsigned int getTerrainHeight();
    float getYOffset();
    Vector3d *vertex, *normals;
    vector<OGModel3DS*> models;
};


#endif
