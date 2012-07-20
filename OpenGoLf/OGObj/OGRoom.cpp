#include <iostream>

#include "OGRoom.h"

extern int W_WIDTH;
extern int W_HEIGHT;

OGRoom* OGRoom::activeRoom=NULL;


OGRoom::OGRoom(){
    glInitNames();
    activeRoom = this;
    oldMousePos = Vector3d();


    activeRoom->loadDoor(0,0,800,0); //campo 0
    activeRoom->loadDoor(0,0,600,0); //campo 1
    activeRoom->loadDoor(0,0,400,0); //campo 2


    activeRoom->loadCabinet(0,0,100,90);
    activeRoom->loadVase(200,0,50,270);
    activeRoom->loadArmchair(400,0,70,180);
    activeRoom->loadArmchair(520,0,70,180);
    activeRoom->loadArmchair(640,0,70,180);


    activeRoom->loadLamp(30,0,970,1); //oggetto n°8
    activeRoom->loadLamp(970,0,30,1);



    modelsDL = createModelsDL();


}

GLuint OGRoom::createModelsDL(){
    //create models DisplayList
    GLuint modelsDL = glGenLists(1);
    glNewList(modelsDL, GL_COMPILE);

    for (int i=0; i<models.size(); i++) {
        printf("name: %i\n",i);
        glPushName(i);
        if(i<3){
        activeRoom->materialDoor();
        }else{
        activeRoom->materialArmchair();
        }
        //glDisable(GL_LIGHTING);
        // http://devernay.free.fr/cours/opengl/materials.html
        glColor3f(0.2125,0.1275,0.054);
        models[i]->draw();
        glEnable(GL_LIGHTING);
        glPopName();
    }

    glEndList();

    return modelsDL;
}

void OGRoom::init(){
    projection = new OGProjection();
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    projection->setPerspective(60.0f, aspect, 0.1f, 1500.0f);

    pov = new OGPov(500,180,500); //initial pov
    pov->setRotation(0, 90);

    OGLight *light0 = new OGLight(GL_LIGHT0,500,500,500,1);
    light0->setDirection(-1,0,0);
    light0->set();
    lights.push_back(light0);
    light0->enable();

    OGLight *light1 = new OGLight(GL_LIGHT1,900,900,100,1);
    light1->setDirection(0,-1,0);
    light1->set();
    lights.push_back(light1);
    light1->enable();


    glutDisplayFunc(OGRoom::roomDisplay);
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
    activeRoom->startPicking(x,y,button);
}


void OGRoom::startPicking(int x, int y, int button){
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
    gluPerspective(60.0f,aspect,0.1,1500);
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
        case 8:
            if(button == GLUT_LEFT_BUTTON){
                glEnable(GL_LIGHT0);
            }else{
                glDisable(GL_LIGHT0);
            }
            break;
        case 9:
            if(button == GLUT_LEFT_BUTTON){
                glEnable(GL_LIGHT1);
            }else{
                glDisable(GL_LIGHT1);
            }
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
    //model->setScale(0.01, 0.01, 0.01);
    model->draw();
    models.push_back(model);
}

void OGRoom::loadDoor(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(DOOR);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    //model->setScale(0.01, 0.01, 0.01);
    model->draw();
    models.push_back(model);
}

void OGRoom::loadArmchair(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(ARMCHAIR);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(3, 3, 3);
    model->draw();
    models.push_back(model);
}


void OGRoom::loadCabinet(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(CABINET);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    //model->setScale(0.01, 0.01, 0.01);
    model->draw();
    models.push_back(model);
}


void OGRoom::loadLamp(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(LAMP);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.1, 0.1, 0.1);
    model->draw();
    models.push_back(model);
}

void OGRoom::drawRoom(){
    glPushName(100);
    activeRoom->materialWall();
    glBegin(GL_QUADS);
        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(1000, 0, 0);
        glVertex3d(1000, 1000, 0);
        glVertex3d(0, 1000, 0);
        glNormal3f(1.0f,0.0f,0.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 1000);
        glVertex3d(0, 1000, 1000);
        glVertex3d(0, 1000, 0);
         glNormal3f(0.0f,1.0f,0.0f);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 1000);
        glVertex3d(1000, 0, 1000);
        glVertex3d(1000, 0, 0);
        glNormal3f(0.0f,0.0f,-1.0f);
        glVertex3d(0, 0, 1000);
        glVertex3d(1000, 0, 1000);
        glVertex3d(1000, 1000, 1000);
        glVertex3d(0, 1000, 1000);
        glNormal3f(-1.0f,0.0f,0.0f);
        glVertex3d(1000, 0, 0);
        glVertex3d(1000, 0, 1000);
        glVertex3d(1000, 1000, 1000);
        glVertex3d(1000, 1000, 0);
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

void OGRoom::materialDoor() {
    float mat_ambient[] = {0.2125f, 0.1275f, 0.054f};
    float mat_diffuse[] = {0.714f, 0.4284f, 0.18144f};
    float mat_specular[] = {0.393548f, 0.271906f, 0.166721f, 1.0f};

    float mat_shininess = 0.2;

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess*128 );

}


void OGRoom::materialArmchair() {
    float mat_ambient[] = {0.19225f, 0.19225f, 0.19225f};
    float mat_diffuse[] = {0.50754f, 0.50754f, 0.50754f};
    float mat_specular[] = {0.508273f, 0.508273f, 0.508273f, 1.0f};

    float mat_shininess = 0.4;

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess*128 );

}


void OGRoom::reinit(){
    glutDisplayFunc(OGRoom::roomDisplay);
    glutMouseFunc(OGRoom::mouseClickFunction);
    glutKeyboardFunc(OGRoom::keyPress);
    delete level;
}


void OGRoom::keyPress(unsigned char key, int x, int y){
    if (key == 'w'){
        Vector3d newPos= activeRoom->pov->getPosition()+activeRoom->pov->getDirection()*20;
        if(newPos.x<900 && newPos.x>50 && newPos.z>50 && newPos.z<900){

        activeRoom->pov->setPosition(newPos);
        }
    }
    if (key == 's'){
        Vector3d newPos= activeRoom->pov->getPosition()+activeRoom->pov->getDirection()*-20;
        if(newPos.x<900 && newPos.x>50 && newPos.z>50 && newPos.z<900){

        activeRoom->pov->setPosition(newPos);
        }
    }
    if (key == 'a'){
        activeRoom->pov->addRotation(0, -10);
    }
    if (key == 'd'){
        activeRoom->pov->addRotation(0, +10);
    }
}
