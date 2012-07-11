//
//  OGPhysic.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 11/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGPhysic.h"

OGPhysic::OGPhysic(OGBall* ball, OGTerrain* terrain){
    viscosity = 0.02;
    friction = 0.03;
    gravity = -9.81;
    elasticity = 0.6;
    srand ( time(NULL) );
    const int MAX_WIND = 50;
    wind = Vector3d((rand() % MAX_WIND),0,(rand() % MAX_WIND));
    wind = Vector3d();
    this->ball = ball;
    this->terrain = terrain;
}

void OGPhysic::update(double time){
    
    
    
    
    
    //printf("time:%f\n",time);
    Vector3d fp = Vector3d(0,ball->getMass() * gravity,0); //forza peso
    Vector3d faa = 6 * M_PI * viscosity * ball->getMass() * (wind - ball->getSpeed()); //forza attrito aria
    Vector3d fr = fp + faa; //forza attiva sulla pallina
     
    //printf("fp x:%f y:%f z:%f\n",fp.x,fp.y,fp.z);
    //printf("faa x:%f y:%f z:%f\n",faa.x,faa.y,faa.z);
    
    Vector3d terrainNorm;
    bool collision = terrainCollision(terrainNorm);
    
    if (collision) {
        Vector3d normV = ball->getSpeed().getNormalized();
        Vector3d fat = -normV * (-fp.y * friction); //attrito radente terreno
        fr += fat;
        //terrainNorm = Vector3d(0,1,0);
        fr = fr - (2 * terrainNorm * (terrainNorm.dot(fr)));
    }
    
    //printf("fr x:%f y:%f z:%f\n",fr.x,fr.y,fr.z);
    Vector3d acc = fr / ball->getMass();
    
        
    //printf("acc x:%f y:%f z:%f\n",acc.x,acc.y,acc.z);
    ball->setSpeed(ball->getSpeed() + (acc * time));
    ball->setPosition(ball->getPosition() + (ball->getSpeed() * time));
}

bool OGPhysic::terrainCollision(Vector3d &normal){
    Vector3d newPos = ball->getPosition()*terrain->getHScale(); //posizione nell'immagine scalata
    
    Vector3d v1 = terrain->vertex[(int)round(newPos.z * terrain->getTerrainHeight() + newPos.x)];
    Vector3d v2 = terrain->vertex[(int)round(newPos.z * terrain->getTerrainHeight() + newPos.x + 1)];
    Vector3d v3 = terrain->vertex[(int)round((newPos.z +1) * terrain->getTerrainHeight() + newPos.x)];
    
    Vector3d center = (v1+v2+v3)/3;
    double distance = (ball->getPosition().y - center.y);
    if(distance<ball->getRadius()){
        Vector3d n1 = terrain->normals[(int)round(newPos.z * terrain->getTerrainHeight() + newPos.x)];
        Vector3d n2 = terrain->normals[(int)round(newPos.z * terrain->getTerrainHeight() + newPos.x + 1)];
        Vector3d n3 = terrain->normals[(int)round((newPos.z +1) * terrain->getTerrainHeight() + newPos.x)];
        normal = (n1).getNormalized(); //
        return true;
    }else{
        normal = Vector3d();
        return false;
    }
    
    
}

void OGPhysic::shoot(float power,Vector3d direction){
    direction.normalize();
    ball->setSpeed(direction * power);
}