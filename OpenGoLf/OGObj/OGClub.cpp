//
//  OGClub.cpp
//  OpenGoLf
//
//  Created by Simone Micheli on 15/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "OGClub.h"

const int OGClub::length = 3;
const std::string OGClub::e_name[]={"Driver","Wedge","Putter"};
const int OGClub::e_power[]={20,5,1};
const int OGClub::e_angle[]={45,20,5};

OGClub::OGClub(){
    c_enum = DRIVER;
    c_string = e_name[c_enum];
    c_power=e_power[c_enum];
    c_angle=e_angle[c_enum];
}

OGClub::OGClub(club c){
    c_enum = c;
    c_string = e_name[c_enum];
    c_power = e_power[c_enum];
    c_angle = e_angle[c_enum];
}

OGClub& OGClub::operator=(const OGClub& c){
    if (this != &c) {
        this->c_enum = c.c_enum;
        this->c_string = c.c_string;
        this->c_power = c.c_power;
        this->c_angle = c.c_angle;
    }
    return *this;
}

OGClub& OGClub::operator=(OGClub::club c){
    c_enum = c;
    c_string = e_name[c_enum];
    c_power=e_power[c_enum];
    c_angle=e_angle[c_enum];
    return *this;
}

OGClub& OGClub::operator++(int t) {
    c_enum = (OGClub::club)((c_enum + 1) % length);
    c_string = e_name[c_enum];
    c_power=e_power[c_enum];
    c_angle=e_angle[c_enum];
    return *this;
}

OGClub& OGClub::operator--(int t) {
    c_enum = (OGClub::club)((c_enum - 1) % length);
    c_string = e_name[c_enum];
    c_power=e_power[c_enum];
    c_angle=e_angle[c_enum];
    return *this;
}

bool OGClub::operator==(const OGClub& c) const{
    if (this->c_enum == c.c_enum) {
        return true;
    }
    return false;
}

bool OGClub::operator==(const OGClub::club c) const{
    if (this->c_enum == c) {
        return true;
    }
    return false;
}
