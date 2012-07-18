//
//  Util.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 13/07/12.
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
typedef uint8_t  CHAR;
typedef uint16_t WORD;
typedef uint32_t DWORD;

typedef int8_t  BYTE;
typedef int16_t SHORT;
typedef int32_t LONG;
#endif

#include "Vector3d.h"
#include "OGObject.h"
#include "OGLevel.h"

extern int W_WIDTH;
extern int W_HEIGHT;

void renderString(const char* string)
{ 
    //save modelview
    glPushMatrix();
    glLoadIdentity();
    
    //save projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, W_WIDTH, 0, W_HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glColor3f(0, 0, 0);
    glRasterPos2i(5,W_HEIGHT - 20);
    
    for (int i = 0; i < strlen(string); i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
    
    //restore projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    //restore modelview
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void strokeString(const char* string){
    for (int i = 0; i < strlen(string); i++) {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, string[i]);
    }
}