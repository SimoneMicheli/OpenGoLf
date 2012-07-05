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
#include "OGAmbient.h"
#include "OGRoom.h"
#include "OGLevel.h"
#include "OGTerrain.h"

#define W_WIDTH 800
#define W_HEIGHT  600

/*int main(int argc, const char * argv[]){

    
}*/

//----------global var---------------------
OGLevel *level;
//-----------------------------------------

void init();
void display();

int main(int argc, char** argv){
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    init();
    
    glutDisplayFunc(display);
    //glutReshapeFunc(resize);
    //glutKeyboardFunc(getKey);
    //glutMotionFunc(mouseMove);
    //glutMouseFunc(mouseClick);
    
    //gettimeofday(&before , NULL);
    //ti = before;
    glutMainLoop();
    
    return 0;
    
}

void init(){
    
    // enable depth testing
	glEnable(GL_DEPTH_TEST);
    
    //enable lghting
    //glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
	
    //set perspective mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();               //reset view
    GLfloat aspect = (GLfloat) W_WIDTH /(GLfloat)W_HEIGHT;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);    
    glEnable(GL_LIGHT0);
    
    //anable wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    //-----------------------------------------------------
    level = new OGLevel();
    level->init("/Volumes/Personal/xcode/terreno/terrain_small.bmp");
    glutPassiveMotionFunc(OGLevel::wrapperMousePassiveMotionFunction);
    
}

void display(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    

    
    glColor3b(0,0,0);

    level->draw();
    
    glutSwapBuffers();
}
