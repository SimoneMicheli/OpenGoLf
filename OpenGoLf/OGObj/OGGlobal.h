//
//  OGGlobal.h
//  OpenGoLf
//
//  Created by Simone Micheli on 09/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenGoLf_OGGlobal_h
#define OpenGoLf_OGGlobal_h


#define W_WIDTH 900
#define W_HEIGHT 600
#define DEBUGGING false
#define BALL_DIST_X -0.8
#define BALL_DIST_Y +0.3
#define BALL_DIST_Z -0.8


#ifdef WIN32


#define TERRAIN_PATH "./terrains/terrain_small.bmp"

#else

#define TERRAIN_PATH "/Volumes/Personal/xcode/OpenGoLf/OpenGoLf/terrains/terrain_small.bmp"

#endif

//int W_WIDTH = 900;
//int W_HEIGHT = 900;


#endif
