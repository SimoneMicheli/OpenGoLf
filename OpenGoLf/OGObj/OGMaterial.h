//
//  OGMaterial.h
//  OpenGoLf
//
//  Created by Simone Micheli on 06/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGMaterial_h
#define OpenGoLf_OGMaterial_h

#include "OGObject.h"

class OGMaterial : public OGObject{
    
public:
    float *ambient,*diffuse,*specular,*emission;
    float *shininess;
    
    OGMaterial();
    OGMaterial(float ambient[],float diffuse[],float specular[],float emission[], float shininess[]);
    void init();
    
private:
};

#endif
