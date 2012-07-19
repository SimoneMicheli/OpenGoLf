#include <iostream>

#include "OGRoom.h"

extern int W_WIDTH;
extern int W_HEIGHT;

OGRoom* OGRoom::activeRoom=NULL;


OGRoom::OGRoom(){
    glInitNames();
    activeRoom = this;
    oldMousePos = Vector3d();


    activeRoom->loadDoor(0,0,8,0); //campo 0
    activeRoom->loadDoor(0,0,6,0); //campo 1
    activeRoom->loadDoor(0,0,4,0); //campo 2


    activeRoom->loadCabinet(0,0,1,90);
    activeRoom->loadVase(2,0,0.5,270);
    activeRoom->loadArmchair(4,0,0.7,180);
    activeRoom->loadArmchair(5.2,0,0.7,180);
    activeRoom->loadArmchair(6.4,0,0.7,180);


    activeRoom->loadLamp(0.3,0,3,1);
    activeRoom->loadLamp(0.3,0,5,1);
    activeRoom->loadLamp(0.3,0,7,1);



    activeRoom->loadDoor(2,0,10,90); //esci dal gioco

    modelsDL = createModelsDL();


}

GLuint OGRoom::createModelsDL(){
    //create models DisplayList
    GLuint modelsDL = glGenLists(1);
    glNewList(modelsDL, GL_COMPILE);

    for (int i=0; i<models.size(); i++) {
        printf("name: %i\n",i);
        glPushName(i);
        models[i]->draw();
        glPopName();
    }

    glEndList();

    return modelsDL;
}

void OGRoom::init(){
    projection = new OGProjection();
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 150.0f);

    pov = new OGPov(5,1.80,5); //initial pov
    pov->setRotation(0, 90);

    OGLight *light0 = new OGLight(GL_LIGHT0,5,5,5,1);
    light0->setDirection(-1,0,0);
    light0->set();
    lights.push_back(light0);
    light0->enable();

    OGLight *light1 = new OGLight(GL_LIGHT1,5,5,5,1);
    light1->setDirection(-1,0,0);
    light1->set();
    lights.push_back(light1);
    light1->enable();

    /*
    OGLight *light2 = new OGLight(GL_LIGHT2,0,5,0,1);
    light2->setDirection(1,-1,1);
    light2->set();
    lights.push_back(light2);
    light2->enable();
    */

    glutDisplayFunc(OGRoom::roomDisplay);
    //glutPassiveMotionFunc(OGRoom::mousePassiveMotionFunction);
    //glutMotionFunc(OGRoom::mouseMotionFunction);
    glutMouseFunc(OGRoom::mouseClickFunction);
    glutKeyboardFunc(OGRoom::keyPress);
}

void OGRoom::resize(int x, int y){
    W_WIDTH = x;
    W_HEIGHT = y;
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    activeRoom->projection->setAspect(aspect);
    activeRoom->projection->init();
    glutPostRedisplay();
}



void OGRoom::mouseMotionFunction(int x, int y){
    if (activeRoom->oldMousePos.x != 0 || activeRoom->oldMousePos.y != 0) {
        double b = (activeRoom->oldMousePos.x - (double) x) / 1.5;
        activeRoom->pov->addRotation(0, -b); //disabilito rotazione su giu
    }

    activeRoom->oldMousePos.x = x;
    activeRoom->oldMousePos.y = y;
    glutPostRedisplay();
}

void OGRoom::mouseClickFunction(int button, int status, int x, int y){
    activeRoom->startPicking(x,y);
}


