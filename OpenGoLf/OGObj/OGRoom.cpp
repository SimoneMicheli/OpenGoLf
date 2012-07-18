#include <iostream>

#include "OGRoom.h"

extern int W_WIDTH;
extern int W_HEIGHT;

OGRoom* OGRoom::activeRoom=NULL;


OGRoom::OGRoom(){
    activeRoom = this;
    oldMousePos = Vector3d();
}

void OGRoom::init(){
    projection = new OGProjection();
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 150.0f);

    pov = new OGPov(5,1.80,5); //initial pov
    pov->setRotation(0, 0);

    OGLight *light0 = new OGLight(GL_LIGHT0,5,5,5,1);
    light0->setDirection(0,1,0);
    light0->set();
    lights.push_back(light0);
    light0->enable();


    //glutKeyboardFunc(OGRoom::keyPress);
    glutDisplayFunc(OGRoom::roomDisplay);
    glutPassiveMotionFunc(OGRoom::mousePassiveMotionFunction);
    glutMotionFunc(OGRoom::mouseMotionFunction);
    glutMouseFunc(OGRoom::mouseClickFunction);
}

void OGRoom::resize(int x, int y){
    W_WIDTH = x;
    W_HEIGHT = y;
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    activeRoom->projection->setAspect(aspect);
    activeRoom->projection->init();
    glutPostRedisplay();
}

//-----------------------mouse functions-----------------------
void OGRoom::mousePassiveMotionFunction(int x, int y){
     //recupera la posizione del mouse all'inizo dopo il primo movimento
    if(x<0.1*W_WIDTH || x>0.9*W_WIDTH){
        if (activeRoom->oldMousePos.x != 0 || activeRoom->oldMousePos.y != 0) {
            double a = (activeRoom->oldMousePos.y - (double) y) / 0.2;
            double b = (activeRoom->oldMousePos.x - (double) x) / 0.2;
            activeRoom->pov->addRotation(0, b); //disabilito rotazione su giu
        }

        activeRoom->oldMousePos.x = x;
        activeRoom->oldMousePos.y = y;
        glutPostRedisplay();
    }
}

void OGRoom::mouseMotionFunction(int x, int y){
    //recupera la posizione del mouse all'inizo dopo il primo movimento
    if (activeRoom->oldMousePos.x != 0 || activeRoom->oldMousePos.y != 0) {
        double a = (activeRoom->oldMousePos.y - (double) y) / 1;
        double b = (activeRoom->oldMousePos.x - (double) x) / 1;
        activeRoom->pov->addRotation(0, b); //disabilito rotazione su giu
    }

    activeRoom->oldMousePos.x = x;
    activeRoom->oldMousePos.y = y;
    glutPostRedisplay();
}



//---------------------follow display--------------------------
void OGRoom::roomDisplay(){

    float radiusBall = 2;
    glClearColor(0.0,0.5,0.0,1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    activeRoom->pov->lookAt();




    activeRoom->materialWall();



    glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(10, 0, 0);
        glVertex3d(0, 10, 0);
        glVertex3d(10, 10, 0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glNormal3f(1.0f,0.0f,0.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 10);
        glVertex3d(0, 10, 0);
        glVertex3d(0, 10, 10);
    glEnd();
    glBegin(GL_QUAD_STRIP);
         glNormal3f(0.0f,1.0f,0.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 10);
        glVertex3d(10, 0, 0);
        glVertex3d(10, 0, 10);
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0f,0.0f,-1.0f);
        glVertex3d(0, 0, 10);
        glVertex3d(10, 0, 10);
        glVertex3d(0, 10, 10);
        glVertex3d(10, 10, 10);
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glNormal3f(-1.0f,0.0f,0.0f);
        glVertex3d(10, 0, 0);
        glVertex3d(10, 0, 10);
        glVertex3d(10, 10, 0);
        glVertex3d(10, 10, 10);
    glEnd();




    glEnable(GL_LIGHTING);






    activeRoom->drawBall(1,0,1,0);


    /*glPushMatrix();
        glColor3f(0.2,0.2,0.2);
        glTranslatef(radiusBall,radiusBall,radiusBall);
        glutSolidSphere(radiusBall,70,70);
    glPopMatrix();*/

    glutSwapBuffers();
    glutPostRedisplay();
}



//---------------------mouse click--------------------

void OGRoom::mouseClickFunction(int button,int state, int x, int y){
    //glutDisplayFunc(activeRoom->OGLevel::launchDisplay);

   //nothing
}


void OGRoom::drawBall(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(TREE);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.0001, 0.0001, 0.0001);
    model->draw();
}



OGRoom::~OGRoom(){
    activeRoom = NULL;
    delete projection;
    delete pov;
    while (!lights.empty())
    {
        delete lights.back();
        lights.pop_back();
    }
}


void OGRoom::materialWall() {
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float mat_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};

    float mat_shininess = 100.0;

    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );

}
