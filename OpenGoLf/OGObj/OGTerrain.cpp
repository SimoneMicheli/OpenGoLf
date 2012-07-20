//
//  OGTerrain.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGTerrain.h"

OGTerrain::OGTerrain(string path, string modelPath){
    H_SCALE = 10;
    V_SCALE = 50;

    hole.radius = 0.108;
    yOffset = 0.045;

    initWaterTexture();
    terrainFromImage(path.c_str(), header, vertex, normals);
    terrainDL = createTerrainDL(header, vertex, normals);

    readModelsFromFile(modelPath);
    modelsDL = createModelsDL();

    holeDL = createHoleDL();
}

void OGTerrain::draw(){
    //show terrain
    glCallList(terrainDL);
    glCallList(modelsDL);

    //show hole
    glDisable(GL_TEXTURE_2D);
    glCallList(holeDL);
    glEnable(GL_TEXTURE_2D);
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

    initTexture();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float c_step = 1.0 / ((float)header.width / 100.0);
    float r_step = 1.0 / ((float)header.height / 100.0);

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

    glBindTexture(GL_TEXTURE_2D, waterTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //draw water
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2d(0, 0);
    glVertex3f(-1000, yOffset, -1000);
    glTexCoord2d(1000, 0);
    glVertex3f(1000, yOffset, -1000);
    glTexCoord2d(1000, 1000);
    glVertex3f(1000, yOffset,1000);
    glTexCoord2d(0, 1000);
    glVertex3f(-1000, yOffset ,1000);
    glEnd();

    glEndList();

    return terrainDL;
}

void OGTerrain::initWaterTexture(){
    glGenTextures(1, &waterTex);
    glBindTexture(GL_TEXTURE_2D, waterTex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TXwater.width, TXwater.height, 0, GL_RGB, GL_UNSIGNED_BYTE, TXwater.pixel_data);
}

void OGTerrain::initTexture(){
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TXgrass.width, TXgrass.height, 0, GL_RGB, GL_UNSIGNED_BYTE, TXgrass.pixel_data);
}

void OGTerrain::initModelsTexture(){
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TXtree.width, TXtree.height, 0, GL_RGB, GL_UNSIGNED_BYTE, TXtree.pixel_data);
}

int OGTerrain::getHScale(){
    return H_SCALE;
}

unsigned int OGTerrain::getTerrainWidth(){
    return header.width;
}

unsigned int OGTerrain::getTerrainHeight(){
    return header.height;
}

GLuint OGTerrain::createModelsDL(){
    //create models DisplayList
    initModelsTexture();
    GLuint modelsDL = glGenLists(1);
    glNewList(modelsDL, GL_COMPILE);
    
    //bind texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for (int i=0; i<models.size(); i++) {
        models[i]->draw();
    }

    glEndList();

    return modelsDL;
}


template<typename T> T StringToNumber(const std::string& numberAsString)
{
    T valor;

    std::stringstream stream(numberAsString);
    stream >> valor;
    return valor;
}

GLuint OGTerrain::createHoleDL(){
    GLuint list = glGenLists(1);

    glNewList(list, GL_COMPILE);


    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(0,0,0);
    glTranslated(hole.pos.x, hole.pos.y+0.01 , hole.pos.z);
    glScalef(1, 0.01, 1);
    glutSolidSphere(hole.radius, 10, 10);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(1,1,1);
    float rodDim = 0.03;
    glTranslated(hole.pos.x+hole.radius+(rodDim/2), hole.pos.y+rodDim*100*0.5 , hole.pos.z);
    glScalef(1, 100, 1);
    glutSolidCube(rodDim);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslated(hole.pos.x+hole.radius+(rodDim/2), hole.pos.y+rodDim*100-0.16 , hole.pos.z+0.24);
    glScalef(0.1, 1, 1.5);
    glutSolidCube(0.31);
    glPopMatrix();
    glEnable(GL_LIGHTING);


    glEndList();

    return list;
}

void OGTerrain::readModelsFromFile(string path){
    ifstream file;

    file.open(path.c_str(), ios::binary | ios::in);

    if (!file){
        printf("can't open model file\n");
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream linestream(line);
        string info;
        double x,z,angle;
        char type;

        linestream>>info;
        type = info.c_str()[0];
        linestream>>info;
        x = StringToNumber<double>(info);
        linestream>>info;
        z = StringToNumber<double>(info);
        linestream>>info;
        angle = StringToNumber<double>(info);
        loadModel(type, x, z, angle);
    }

    file.close();
}

void OGTerrain::loadModel(char type, double x, double z, double angle){
    OGModel3DS *model;
    switch (type) {
        case 'T':{
            model = new OGModel3DS(TREE);
            Vector3d pos = modelInitPosition(x, z, 0);
            model->setPosition(pos);
            model->setRotation(-90, 1, 0, 0);
            model->setScale(0.0003, 0.0003, 0.0003);
            break;
        }
        case 'B':{

            model = new OGModel3DS(BUSH);
            model->setPosition(modelInitPosition(x, z, 0.3));
            model->setRotation(angle, 0, 1, 0);
            model->setRotation(90, 1, 0, 0);
            model->setScale(0.001, 0.001, 0.001);
            break;
        }
        case 'H':{
            Vector3d p = modelInitPosition(x, z, 0);
            hole.pos = p;
            return;
        }
        default:
            return;
    }
    models.push_back(model);
}

Vector3d OGTerrain::modelInitPosition(double x, double z, double offset){
    Vector3d newPos = Vector3d(x,0,z) * getHScale(); //posizione nell'immagine scalata

    Vector3d v1 = vertex[(int)(floor(newPos.z) * (int)getTerrainWidth() + floor(newPos.x))];
    Vector3d v2 = vertex[(int)(floor(newPos.z) * (int)getTerrainWidth() + floor(newPos.x + 1))];
    Vector3d v3 = vertex[(int)(floor(newPos.z +1) * (int)getTerrainWidth() + floor(newPos.x))];

    Vector3d center = (v1+v2+v3)/3;
    return Vector3d(x,center.y+offset,z);
}

OGTerrain::~OGTerrain(){
    free(vertex);
    free(normals);
    while (!models.empty())
    {
        delete models.back();
        models.pop_back();
    }
}

float OGTerrain::getYOffset(){
    return yOffset;
}

OGHole OGTerrain::getHole(){
    return hole;
}