void OGRoom::startPicking(int x, int y){
    GLint hits;
    GLint viewport[4];
    GLuint selectBuf[BUFSIZE];

    glSelectBuffer(BUFSIZE,selectBuf);
    glGetIntegerv(GL_VIEWPORT,viewport);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    system("cls");
    gluPickMatrix(x-1,viewport[3]-y-1,2,2,viewport);
    //projection->init();
    extern int W_WIDTH;
    extern int W_HEIGHT;
    float aspect = (float)viewport[2]/(float)viewport[3];
    gluPerspective(60.0f,aspect,0.1,150);
    printf("w %i h:%i %i %i %f\n",viewport[2],viewport[3],W_WIDTH,W_HEIGHT,aspect);
    glMatrixMode(GL_MODELVIEW);
    roomDisplay();



    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);

    printf("hits: %i\n",hits);
    int max=-1,name=-1;
    for (int i = 0; i < hits; i++){
 		printf(	"Number: %d\n"
 				"Min Z: %d\n"
 				"Max Z: %d\n"
 				"Name on stack: %i\n",
 				(GLubyte)selectBuf[i * 4],
 				(GLubyte)selectBuf[i * 4 + 1],
 				(GLubyte)selectBuf[i * 4 + 2],
 				(GLuint)selectBuf[i * 4 + 3]
 				);

        if(max <= (GLubyte)selectBuf[i * 4 + 1]){
            max = (GLubyte)selectBuf[i * 4 + 1];
            name = (GLubyte)selectBuf[i * 4 + 3];
        }
    }
 	printf("\n");
    printf("max: %i name: %i",max,name);

    glMatrixMode(GL_MODELVIEW);


    switch(name){
        case 0:
            level = new OGLevel();
            level->init(TERRAIN_PATH,MODELS);

            glutPostRedisplay();
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }

}


//---------------------room display--------------------------
void OGRoom::roomDisplay(){
    //glInitNames();
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    activeRoom->pov->lookAt();
    glEnable(GL_LIGHTING);

    activeRoom->drawRoom();
    glCallList(activeRoom->modelsDL);

    glutSwapBuffers();
    glutPostRedisplay();
}



//---------------------mouse click--------------------



void OGRoom::loadVase(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(VASE);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.01, 0.01, 0.01);
    model->draw();
    models.push_back(model);
}

void OGRoom::loadDoor(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(DOOR);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.01, 0.01, 0.01);
    model->draw();
    models.push_back(model);
}

void OGRoom::loadArmchair(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(ARMCHAIR);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.03, 0.03, 0.03);
    model->draw();
    models.push_back(model);
}


void OGRoom::loadCabinet(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(CABINET);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.01, 0.01, 0.01);
    model->draw();
    models.push_back(model);
}


void OGRoom::loadLamp(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(LAMP);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.001, 0.001, 0.001);
    model->draw();
    models.push_back(model);
}

void OGRoom::drawRoom(){
    glPushName(100);
    activeRoom->materialWall();
    glBegin(GL_QUADS);
        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(10, 0, 0);
        glVertex3d(10, 10, 0);
        glVertex3d(0, 10, 0);
        glNormal3f(1.0f,0.0f,0.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 10);
        glVertex3d(0, 10, 10);
        glVertex3d(0, 10, 0);
         glNormal3f(0.0f,1.0f,0.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 10);
        glVertex3d(10, 0, 10);
        glVertex3d(10, 0, 0);
        glNormal3f(0.0f,0.0f,-1.0f);
        glVertex3d(0, 0, 10);
        glVertex3d(10, 0, 10);
        glVertex3d(10, 10, 10);
        glVertex3d(0, 10, 10);
        glNormal3f(-1.0f,0.0f,0.0f);
        glVertex3d(10, 0, 0);
        glVertex3d(10, 0, 10);
        glVertex3d(10, 10, 10);
        glVertex3d(10, 10, 0);
    glEnd();
    glPopName();
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
    float mat_diffuse[] = {0.8f, 0.8f, 1.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};

    float mat_shininess = 100.0;

    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );

}


void OGRoom::keyPress(unsigned char key, int x, int y){


    if (key == 'w'){
        Vector3d newPos= activeRoom->pov->getPosition()+activeRoom->pov->getDirection()*0.2;
        if(newPos.x<9.8 && newPos.x>0.2 && newPos.z>0.2 && newPos.z<9.8){

        activeRoom->pov->setPosition(newPos);
        }
    }
    if (key == 's'){
        Vector3d newPos= activeRoom->pov->getPosition()+activeRoom->pov->getDirection()*-0.2;
        if(newPos.x<9.8 && newPos.x>0.2 && newPos.z>0.2 && newPos.z<9.8){

        activeRoom->pov->setPosition(newPos);
        }
    }
}
