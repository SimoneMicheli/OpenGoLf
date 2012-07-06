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
    float ambient[3],diffuse[3],specular[3],emission[3],shininess;
    
    void init();
    
private:
}

#endif
