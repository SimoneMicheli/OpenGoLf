
#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include <math.h>

struct Vector3d
{
   double x, y, z;

   //Constructors
   Vector3d( void )
   {
       x = y = z = 0;
   }

   Vector3d( const Vector3d &v )
   {
       x = v.x;
       y = v.y;
       z = v.z;
   }

   Vector3d( const double newx,
             const double newy,
             const double newz)
   {
       x = newx;
       y = newy;
       z = newz;
   }

   Vector3d( const double a[3] )
   {
       x = a[0];
       y = a[1];
       z = a[2];
   }

    Vector3d( const double*& a )
    {
        x = a[0]; y = a[1]; z = a[2];
    }

   //operator overloading
   inline void operator=( const double a[3] )
   {
       x = a[0];
       y = a[1];
       z = a[2];
   }

   inline void operator=( const float a[3] )
   {
       x = a[0];
       y = a[1];
       z = a[2];
   }

   inline void operator+=( const Vector3d a )
   {
       x = x + a.x;
       y = y + a.y;
       z = z + a.z;
   }

   inline void copyInto( double a[3] )
   {
       a[0] = x;
       a[1] = y;
       a[2] = z;
   }

   inline void copyInto( float a[3] )
   {
       a[0] = x;
       a[1] = y;
       a[2] = z;
   }

    /*inline double distance ( const Vector3d a){
        Vector3d d =
        if (<#condition#>) {
            <#statements#>
        }
    }*/

   //referencing as array
   inline double &operator[]( int i )
   {
       switch (i) {
           case 0:
               return x;

            case 1:
               return y;

            case 2:
               return z;
       }
   }

   inline const double &operator[]( int i ) const
   {
       switch (i) {
           case 0:
               return x;

           case 1:
               return y;

           case 2:
               return z;
       }
   }

   //vector length square
   inline double lengthSq()
   {
       return pow(x, 2) + pow(y, 2) + pow(z, 2);
   }

   //vector length
   inline double length()
   {
       return sqrt(lengthSq());
   }

   //normalization
   inline void normalize()
   {
       double len = length();
       if (len ==0) {
           x=0;
           y=0;
           z=0;
       }
       x = x / len;
       y = y / len;
       z = z / len;
   }

    inline Vector3d getNormalized(){
        double len = length();
        if (len ==0) {
            return Vector3d();
        }
        x = x / len;
        y = y / len;
        z = z / len;
        return Vector3d(x,y,z);
    }

   //set to zero
   inline void zero() {
       x = y = z = 0;
   }

   //set to argument values
   inline void set(  const double newx,
                     const double newy,
                     const double newz )
   {
       x = newx;
       y = newy;
       z = newz;
   }

   //dot product
   inline double dot( const Vector3d &v )
   {
       return (x * v.x + y * v.y + z * v.z);
   }

   //cross product
   inline Vector3d cross( const Vector3d &v )
   {
      /* x = (y * v.z) - (z* v.y);
       y = (z * v.x) - ( x * v.z);
       z = (x * v.y) - (y * v.x);*/

       return Vector3d( y * v.z - z * v.y,
                       z * v.x - x * v.z,
                       x * v.y - y * v.x );
   }

   //rotations
   inline void rotateX( const double angle )
   {
       double y1 = (y * cos(angle)) - ( z * sin(angle));
       z = (y * sin(angle)) + ( z * cos(angle));
       y=y1;
   }

   inline void rotateY( const double angle )
   {
       double x1 = (x * cos(angle)) + (z * sin(angle));
       z = (-x * sin(angle)) + (z * cos(angle));
       x = x1;
   }

   inline void rotateZ( const double angle )
   {
       double x1 = (x * cos(angle)) - (y * sin(angle));
       y = (x * sin(angle)) + (y * sin(angle));
       x = x1;
   }


    inline float alfa(){
        float l = length();
        if (l != 0)
            if (l > 0)
                return asin(y/l);
            else
                return asin(y/l) - (M_PI/2);
        else
            return M_PI / 2;
    }

    inline float beta(){
        if (x != 0)
            if (x>0)
                return atanf(z/x);
            else
                return atanf(z/x) - M_PI;
        else
            return M_PI / 2;
    }

   /*
   inline void rotate( const Vector3d &axis, const double angle )
   {

   }
*/

};

//equality
inline bool operator==( const Vector3d &v1, const Vector3d &v2 )
{
    if (v1.x == v2.x && v1.y==v2.y && v1.z == v2.z) {
        return true;
    }else{
        return false;
    }
}

//vector sum
inline Vector3d operator+( const Vector3d &v1, const Vector3d &v2 )
{
    return *new Vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

//vector difference
inline Vector3d operator-( const Vector3d &v1, const Vector3d &v2 )
{
    return *new Vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

//vector-scalar product
inline Vector3d operator*( const Vector3d &v, const double &a )
{
    return *new Vector3d(v.x*a,v.y*a,v.z*a);
}

//scalar-vector product
inline Vector3d operator*( const double &a, const Vector3d &v )
{
    return *new Vector3d(v.x*a,v.y*a,v.z*a);
}

// unary minus
inline Vector3d operator-( const Vector3d &v )
{
    return *new Vector3d(-v.x,-v.y,-v.z);
}

/*inline Vector3d operator*( const double &v){
    return *new Vector3d();
}*/

//scalar division
inline Vector3d operator/( const Vector3d &v, const double &a )
{
    return *new Vector3d(v.x/a,v.y/a,v.z/a);
}

#endif
