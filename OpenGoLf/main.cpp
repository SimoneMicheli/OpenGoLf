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

#include "OGLevel.h"


//----------global var---------------------
OGLevel *level;
int W_WIDTH = 900;
int W_HEIGHT = 800;
int pippo = 0;
//-----------------------------------------

void init();

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);

    init();

    glutReshapeFunc(OGLevel::resize);
    glutKeyboardFunc(OGLevel::keyPress);
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
    //glEnable(GL_FOG);                   // Enables GL_FOG

    //enable wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    //-----------------------------------------------------
    level = new OGLevel();
    level->init(TERRAIN_PATH,MODELS);
}

