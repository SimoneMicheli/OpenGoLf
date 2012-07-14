#include <iostream>

#include "OGLevel.h"

OGLevel* OGLevel::activeLevel=NULL;
struct timeval OGLevel::before,OGLevel::now;

OGLevel::OGLevel(){
    activeLevel = this;
    oldMousePos = Vector3d();
}

void OGLevel::init(string path){
    projection = new OGProjection();
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 1000.0f);

    terrain = new OGTerrain(path);
    ball = new OGBall(0.3,1,0.3);
    pov = new OGPov(-20,30,-20); //initial pov
    pov->setRotation(-30, -45);

    //create physics
    physic = new OGPhysic(ball, terrain);

    OGLight *light0 = new OGLight(GL_LIGHT0,-3.0f,3.0,-3.0f,0.0f);
    light0->set();
    lights.push_back(light0);
    light0->enable();


}


void OGLevel::drawmap(){
    GLdouble modelMatrix[16],projMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    OGProjection *p = new OGProjection();
    p->setOrtho(0.1f,100.0f,-8.0f,8.0f,8.0f,-8.0f);
    OGPov *pov = new OGPov(8,5,0);
    pov->setRotation(-89,0);


    pov->lookAt();

    glViewport(0,0,200,100);
    terrain->draw();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(projMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(modelMatrix);
    delete p;
    delete pov;

}


void OGLevel::drawwind(){

    GLdouble modelMatrix[16],projMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    OGProjection *p = new OGProjection();
    p->setOrtho(0.1f,100.0f,-4.0f,4.0f,4.0f,-4.0f);
    OGPov *pov = new OGPov(0,1,0);


    pov->lookAt();

    glViewport(0,400,200,200);
    OGLight *light7 = new OGLight(GL_LIGHT7,0.0f,10.0,0.0f,0.0f);
    light7->set();
    light7->enable();
    glDisable(GL_TEXTURE_2D);
    glutSolidSphere(3,50,50);
    light7->disable();

    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(projMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(modelMatrix);
    delete p;
    delete pov;
}
//-----------------------mouse functions-----------------------

void OGLevel::wrapperMousePassiveMotionFunction(int x, int y){
    activeLevel->mousePassiveMotionFunction(x,y);
}

void OGLevel::mousePassiveMotionFunction(int x, int y){
    //recupera la posizione del mouse all'inizo dopo il primo movimento
    if (oldMousePos.x != 0 || oldMousePos.y != 0) {
        double a = (oldMousePos.y - (double) y) / 5;
        double b = (oldMousePos.x - (double) x) / 5;
        pov->addRotation(a, b);
    }

    oldMousePos.x = x;
    oldMousePos.y = y;
    glutPostRedisplay();
}


//--------------------launch display function-----------------
void OGLevel::launchDisplay(){

    glClearColor(0.377, 0.77, 1, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    //activeLevel->projection->init();
    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    activeLevel->ball->draw();
    activeLevel->drawmap();
    activeLevel->drawwind();
    glutSwapBuffers();

}

//---------------------follow display--------------------------
void OGLevel::followDisplay(){
    glClearColor(0.377, 0.77, 1, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    gettimeofday(&OGLevel::now,NULL);
    double dtime = time_diff(OGLevel::before,OGLevel::now);

    activeLevel->physic->update(dtime);

    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    activeLevel->ball->draw();

    OGLevel::before = OGLevel::now;

    glutSwapBuffers();
    glutPostRedisplay();
}


//---------------------time diff function--------------------
double OGLevel::time_diff(timeval before, timeval now){
    double x_ms, y_ms, diff;

    x_ms = (double)before.tv_sec * 1000000 + (double)before.tv_usec;
    y_ms = (double)now.tv_sec * 1000000 + (double)now.tv_usec;

    diff = ((double)y_ms - (double)x_ms)/ 1000000.0f;

    return diff;
}

//---------------------mouse click--------------------

void OGLevel::mouseClickFunction(int button,int state, int x, int y){
    glutPassiveMotionFunc(NULL); //disattivo rotazione se mouse cliccato
    activeLevel->ball->setSpeed(0, 5, 0);
    gettimeofday(&OGLevel::before,NULL);

    glutDisplayFunc(OGLevel::followDisplay);
    glutPostRedisplay();
}

