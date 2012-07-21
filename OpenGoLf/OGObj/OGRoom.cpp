#include <iostream>

#include "OGRoom.h"

extern int W_WIDTH;
extern int W_HEIGHT;

OGRoom* OGRoom::activeRoom=NULL;


OGRoom::OGRoom(){
    glInitNames();
    activeRoom = this;
    oldMousePos = Vector3d();
    activeRoom->wind = false;
    activeRoom->fog = false;

    activeRoom->loadDoor(0,0,800,0); //campo 0
    activeRoom->loadDoor(0,0,600,0); //campo 1
    activeRoom->loadDoor(0,0,400,0); //campo 2


    activeRoom->loadDoor(300,0,1000,90); //uscita 3
    activeRoom->loadVase(200,0,50,0); //vaso 4
    activeRoom->loadArmchair(400,0,70,0);
    activeRoom->loadArmchair(520,0,70,0);

    activeRoom->loadVase(720,0,50,0); //vaso 7

    activeRoom->loadLamp(100,300,1000,0); //vaso 7




    modelsDL = createModelsDL();

    loading = false;
}

GLuint OGRoom::createModelsDL(){
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    //create models DisplayList
    GLuint modelsDL = glGenLists(1);
    glNewList(modelsDL, GL_COMPILE);

    for (int i=0; i<models.size(); i++) {
        //printf("name: %i\n",i);
        glPushName(i);
        activeRoom->materialArmchair(); //predefinito
        if(i<4){
        activeRoom->materialDoor();
        }
        if(i==4 || i==7){ //vaso
            activeRoom->materialVase();
        }
        if(i==5){ //poltrona vento
            activeRoom->materialArmchair();
        }
        if(i==6){ //poltrona nebbia
            activeRoom->materialArmchair();
        }
        if(i==8 || i==9){
            activeRoom->materialVase();
        }
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
    glutReshapeFunc(OGRoom::resize);
}

void OGRoom::resize(int x, int y){
    W_WIDTH = x;
    W_HEIGHT = y;
    float aspect = (float) W_WIDTH/(float) W_HEIGHT;
    activeRoom->projection->setAspect(aspect);
    activeRoom->projection->init();
    glutPostRedisplay();
}

void OGRoom::mouseClickFunction(int button, int status, int x, int y){
    if (button == GLUT_LEFT_BUTTON && status == GLUT_UP) {
        activeRoom->startPicking(x,y);
    }
}


void OGRoom::startPicking(int x, int y){
    GLint hits;
    GLint viewport[4];
    GLuint selectBuf[BUFSIZE];

    glGetDoublev(GL_PROJECTION_MATRIX, projMat);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);

    glSelectBuffer(BUFSIZE,selectBuf);
    glGetIntegerv(GL_VIEWPORT,viewport);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(x-1,viewport[3]-y-1,2,2,viewport);
    float aspect = (float)viewport[2]/(float)viewport[3];
    gluPerspective(60.0f,aspect,0.1,1500);
    glMatrixMode(GL_MODELVIEW);
    roomDisplay();



    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);

    int max=-1,name=-1;
    for (int i = 0; i < hits; i++){

        if(max <= (GLubyte)selectBuf[i * 4 + 1]){
            max = (GLubyte)selectBuf[i * 4 + 1];
            name = (GLubyte)selectBuf[i * 4 + 3];
        }
    }
    glMatrixMode(GL_MODELVIEW);

    printf("Clicked name: %i\n",name);
    switch(name){
        case 0:{
            //load level 0
            level = new OGLevel();
            level->init(TERRAIN_PATH,MODELS);
            glutPostRedisplay();
            break;
        }
        case 1:
            //load level 1
            level = new OGLevel();
            level->init(TERRAIN_PATH,MODELS);
            glutPostRedisplay();
            break;
        case 2:
            //load level 2
            level = new OGLevel();
            level->init(TERRAIN_PATH,MODELS);
            glutPostRedisplay();
            break;
        case 3:
            exit(0);
            break;
        case 5:
            activeRoom->wind=!activeRoom->wind;
            printf("wind: %s\n",(activeRoom->wind)?"true":"false");
            glutPostRedisplay();
            break;
        case 6:
            activeRoom->fog=!activeRoom->fog;
            printf("fog: %s\n",(activeRoom->fog)?"true":"false");
            glutPostRedisplay();
            break;
        case 8:{
            GLboolean status;
            glGetBooleanv(GL_LIGHT0, &status);
            if(!status){
                glEnable(GL_LIGHT0);
            }else{
                glDisable(GL_LIGHT0);
            }
            glutPostRedisplay();
            break;
        }
        case 9:{
            GLboolean status;
            glGetBooleanv(GL_LIGHT1, &status);
            if(!status){
                glEnable(GL_LIGHT1);
            }else{
                glDisable(GL_LIGHT1);
            }
            glutPostRedisplay();
            break;
        }
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

    printf("passo 1\n");
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    activeRoom->pov->lookAt();
    glEnable(GL_LIGHTING);

    //double x = activeRoom->pov->getDirection().x;
    //double y = activeRoom->pov->getDirection().y;
    //double z = activeRoom->pov->getDirection().z;
    //printf("po: x:%f y:%f z:%f\n",x,y,z);
    //printf("int: ",activeRoom->pov->getPosition(),activeRoom->pov->getDirection());
    activeRoom->drawRoom();

    printf("passo 2\n");
    glCallList(activeRoom->modelsDL);

    printf("passo 3\n");

    GLint type;
    glGetIntegerv(GL_RENDER_MODE, &type);

    if (type == GL_RENDER){
        if(activeRoom->wind){
            if(activeRoom->fog){
                renderString("Vento: SI    Nebbia: SI");
            }else{
                renderString("Vento: SI    Nebbia: NO");
            }
        }else{
            if(activeRoom->fog){
                renderString("Vento: NO    Nebbia: SI");
            }else{
                renderString("Vento: NO    Nebbia: NO");
            }
        }
    }

    printf("passo 4\n");
    glutSwapBuffers();
}

