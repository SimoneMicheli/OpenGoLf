//
//  OGTerrain.cpp
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGTerrain.h"

OGTerrain::OGTerrain(string path){
    H_SCALE = 10;
    V_SCALE = 80;

    terrainFromImage(path.c_str(), header, vertex, normals);
    terrainDL = createTerrainDL(header, vertex, normals);
}

void OGTerrain::draw(){
    glCallList(terrainDL);
}

int OGTerrain::terrainFromImage(const char *filename, BMPHeader &header ,Vector3d* &vertex, Vector3d* &normals){
    ifstream in;
    in.open(filename , std::ios::in | std::ios::binary);

    if (!in) {
        printf("can't read file");
        return -1;
    }

    //skip starting byte
    in.seekg(2);
    in.read((char *)&header, sizeof(BMPHeader));

    //terrain height and normal
    vertex = (Vector3d *) malloc(sizeof(Vector3d) * header.width * header.height);
    normals = (Vector3d *) malloc(sizeof(Vector3d) * header.width * header.height);

    if (vertex == NULL || normals== NULL) {
        printf("Memory allocation error");
        return -1;
    }

    //go to data
    in.seekg(header.data_offset);

    int read_px = (header.bpp * header.width)/32*4;
    int padding = read_px % 4;
    int r,c;
    unsigned char tmp;
    float x,z,y;

    //read each row
    for (r=0; r<header.height; r++) {
        //read each pixel per row
        for (c=0; c < header.width; c++) {

            in.read((char *)&tmp, header.bpp/8);
            //heights[(r * header.width) + c] = (float) tmp / 256;

            x = (float) c/ H_SCALE;
            z = (float) r / H_SCALE;
            y = (float) tmp / V_SCALE;
            vertex[(r * header.width) + c] = Vector3d(x,y,z);
        }

        //skip read padding
        in.seekg(padding, std::ios::cur);
    }

    computeNormals(header, vertex, normals);

    return 0;
}

void OGTerrain::computeNormals(BMPHeader &header, Vector3d* &vertex, Vector3d* &normals){

    Vector3d v1,v2,v3,v4, norm;

    for (int r=0; r < header.height; r++) {

        for (int c = 0; c < header.width; c++) {

            //reset current normal vector
            norm = Vector3d();

            if (r==0)
                v1 = Vector3d();
            else
                v1 = vertex[((r-1) * header.width) + c] - vertex[(r * header.width) + c];


            if (c==0)
                v2 = Vector3d();
            else
                v2 = vertex[(r * header.width) + (c-1)] - vertex[(r * header.width) + c];

            if (r == header.height - 1)
                v3 = Vector3d();
            else
                v3 = vertex[((r+1) * header.width) + c] - vertex[(r * header.width) + c];

            if (c == header.width -1)
                v4 = Vector3d();
            else
                v4 = vertex[(r * header.width) + (c+1)] - vertex[(r * header.width) + c];

            //calculate and sum normal vector
            norm += v1.cross(v2);
            norm += v2.cross(v3);
            norm += v3.cross(v4);
            norm += v4.cross(v1);
            //normalize vector
            norm.normalize();

            normals[(r * header.width) + c] = norm;
        }
    }
}

GLuint OGTerrain::createTerrainDL(BMPHeader &header,Vector3d* &vertex, Vector3d* &normals){

    GLuint terrainDL;
    Vector3d v1,v2,n1,n2;


    //crate dl id
    terrainDL = glGenLists(1);
    //crate DL
    glNewList(terrainDL,GL_COMPILE);

    glColor3f(0, 0, 0);
    initTexture();
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float c_step = 1.0 / ((float)header.width / 15.0);
    float r_step = 1.0 / ((float)header.height / 15.0);

    //crate n stip one for each image line
    for (int r=0; r < header.height - 1; r++) {

        glBegin(GL_TRIANGLE_STRIP);

        for (int c=0; c < header.width; c++) {
            float s = c * c_step;
            float t = r * r_step;

            v1 = vertex[ (r * header.width) + c];
            v2 = vertex[ ((r+1) * header.width) + c];
            n1 = normals [ (r * header.width) + c];
            n2 = normals [ ((r+1) * header.width) + c];

            glTexCoord2f(s, t);
            glNormal3d(n1.x, n1.y, n1.z);
            glVertex3d(v1.x, v1.y, v1.z);
            glTexCoord2f(s, (r + 1) * r_step);
            glNormal3f(n2.x, n2.y, n2.z);
            glVertex3d(v2.x, v2.y, v2.z);
        }

        glEnd();

    }

    printf("finish\n");
    glEndList();

    return terrainDL;
}

void OGTerrain::initTexture(){
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TXgrass.width, TXgrass.height, 0, GL_RGB, GL_UNSIGNED_BYTE, TXgrass.pixel_data);
}
