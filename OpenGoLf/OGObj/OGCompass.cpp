//
//  OGCompass.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 15/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGCompass.h"

OGCompass::OGCompass(int x, int y, int width, int height){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    
    circleDL = createCircleDL(0.3, 10);
    arrowDL = createArrowDL();
    
}

GLuint OGCompass::createCircleDL(float r, int num_segments) 
{ 
	float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tanf(theta); 
    
	float radial_factor = cosf(theta);
	
	float x = r;
    
	float y = 0; 
    
    GLuint list = glGenLists(1);
    
    glNewList(list, GL_COMPILE);
    
	glBegin(GL_TRIANGLE_FAN);
    glVertex2d(0, 0);
	for(int ii = 0; ii <= num_segments; ii++) 
	{ 
		glVertex2f(x , y);
        
		float tx = -y; 
		float ty = x; 
        
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd();
    
    glEndList();
    
    return list;
}

GLuint OGCompass::createArrowDL(){
    GLuint list = glGenLists(1);
    
    glNewList(list, GL_COMPILE);
    
    glBegin(GL_TRIANGLES);
    
    glColor3f(1, 0, 0);
    glVertex2d(-0.5,0);
    glVertex2d(0,2);
    glVertex2d(0.5,0);
    glColor3f(0,0,1);
    glVertex2d(-0.5,0);
    glVertex2d(0,-2);
    glVertex2d(0.5,0);
    
    glEnd();
    
    glEndList();
    
    return list;
}


void OGCompass::drawWind(Vector3d v){
    float angle = (180 * - v.beta() / M_PI) - 90;
    
    GLdouble modelMatrix[16], projMatrix[16];
    
    //save opengl state
    glGetBooleanv(GL_DEPTH_TEST, &depth);
    glGetBooleanv(GL_LIGHTING, &light);
    glGetBooleanv(GL_TEXTURE_2D, &texture);
    
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(x, y, width, height);
    glOrtho(0, 10, 10, 0, 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //disable opengl state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    //draw text
    glColor3f(1, 1, 1);
    glRasterPos2f(4.2,9.5);
    
    char *string="Wind";
    for (int i = 0; i < strlen(string); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    
    //center and rotate
    glTranslated(5, 5, 0);
    glRotated(180, 0, 0, 1);
    
    //draw arrow
    glPushMatrix();
    glRotated(angle, 0, 0, 1);
    glCallList(arrowDL);
    glPopMatrix();
    
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

void OGCompass::drawMap(float angle){
    angle = (180 * angle / M_PI) - 90;
    
    GLdouble modelMatrix[16], projMatrix[16];
    
    //save opengl state
    glGetBooleanv(GL_DEPTH_TEST, &depth);
    glGetBooleanv(GL_LIGHTING, &light);
    glGetBooleanv(GL_TEXTURE_2D, &texture);
    
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(x, y, width, height);
    glOrtho(0, 10, 10, 0, 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //disable opengl state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    //draw text
    glColor3f(1, 1, 1);
    glRasterPos2f(4.0,9.5);
    
    char *string="Map";
    for (int i = 0; i < strlen(string); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    
    //center and rotate
    glTranslated(5, 5, 0);
    glRotated(180, 0, 0, 1);
    
    //draw arrow
    glCallList(arrowDL);
    
    //drow circle
    glRotated(angle, 0, 0, 1);
    glTranslated(0, 3, 0);
    glColor3f(1,1,0);
    
    glCallList(circleDL);
    
    
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