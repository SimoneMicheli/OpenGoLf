//
//  OGGlobal.h
//  OpenGoLf
//
//  Created by Simone Micheli on 09/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGGlobal_h
#define OpenGoLf_OGGlobal_h


#define DEBUGGING false
#define BALL_DIST_X -0.8
#define BALL_DIST_Y +0.3
#define BALL_DIST_Z -0.8


#ifdef WIN32


#define TERRAIN_PATH "./terrains/terrain_small.bmp"
#define TREE "./models/tree.3ds"
#define BUSH "./models/bush.3ds"
#define MODELS "./models/terrain0.dat"
#define STEP 0.1 //step per la barra di caricamento



//object in room
#define VASE "./models/vase.3ds"
#define DOOR "./models/door.3ds"
#define ARMCHAIR "./models/armchair.3ds"
#define CABINET "./models/cabinet.3ds"
#define LAMP "./models/lamp.3ds"




#else

#define TREE  "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/tree.3ds"
#define BUSH  "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/bush.3ds"
#define MODELS "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/terrain0.dat"
#define TERRAIN_PATH "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/terrains/terrain_small.bmp"
#define STEP 0.01 //step per la barra di caricamento

#define VASE "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/vase.3ds"
#define DOOR "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/door.3ds"
#define ARMCHAIR "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/armchair.3ds"
#define CABINET "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/models/cabinet.3ds"



#endif


#endif
