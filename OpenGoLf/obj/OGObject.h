//
//  GLObj.h
//  OpenGoLf
//
//  Created by Simone Micheli on 06/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_GLObj_h
#define OpenGoLf_GLObj_h

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
//#include "OGGlobal.h"

class OGObject{
    
    
};


#endif