void OGRoom::printLoading(){
    if (loading) {
        glDisable(GL_DEPTH_TEST);
        glPushMatrix();
        Vector3d pos = activeRoom->pov->getPosition() + (activeRoom->pov->getDirection() * 400);
        glTranslated(pos.x, 180, pos.z);
        glLineWidth(5);
        glScalef(0.5, 0.5, 0.5);
        double angle = activeRoom->pov->getBeta() * 180 / M_PI;
        glRotatef(360 - angle - 90, 0, 1, 0);
        glTranslatef(-350, 0, 0);
        strokeString("LOADING");
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);
    }
}

void OGRoom::loadVase(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(VASE);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation2(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    models.push_back(model);
}

void OGRoom::loadDoor(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(DOOR);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation2(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    models.push_back(model);
}

void OGRoom::loadArmchair(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(ARMCHAIR);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation2(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(3, 3, 3);
    models.push_back(model);
}




void OGRoom::loadLamp(double x, double y, double z, double angle){

    OGModel3DS *model = new OGModel3DS(LAMP);
    Vector3d pos = Vector3d(x, y, z);
    model->setPosition(pos);
    model->setRotation2(angle, 0, 1, 0);
    model->setRotation(-90, 1, 0, 0);
    model->setScale(0.3, 0.3, 0.3);
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
    float mat_ambient[] = {0.1f, 0.18725f, 0.1745f};
    float mat_diffuse[] = {0.396f, 0.74151f, 0.69102f};
    float mat_specular[] = {0.297254f, 0.30829f, 0.306678f, 1.0f};

    float mat_shininess = 0.1;

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess*128 );

}


void OGRoom::materialVase() {
    float mat_ambient[] = {0.24725f, 0.1995f, 0.0745f};
    float mat_diffuse[] = {0.75164f, 0.60648f, 0.22648f};
    float mat_specular[] = {0.628281f, 0.555802f, 0.366065f, 1.0f};

    float mat_shininess = 0.4;

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess*128 );

}

void OGRoom::materialLamp() {
    float mat_ambient[] = {0.25f, 0.20725f, 0.20725f};
    float mat_diffuse[] = {1.0f, 0.829f, 0.829f};
    float mat_specular[] = {0.296648f, 0.296648f, 0.296648f, 1.0f};

    float mat_shininess = 0.088;

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialf ( GL_FRONT, GL_SHININESS, mat_shininess*128 );

}

void OGRoom::materialArmchairActived() {
    float mat_ambient[] = {0.9, 0.9, 0.9};
    float mat_diffuse[] = {0.50754f, 0.50754f, 0.50754f};
    float mat_specular[] = {0.508273f, 0.508273f, 0.508273f, 1.0f};

    float mat_shininess = 0.4;

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess*128 );

}

void OGRoom::reInit(){


    glutDisplayFunc(OGRoom::roomDisplay);
    glutMouseFunc(OGRoom::mouseClickFunction);
    glutPassiveMotionFunc(NULL);
    glutMotionFunc(NULL);
    glutKeyboardFunc(OGRoom::keyPress);
    glutReshapeFunc(OGRoom::resize);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(projMat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(modelMat);

    delete level;
    init();

    glutPostRedisplay();
}


void OGRoom::keyPress(unsigned char key, int x, int y){
    if (key == 'w'){
        Vector3d newPos= activeRoom->pov->getPosition()+activeRoom->pov->getDirection()*10;
        if(newPos.x<900 && newPos.x>50 && newPos.z>50 && newPos.z<900){

        activeRoom->pov->setPosition(newPos);
        }
    }
    if (key == 's'){
        Vector3d newPos= activeRoom->pov->getPosition()+activeRoom->pov->getDirection()*-10;
        if(newPos.x<900 && newPos.x>50 && newPos.z>50 && newPos.z<900){

        activeRoom->pov->setPosition(newPos);
        }
    }
    if (key == 'a'){
        activeRoom->pov->addRotation(0, -5);
    }
    if (key == 'd'){
        activeRoom->pov->addRotation(0, +5);
    }
    glutPostRedisplay();
}
