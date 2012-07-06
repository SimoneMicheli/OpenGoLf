//
//  OGLevel.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGLevel_h
#define OpenGoLf_OGLevel_h

#include "OGAmbient.h"
#include <string.h>
#include <stdlib.h>
#include "OGLight.h"
#include <vector>

class OGLevel : public OGAmbient {
    
public:
    
    OGLevel();
    void init(string);
    void draw();
    
    void mousePassiveMotionFunction(int, int);
    void static wrapperMousePassiveMotionFunction(int, int);
    
private:
    static OGLevel* activeLevel;
    OGTerrain *terrain;
    vector<OGLight*> lights;
    
    Vector3d oldMousePos;
};


#endif
