#include <iostream>

#include "OGLevel.h"

OGLevel* OGLevel::activeLevel=NULL;
struct timeval OGLevel::before,OGLevel::now,OGLevel::stopTime;

OGLevel::OGLevel(){
    activeLevel = this;
    oldMousePos = Vector3d();
}

void OGLevel::init(string path){
    projection = new OGProjection(); 
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 1000.0f);

    terrain = new OGTerrain(path);
    ball = new OGBall(0.12,4,0.22);
    pov = new OGPov(0.12,3,0.22); //initial pov
    pov->setRotation(-30, -90);
    
    //create physics
    physic = new OGPhysic(ball, terrain, pov);
    
    OGLight *light0 = new OGLight(GL_LIGHT0,0.0f,1.0,0.0f,0.0f);
    light0->set();
    lights.push_back(light0);
    light0->enable();
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
    //activeLevel->drawMap();

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    
    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    activeLevel->ball->draw();
    
    activeLevel->drawMap();
    
    glutSwapBuffers();
    
}

//---------------------follow display--------------------------
void OGLevel::followDisplay(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    
    gettimeofday(&OGLevel::now,NULL);
    double dtime = time_diff(OGLevel::before,OGLevel::now);
    
    activeLevel->physic->update(dtime);
    
    if (activeLevel->ball->getSpeed().length() < 0.01){
        glutDisplayFunc(OGLevel::launchDisplay);
        glutPassiveMotionFunc(OGLevel::wrapperMousePassiveMotionFunction);
    }
    
    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    activeLevel->ball->draw();
    
    OGLevel::before = OGLevel::now;
    
    glutSwapBuffers();
    glutPostRedisplay();
}

//-------------------display map viewport----------------------
void OGLevel::drawMap(){
    GLdouble modelMatrix[16], projMatrix[16];
    
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
    glViewport(0, 0, 200, 200);
    
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
    activeLevel->ball->setSpeed(2, 5, 2);
    gettimeofday(&OGLevel::before,NULL);
    glutDisplayFunc(OGLevel::followDisplay);
    glutPostRedisplay();
}

OGLevel::~OGLevel(){
    activeLevel = NULL;
    delete terrain;
    delete ball;
    delete projection;
    delete physic;
    while (!lights.empty())
    {
        delete lights.back();
        lights.pop_back();
    }
    
}
