#include <iostream>

#include "OGLevel.h"

OGLevel* OGLevel::activeLevel=NULL;
struct timeval OGLevel::before,OGLevel::now,OGLevel::launchTime;

OGLevel::OGLevel(){
    activeLevel = this;
    oldMousePos = Vector3d();
}

void OGLevel::init(string path){
    projection = new OGProjection(); 
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 100.0f);

    terrain = new OGTerrain(path);
    ball = new OGBall(5,0,5);
    oldBall = new OGBall();
    pov = new OGPov(5,0,5); //initial pov
    oldPov = new OGPov();
    pov->setRotation(-30, -90);
    
    //create physics
    physic = new OGPhysic(ball, terrain, pov);
    
    OGLight *light0 = new OGLight(GL_LIGHT0,0.0f,50.0,50.0f,0.0f);
    light0->set();
    lights.push_back(light0);
    light0->enable();
    
    //fog
    //glClearColor(0.5f,0.5f,0.5f,1.0f);          // We'll Clear To The Color Of The Fog ( Modified )
    GLfloat fogColor[4]= {0.8f, 0.8f, 0.8f, 1.0f};
    
    glFogi(GL_FOG_MODE, GL_LINEAR);        // Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
    glFogf(GL_FOG_DENSITY, 0.005f);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_NICEST);          // Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
    glFogf(GL_FOG_END, 20.0f);               // Fog End Depth
    glEnable(GL_FOG);                   // Enables GL_FOG
}

//-----------------------mouse functions-----------------------
void OGLevel::mousePassiveMotionFunction(int x, int y){
    //recupera la posizione del mouse all'inizo dopo il primo movimento
    if (activeLevel->oldMousePos.x != 0 || activeLevel->oldMousePos.y != 0) {
        double a = (activeLevel->oldMousePos.y - (double) y) / 1;
        double b = (activeLevel->oldMousePos.x - (double) x) / 1;
        activeLevel->pov->addRotation(a, b);
    }
    
    activeLevel->oldMousePos.x = x;
    activeLevel->oldMousePos.y = y;
    glutPostRedisplay();
}


//--------------------launch display function-----------------
void OGLevel::launchDisplay(){
    //activeLevel->drawMap();

    glClearColor(0.376, 0.77, 1, 1.0);
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
    glClearColor(0.376, 0.77, 1, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    
    gettimeofday(&OGLevel::now,NULL);
    double dtime = time_diff(OGLevel::before,OGLevel::now);
    
    if (activeLevel->physic->terrainEdge()){
        *activeLevel->ball = *activeLevel->oldBall;
        *activeLevel->pov = *activeLevel->oldPov;
    }
    
    activeLevel->physic->update(dtime);
    
    if (activeLevel->ball->getSpeed().length() < 0.01){
        glutDisplayFunc(OGLevel::launchDisplay);
        glutPassiveMotionFunc(OGLevel::mousePassiveMotionFunction);
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
    
    //glPushMatrix();
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
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
    
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        //copy old object
        //activeLevel->oldBall = new OGBall(0,0,0);
        *activeLevel->oldBall = *activeLevel->ball;
        *activeLevel->oldPov = *activeLevel->pov;
        
        printf("std: %f\n",activeLevel->ball->getSpeed().x);
        printf("old: %f\n",activeLevel->pov->getDirection().x);
        //return;
        
        glutPassiveMotionFunc(NULL); //disattivo rotazione
        
        activeLevel->physic->shoot(10);
        gettimeofday(&OGLevel::before,NULL);
        glutDisplayFunc(OGLevel::followDisplay);
        glutPostRedisplay();    
    }
    
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
