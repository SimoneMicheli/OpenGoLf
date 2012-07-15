//
//  main.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#ifdef WIN32

#include <GL/gl.h>
#include <GL/glu.h>

#include <GL/glut.h>

#else

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <GLUT/glut.h>
#endif

#include "Vector3d.h"
//#include "OGAmbient.h"
//#include "OGRoom.h"
#include "OGLevel.h"
//#include "OGTerrain.h"
//#include "OGGlobal.h"
//#include "OGObject.h"

/*int main(int argc, const char * argv[]){

    
}*/

//----------global var---------------------
OGLevel *level;
//extern int W_WIDTH;
//extern int W_HEIGHT;
//-----------------------------------------

void init();
void resize(int x, int y);

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    init();
    
    //glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(OGLevel::keyPress);
    //glutMotionFunc(mouseMove);
    //glutMouseFunc(mouseClick);
    
    glutDisplayFunc(OGLevel::launchDisplay);
    glutPassiveMotionFunc(OGLevel::mousePassiveMotionFunction);
    glutMouseFunc(OGLevel::mouseClickFunction);
    
    if (DEBUGGING)
     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //enable wireframe
    
    glutMainLoop();
    
    return 0;
    
}

void init(){
    
    // enable depth testing
	glEnable(GL_DEPTH_TEST);
    
    //enable lghting
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
	
    //enable wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    //-----------------------------------------------------
    level = new OGLevel();
    level->init(TERRAIN_PATH);
}

void resize(int x, int y){
    
    //W_WIDTH = x;
    //W_HEIGHT = y;
    glutPostRedisplay();
}
