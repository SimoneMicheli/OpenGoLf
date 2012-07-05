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
    V_SCALE = 50;
    
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
        printf("memory allocation error");
        return -1;
    }
    
    //go to data
    in.seekg(header.data_offset);
    
    int read_px = (header.bpp * header.width)/32*4;
    int padding = read_px % 4;
    int r,c;
    CHAR tmp;
    float x,z,y;
    
    //read each row
    for (r=0; r<header.height; r++) {
        //read each pixel per row
        for (c=0; c < header.width; c++) {
            
            in.read((char *)&tmp, header.bpp/8);
            //heights[(r * header.width) + c] = (float) tmp / 256;
            
            x = (float) c/ H_SCALE;
            z = (float) r / H_SCALE;
            y = (float) (tmp - 127) / V_SCALE;
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
    
    //light_global();
    glColor3f(0, 0, 0);
    
    //crate n stip one for each image line
    for (int r=0; r < header.height - 1; r++) {
        
        glBegin(GL_TRIANGLE_STRIP);
        
        for (int c=0; c < header.width; c++) {
            
            v1 = vertex[ (r * header.width) + c];
            v2 = vertex[ ((r+1) * header.width) + c];
            n1 = normals [ (r * header.width) + c];
            n2 = normals [ ((r+1) * header.width) + c];
            
            glNormal3d(n1.x, n1.y, n1.z);
            glVertex3d(v1.x, v1.y, v1.z);
            glNormal3f(n2.x, n2.y, n2.z);
            glVertex3d(v2.x, v2.y, v2.z);
        }
        
        glEnd();
        
    }
    
    glEndList();
    
    return terrainDL;
}