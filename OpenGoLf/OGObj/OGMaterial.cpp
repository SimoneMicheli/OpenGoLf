//
//  OGMaterial.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 17/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGMaterial.h"

OGMaterial::OGMaterial(float ambient[],float diffuse[],float specular[],float emission[], float shininess[]){
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emission = emission;
    this->shininess = shininess;
    init();
}

OGMaterial::OGMaterial(){
    ambient = new float[4]{0.2,0.2,0.2,1.0};
    diffuse = new float[4]{0.8,0.8,0.8,1.0};
    specular = new float[4]{0.0,0.0,0.0,1.0};
    emission = new float[4]{0.0,0.0,0.0,1.0};
    shininess = new float[1]{0};
    init();
}

void OGMaterial::init(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}