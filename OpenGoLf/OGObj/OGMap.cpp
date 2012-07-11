
#include "OGMap.h"
#include "OGPov.h"
#include "OGTerrain.h"
#include "OGProjection.h"

//OGMap* OGMap::activeLevel=NULL;

OGMap::OGMap(Vector3d position, int width, int height){
    activeLevel = this;
    this.position = position;
    this.width = width;
    this.height = height;
    modelMatrix = new GLdouble[16];
    projMatrix = new GLdouble[16];
}

void OGMap::draw(){

    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    OGProjection *p = new OGProjection();
    p->setOrtho(0.1f, 100.0f, -8.0f, 8.0f, 8.0f, -8.0f);

    OGPov *povl = new OGPov(8,5,0);
    povl->setRotation(-90, 0);

    povl->lookAt();
    glViewport(position.x, position.y, width, height);

    terrain->draw();

    glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
    glLoadMatrixd(projMatrix);

    glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
    glLoadMatrixd(modelMatrix);

    //!important avoid memory leak
    delete p;
    delete povl;
}
