#include "OGObject.h"
#include "OGProjection.h"
#include "OGTerrain.h"
#include "OGPov.h"
#include "Vector3d.h"


class OGSubview{

    private:
        OGSubview();
        Vector3d position;
        int height;
        int width;
        GLdouble modelMatrix[16];
        GLdouble projMatrix[16];
        OGProjection *p;
        OGPov *pov;

    public:
        void draw();
        void setDimensions(int,int);
        void setPosition(Vector3d);
        GLdouble getmodelMatrix();
        GLdouble getprojMatrix();

}

