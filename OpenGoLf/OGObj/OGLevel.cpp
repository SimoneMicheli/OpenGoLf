#include <iostream>

#include "OGLevel.h"

extern int W_WIDTH;
extern int W_HEIGHT;

OGLevel* OGLevel::activeLevel=NULL;
struct timeval OGLevel::before,OGLevel::now,OGLevel::launchTime;
bool OGLevel::shooting=false;

OGLevel::OGLevel(){
    if (OGRoom::activeRoom->getFogStatus()){
        glEnable(GL_FOG);                   // Enables GL_FOG
    }
    glutSetCursor(GLUT_CURSOR_NONE);    //hide mouse pointer
    activeLevel = this;
    oldMousePos = Vector3d();

    fogColor[0] = 0.8;
    fogColor[1] = 0.8;
    fogColor[2] = 0.8;
    fogColor[3] = 1.0;

    glGetBooleanv(GL_FOG, &fogStatus);
    if (fogStatus) {
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
    launchPower = 0;
    enabledEagleView = false;
}

void OGLevel::init(string path, string modelPath){
    projection = new OGProjection();
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 150.0f);

    terrain = new OGTerrain(path, modelPath);
    
    double x=30.5,z=30.0; //initial ball position
    
    Vector3d pos = terrain->modelInitPosition(x, z, 0);
    
    ball = new OGBall(pos);
    oldBall = new OGBall();
    
    pov = new OGPov(pos); //initial pov
    oldPov = new OGPov();
    pov->setRotation(-10, -45);

    //eagle view POV
    eagle = new OGPov(5,60,5);
    eagle->setRotation(-45, -45);

    //create physics
    physic = new OGPhysic(ball, terrain, pov);

    map = new OGCompass(0, W_HEIGHT- 130, 150, 150);
    wind = new OGCompass(W_WIDTH - 150, W_HEIGHT-130, 150, 150);

    //default club
    club = OGClub::DRIVER;

    OGLight *light0 = new OGLight(GL_LIGHT0,50.0f,50.0,50.0f,0.0f);
    light0->set();
    lights.push_back(light0);
    light0->enable();
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);

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
    glutReshapeFunc(OGLevel::resize);
    glutPostRedisplay();
}

void OGLevel::resize(int x, int y){
    W_WIDTH = x;
    W_HEIGHT = y;
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    activeLevel->projection->setAspect(aspect);
    activeLevel->projection->init();
    activeLevel->map->setPosition(0, W_HEIGHT- 130);
    activeLevel->wind->setPosition(W_WIDTH - 150, W_HEIGHT-130);
    glutPostRedisplay();
}

//-----------------------mouse functions-----------------------
void OGLevel::mousePassiveMotionFunction(int x, int y){
    //recupera la posizione del mouse all'inizo dopo il primo movimento
    if (activeLevel->oldMousePos.x != 0 || activeLevel->oldMousePos.y != 0) {
        //double a = (activeLevel->oldMousePos.y - (double) y) / 1;
        double b = (activeLevel->oldMousePos.x - (double) x) / 1;
        activeLevel->pov->addRotation(0, b);//disabilito rotazione su giu
    }

    activeLevel->oldMousePos.x = x;
    activeLevel->oldMousePos.y = y;
    glutPostRedisplay();
}

