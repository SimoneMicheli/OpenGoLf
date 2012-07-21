//
//  OGPhysic.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 11/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGPhysic.h"

OGPhysic::OGPhysic(OGBall* ball, OGTerrain* terrain, OGPov* pov){
    viscosity = 0.02;
    friction = 0.4;
    gravity = -9.81;
    elasticity = 0.5;
    srand ( time(NULL) );
    const int MAX_WIND = 10;
    if (OGRoom::activeRoom->getWindStatus())
        wind = Vector3d((rand() % MAX_WIND) - (MAX_WIND /2),0,(rand() % MAX_WIND) - (MAX_WIND / 2));
    else
        wind = Vector3d();
        
    this->ball = ball;
    this->terrain = terrain;
    this->pov = pov;
}

void OGPhysic::update(double time){     
    
    Vector3d terrainNorm;
    Vector3d vertex;
    Vector3d pos;
    bool collision = terrainCollision(vertex, terrainNorm);
    
    Vector3d fp = Vector3d(0,ball->getMass() * gravity,0); //forza peso
    
    if (!collision){
        Vector3d faa = 6 * M_PI * viscosity * ball->getMass() * (wind - ball->getSpeed()); //forza attrito aria
        Vector3d fr = fp + faa; //forza attiva sulla pallina
        Vector3d acc = fr / ball->getMass();
        ball->setSpeed(ball->getSpeed() + (acc * time));
        pos = ball->getPosition() + (ball->getSpeed() * time);
    }else{
        //attrito radente
        Vector3d normV = ball->getSpeed().getNormalized();
        Vector3d fat = normV * terrainNorm.dot(fp) * friction;
        
        //calcolo forza peso parallela
        Vector3d fpp = fp.length() * terrainNorm + Vector3d(0,-1,0);
        Vector3d acc = fat + fpp / ball->getMass();
        
        Vector3d speed = ball->getSpeed();
        pos = ball->getPosition();
        
        speed = Vector3d(speed.x + (acc.x * time), -speed.y * elasticity, speed.z + (acc.z * time));
        
        pos = Vector3d(pos.x + (speed.x * time), vertex.y + (speed.y * time) ,pos.z + (speed.z * time));
        
        ball->setSpeed(speed);
    }
    
    
    
    ball->setPosition(pos.x, pos.y, pos.z);
    //update pov
    pov->setPosition(pos.x, pos.y , pos.z);

}

bool OGPhysic::terrainCollision(Vector3d &vertex, Vector3d &normal){
    Vector3d newPos = ball->getPosition() * terrain->getHScale(); //posizione nell'immagine scalata
    
    Vector3d v1 = terrain->vertex[(int)(floor(newPos.z) * (int)terrain->getTerrainWidth() + floor(newPos.x))];
    Vector3d v2 = terrain->vertex[(int)(floor(newPos.z) * (int)terrain->getTerrainWidth() + floor(newPos.x + 1))];
    Vector3d v3 = terrain->vertex[(int)(floor(newPos.z +1) * (int)terrain->getTerrainWidth() + floor(newPos.x))];
    
    Vector3d center = (v1+v2+v3)/3;
    double distance = (ball->getPosition().y - center.y);
    
    if(distance<ball->getRadius()){
        Vector3d n1 = terrain->normals[(int)(round(newPos.z) * terrain->getTerrainWidth() + round(newPos.x))];
        Vector3d n2 = terrain->normals[(int)(round(newPos.z) * terrain->getTerrainWidth() + round(newPos.x + 1))];
        Vector3d n3 = terrain->normals[(int)(round(newPos.z +1) * terrain->getTerrainWidth() + round(newPos.x))];
        normal = (n1+n2+n3).getNormalized(); //
        vertex = center;
        return true;
        
    }else{
        normal = Vector3d();
        return false;
    }
}

bool OGPhysic::edgeCollision(){
    return terrainEdge() || waterCollision();
}

bool OGPhysic::terrainEdge(){
    Vector3d position = ball->getPosition() * terrain->getHScale();
    unsigned int width = terrain->getTerrainWidth();
    unsigned int height = terrain->getTerrainHeight();
    
    if (position.x > width || position.z > height || position.x< 0 || position.z < 0) {
        return true;
    }
    
    return false;
    
}

bool OGPhysic::waterCollision(){
    float yOffset = terrain->getYOffset();
    if (ball->getPosition().y - ball->getRadius() < yOffset) {
        return true;
    }
    return false;
}

bool OGPhysic::holeCollision(){
    OGHole hole = terrain->getHole();
    Vector3d pos = ball->getPosition();
    bool dist = (pow(pos.x - hole.x,2) + pow(pos.z - hole.z , 2)) <= pow(hole.radius, 2);    
    if (pos.y - ball->getRadius() < hole.y +0.2 && dist) {
        return true;
    }
    return false;
}

Vector3d OGPhysic::getWind(){
    return wind;
}

void OGPhysic::shoot(float power, int angle){
    Vector3d direction = pov->getDirection();
    direction.y = sin(angle);
    direction = direction.getNormalized() * power;
    Vector3d p = ball->getPosition();
    ball->setSpeed(0,0,0);
    ball->setPosition(p.x, p.y + ball->getRadius() + 0.01, p.z);
    ball->setSpeed(direction.x, direction.y, direction.z);
    
}