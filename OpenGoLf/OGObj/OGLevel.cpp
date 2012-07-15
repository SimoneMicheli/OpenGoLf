#include <iostream>

#include "OGLevel.h"

OGLevel* OGLevel::activeLevel=NULL;
struct timeval OGLevel::before,OGLevel::now,OGLevel::launchTime;
bool OGLevel::shooting=false;

OGLevel::OGLevel(){
    activeLevel = this;
    oldMousePos = Vector3d();
    
    fogColor[0] = 0.8;
    fogColor[1] = 0.8;
    fogColor[2] = 0.8;
    fogColor[3] = 1.0;
    
    GLboolean fogEnabled;
    glGetBooleanv(GL_FOG, &fogEnabled);
    if (fogEnabled) {
        skyColor = fogColor;
    }else {
        skyColor = new GLfloat[4];
        skyColor[0]=0.376;
        skyColor[1]=0.77;
        skyColor[2]=1.0;
        skyColor[3]=1.0;
    }
    
    fogDensity = 0.5;
    fogStart = 5.0;
    fogEnd = 60.0;
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
    
    //default club
    club = OGClub::DRIVER;
    
    OGLight *light0 = new OGLight(GL_LIGHT0,0.0f,50.0,50.0f,0.0f);
    light0->set();
    lights.push_back(light0);
    light0->enable();
    
    //fog
    
    glFogi(GL_FOG_MODE, GL_LINEAR);        // Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
    glFogf(GL_FOG_DENSITY, fogDensity);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_NICEST);          // Fog Hint Value
    glFogf(GL_FOG_START, fogStart);             // Fog Start Depth
    glFogf(GL_FOG_END, fogEnd);               // Fog End Depth
    
    //set event function
    glutKeyboardFunc(OGLevel::keyPress);
    glutDisplayFunc(OGLevel::launchDisplay);
    glutPassiveMotionFunc(OGLevel::mousePassiveMotionFunction);
    glutMotionFunc(OGLevel::mouseMotionFunction);
    glutMouseFunc(OGLevel::mouseClickFunction);
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

void OGLevel::mouseMotionFunction(int x, int y){
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
    glClearColor(activeLevel->skyColor[0], activeLevel->skyColor[1], activeLevel->skyColor[2],activeLevel->skyColor[3]);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    
    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    activeLevel->ball->draw();
    renderString(activeLevel->club.toString().c_str());
    printf("pow: %f\n", activeLevel->launchPower);
    //activeLevel->drawMap();
    
    glutSwapBuffers();
    
}

//---------------------follow display--------------------------
void OGLevel::followDisplay(){
    glClearColor(activeLevel->skyColor[0], activeLevel->skyColor[1], activeLevel->skyColor[2],activeLevel->skyColor[3]);
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
    
    if (activeLevel->ball->getSpeed().length() < 0.015){
        activeLevel->restoreLaunch();
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

//---------------------launch count time--------------------
void OGLevel::timer(int args){
    activeLevel->launchPower += 0.01;
    if (activeLevel->launchPower>=1) {
        activeLevel->shoot();
    }
    glutPostRedisplay();
    if (activeLevel->count) {
        glutTimerFunc(TIMER_LOOP, OGLevel::timer, 0);
    }
}

//---------------------mouse click--------------------

void OGLevel::mouseClickFunction(int button,int state, int x, int y){
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && !shooting){
        activeLevel->launchPower = 0;
        activeLevel->count = true;
        activeLevel->timer(0);
    }
    
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        if (!shooting) 
            activeLevel->shoot();
        shooting = false;
    }
    
}

//----------------------key press---------------------
void OGLevel::keyPress(unsigned char key, int x, int y){
    if (key == 'c' || key == 'C') {
        activeLevel->club++;
        glutPostRedisplay();
    }
}

//---------------------shoot function----------------
void OGLevel::shoot(){
    count = false;
    shooting = true;
    //copy obj
    *oldBall = *ball;
    *oldPov = *pov;
    
    glutPassiveMotionFunc(NULL); //disattivo rotazione
    glutMotionFunc(NULL);
    
    physic->shoot(launchPower * club.getPower(), club.getAngle());
    gettimeofday(&OGLevel::before,NULL);
    glutDisplayFunc(OGLevel::followDisplay);
    glutPostRedisplay();
}

//-----------------restore launch function----------
void OGLevel::restoreLaunch(){
    count = true;
    glutDisplayFunc(OGLevel::launchDisplay);
    glutPassiveMotionFunc(OGLevel::mousePassiveMotionFunction);
    glutMotionFunc(OGLevel::mouseMotionFunction);
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
