//
//  OGClub.h
//  OpenGoLf
//
//  Created by Fulvio Micheli on 04/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "OGObject.h"

#ifndef OpenGoLf_OGClub_h
#define OpenGoLf_OGClub_h

class OGClub : public OGObject{

public:

    enum club{
        DRIVER,
        WEDGE,
        PUTTER,
    };
    
    OGClub();
    OGClub(club c);
    
    OGClub& operator = (const OGClub& c);
    OGClub& operator = (club c);
    
    bool operator== (const OGClub& c) const;
    bool operator== (const OGClub::club c) const;
    
    OGClub& operator ++(int ) ;
    OGClub& operator --(int ) ;
    
    inline std::string toString() const;
    inline club getClub() const;
    inline int getPower() const;
    inline int getAngle() const;

private:
    const static int length;
    const static std::string e_name[];
    const static int e_power[];
    const static int e_angle[];

    club c_enum;
    std::string c_string;
    int c_power;
    int c_angle;

};




inline std::string OGClub::toString() const{
    return c_string;
}

inline OGClub::club OGClub::getClub() const{
    return c_enum;
}

inline int OGClub::getPower() const{
    return c_power;
}

inline int OGClub::getAngle() const{
    return c_angle;
}

#endif
