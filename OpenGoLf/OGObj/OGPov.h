#ifndef OpenGoLf_OGPov_h
#define OpenGoLf_OGPov_h

#include "OGObject.h"
#include "Vector3d.h"
#include <math.h>


class OGPov : public OGObject{
    
private:
    double alpha, beta; 
    Vector3d pos,look;
    double mod = 1; //modulo del vettore di vista
    
public:
    OGPov();
    OGPov(double,double,double);
    OGPov(Vector3d);
    void setPosition(double, double, double);
    void setPosition(Vector3d);
    Vector3d getPosition();
    void setRotation(double,double);
    void addRotation(double,double);
    //imposta la pov corrente
    void lookAt();
};


#endif
