//
//  OGModel3DS.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 12/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGModel3DS.h"

OGModel3DS::OGModel3DS(std::string fileName){
    
    f=NULL;
    
    f = lib3ds_file_open(fileName.c_str());
    
    if (!f) {
        throw strcat("Unable to load ", fileName.c_str());
    }
    
    position = Vector3d();
    scale = Vector3d(1,1,1);
    rotation[0]=0;
    rotation[1]=0;
    rotation[2]=0;
    rotation[3]=0;
    
    createVBO();
    
}

unsigned int OGModel3DS::getFaces(){
    
    assert(f != NULL);
    
    unsigned int faces;
    
    for (int i=0; i < f->nmeshes; i++) {
        faces += f->meshes[i]->nfaces;
    }
    
    return faces;
}

void OGModel3DS::createVBO(){
    
    assert(f != NULL);
    
    nfaces = this->getFaces();
    
    vertex = (Lib3dsVector*) malloc(nfaces * 3 * sizeof(Lib3dsVector));
    normals = (Lib3dsVector*) malloc(nfaces * 3 * sizeof(Lib3dsVector));
    
    Lib3dsMesh *mesh;
    unsigned int finishedFace = 0;
    
    //iterate over meshes
    for (int i=0; i < f->nmeshes; i++) {
        mesh = f->meshes[i];
        lib3ds_mesh_calculate_vertex_normals(mesh, &normals[finishedFace*3]);
        
        //iterate over faces
        for(unsigned int cur_face = 0; cur_face < mesh->nfaces;cur_face++)
        {
            Lib3dsFace * face = &mesh->faces[cur_face];
            //copy the 3 vertex of triangle
            for (int j =0 ; j<3; j++) {
                memcpy(&vertex[finishedFace*3 + j], mesh->vertices[face->index[j]], sizeof(Lib3dsVector));
            }
            finishedFace++;
        }
    }
    
    glGenBuffers(2, (GLuint* ) &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * nfaces, vertex, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * nfaces, normals, GL_STATIC_DRAW);
    
    //destroy memory object
    free(normals);
    free(vertex);
    
    lib3ds_file_free(f);
    f = NULL;
}

void OGModel3DS::draw(){
    
    assert(nfaces != 0);
    
    glPushMatrix();
    
    glTranslated(position.x, position.y, position.z);
    glScaled(scale.x, scale.y, scale.z);
    glRotated(rotation[0], rotation[1], rotation[2], rotation[3]);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    //bind vertex array
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    
    glNormalPointer(GL_FLOAT, 0, 0);
    
    //bind vertex array
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    //render triangle
    glDrawArrays(GL_TRIANGLES, 0, nfaces* 3);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    glPopMatrix();
}

OGModel3DS::~OGModel3DS(){
    free(normals);
    free(vertex);
}

void OGModel3DS::setScale(double x, double y, double z){
    scale = Vector3d(x,y,z);
}

void OGModel3DS::setScale(Vector3d v){
    scale = v;
}

Vector3d OGModel3DS::getScale(){
    return scale;
}

void OGModel3DS::setRotation(double angle, double x, double y, double z){
    rotation[0]=angle;
    rotation[1]=x;
    rotation[2]=y;
    rotation[3]=z;
}

double* OGModel3DS::getRotation(){
    return rotation;
}

void OGModel3DS::setPosition(double x, double y, double z){
    position = Vector3d(x,y,z);
}

void OGModel3DS::setPosition(Vector3d v){
    position = v;
}

Vector3d OGModel3DS::getPosition(){
    return position;
}
