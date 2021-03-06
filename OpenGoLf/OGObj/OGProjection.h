//
//  OGProjection.h
//  OpenGoLf
//
//  Created by Simone Micheli on 09/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGProjection_h
#define OpenGoLf_OGProjection_h

#include "OGObject.h"

enum OGProjectionType {
  PERSPECTIVE,
  ORTHO
};

class OGProjection : public OGObject {


public:
  //OGProjection();
  void setPerspective(float ,float,float,float);
  void setOrtho(float, float, float, float, float, float);
  void setAspect(float);
  void init();

private:
  OGProjectionType type;
  float aspect;
  float angle;
  float p_near;
  float p_far;
  float left;
  float top;
  float bottom;
  float right;

};

#endif