void OGLevel::mouseMotionFunction(int x, int y){
    //recupera la posizione del mouse all'inizo dopo il primo movimento
    if (activeLevel->oldMousePos.x != 0 || activeLevel->oldMousePos.y != 0) {
        //double a = (activeLevel->oldMousePos.y - (double) y) / 1;
        double b = (activeLevel->oldMousePos.x - (double) x) / 1;
        activeLevel->pov->addRotation(0, b); //disabilito rotazione su giu
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
    glColor3f(0.8, 0.8, 0.8);

    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    activeLevel->ball->draw();

    activeLevel->drawPower(activeLevel->launchPower,activeLevel->club.toString().c_str());

    Vector3d holePos = activeLevel->terrain->getHole().pos - activeLevel->pov->getPosition();
    Vector3d dir = activeLevel->pov->getDirection();
    double angle = holePos.beta() - dir.beta();

    activeLevel->map->draw(angle);

    Vector3d wind = activeLevel->physic->getWind();
    angle = wind.beta() - dir.beta();
    activeLevel->wind->draw((2*M_PI)- angle, wind.length());

    glutSwapBuffers();
}

//---------------------follow display--------------------------
void OGLevel::followDisplay(){
    glClearColor(activeLevel->skyColor[0], activeLevel->skyColor[1], activeLevel->skyColor[2],activeLevel->skyColor[3]);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glColor3f(0.8, 0.8, 0.8);

    gettimeofday(&OGLevel::now,NULL);
    double dtime = time_diff(OGLevel::before,OGLevel::now);

    if (activeLevel->physic->edgeCollision()){
        *activeLevel->ball = *activeLevel->oldBall;
        *activeLevel->pov = *activeLevel->oldPov;
        activeLevel->restoreLaunch();
    }else{
        activeLevel->physic->update(dtime);

        if (activeLevel->ball->getSpeed().length() < 0.1){
            if (activeLevel->physic->holeCollision()) {
                //back to room
                OGRoom::activeRoom->reInit();
                return;
            }else
                activeLevel->restoreLaunch();
        }
    }

    activeLevel->pov->lookAt();
    activeLevel->terrain->draw();
    
    activeLevel->ball->draw();

    OGLevel::before = OGLevel::now;

    glutSwapBuffers();
    glutPostRedisplay();
}

void OGLevel::drawPower(float power, const char* str){

    GLdouble modelMatrix[16], projMatrix[16];
    GLboolean depth,light,texture;

    power = 10.0 - (power * 10.0);

    //save opengl state
    glGetBooleanv(GL_DEPTH_TEST, &depth);
    glGetBooleanv(GL_LIGHTING, &light);
    glGetBooleanv(GL_TEXTURE_2D, &texture);

    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, 70, 140);
    glOrtho(0, 5, 10, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //disable opengl state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    //print text
    glColor3f(1, 1, 1);
    glRasterPos2f(0,0);

    for (int i = 0; i < strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

    if (power != 10.0) {
        //draw bar
        glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex2d(0, 0);
        glVertex2d(2, 0);
        glColor3f(0, 0, 1);
        glVertex2d(2, 10);
        glVertex2d(0, 10);
        glEnd();

        //draw arrow
        glTranslated(2, power, 0);
        glColor3f(1,1,1);
        glBegin(GL_TRIANGLES);
        glVertex2d(0, 0);
        glVertex2d(1, -0.5);
        glVertex2d(1, 0.5);
        glEnd();
    }

    //reload old state
    glLoadMatrixd(modelMatrix);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(projMatrix);
    glMatrixMode(GL_MODELVIEW);

    //enable opengl state
    if (depth)
        glEnable(GL_DEPTH_TEST);
    if (light)
        glEnable(GL_LIGHTING);
    if (texture)
        glEnable(GL_TEXTURE_2D);
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
    activeLevel->launchPower += STEP;
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
    }

}

//----------------------key press---------------------
void OGLevel::keyPress(unsigned char key, int x, int y){
    //change club
    if (key == 'c' || key == 'C') {
        activeLevel->club++;
        glutPostRedisplay();
    }
    //toggle eagle view
    if ((key == 'e' || key == 'E') && !shooting) {
        activeLevel->toggleEagleView();
        glutPostRedisplay();
    }
    //quit game
    if ((key == 'q' || key == 'Q') && !shooting) {
        exit(0);
    }
    //back to room
    if ((key == 'r' || key == 'R') && !shooting) {
        OGRoom::activeRoom->reInit();
        return;
    }
}

//---------------------shoot function----------------
void OGLevel::shoot(){
    count = false;
    shooting = true;
    //copy obj
    *oldBall = *ball;
    *oldPov = *pov;

    physic->shoot(launchPower * club.getPower(), club.getAngle());
    gettimeofday(&OGLevel::before,NULL);
    glutDisplayFunc(OGLevel::followDisplay);
    glutPostRedisplay();
}

//-----------------restore launch function----------
void OGLevel::restoreLaunch(){
    count = true;
    shooting = false;
    activeLevel->launchPower = 0;
    glutDisplayFunc(OGLevel::launchDisplay);
    glutPassiveMotionFunc(OGLevel::mousePassiveMotionFunction);
    glutMotionFunc(OGLevel::mouseMotionFunction);
}

//----------------eagle view-----------------------
void OGLevel::toggleEagleView(){
    if (!enabledEagleView) {
        glDisable(GL_FOG);
        enabledEagleView = true;
        glutMouseFunc(NULL);
        *oldPov = *pov;
        *pov = *eagle;
    }else {
        enabledEagleView = false;
        *pov = *oldPov;
        if (fogStatus) {
            glEnable(GL_FOG);
        }
        glutMouseFunc(OGLevel::mouseClickFunction);
    }
}

OGLevel::~OGLevel(){
    activeLevel = NULL;
    delete terrain;
    delete ball;
    delete oldBall;
    delete projection;
    delete physic;
    delete pov;
    delete oldPov;
    delete eagle;
    delete wind;
    delete map;
    while (!lights.empty())
    {
        delete lights.back();
        lights.pop_back();
    }

}
