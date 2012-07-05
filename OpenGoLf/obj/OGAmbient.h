//
//  OGAmbient.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGAmbient_h
#define OpenGoLf_OGAmbient_h

#include "OGPov.h"
#include "OGTerrain.h"

class OGAmbient{
public:
    
    OGPov* pov; //
    
    /*virtual void init();
    virtual void display();
    virtual void keyBoardFunction(unsigned int, int, int);
    virtual void mouseFunction(int, int, int, int); //pressione tasto mouse
    virtual void mouseMotionFunction(int, int); //movimento mouse con tasti premuti*/
    void static mousePassiveMotionFunction(int, int); //movimento mouse senza tasti premuti
    
};


#endif
