#include <iostream>

#include "OGLevel.h"

OGLevel* OGLevel::activeLevel=NULL;

OGLevel::OGLevel(){
    activeLevel = this;
    oldMousePos = Vector3d();
}

void OGLevel::init(string path){
    terrain = new OGTerrain(path);
    pov = new OGPov(0,5,0); //initial pov
    pov->setRotation(-30, -45);
}

//-----------------------mouse functions-----------------------

void OGLevel::wrapperMousePassiveMotionFunction(int x, int y){
    activeLevel->mousePassiveMotionFunction(x,y);
}

void OGLevel::mousePassiveMotionFunction(int x, int y){
     std::cout<<oldMousePos.x<<"/"<<oldMousePos.y<<"\n";
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

void OGLevel::draw(){
    pov->lookAt();
    terrain->draw();
    
}