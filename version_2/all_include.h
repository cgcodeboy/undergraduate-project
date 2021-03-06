#ifndef ALL_INCLUDE
#define ALL_INCLUDE

#include <math.h>
#include <time.h>

#define rho 206265

#define M_PI 3.1415926

////////////////////
/// \brief: this header is a user header, and it will inplement some user-defined struct
////////////////////

typedef struct Position{
    float x;
    float y;
    Position(float _x,float _y):x(_x),y(_y){}
}Position;

typedef struct LatLon{
    float lat;
    float lon;
    LatLon(float _lat,float _lon):lat(_lat),lon(_lon){}
}LatLon;

// a struct for Vector include two
class MVec2{
private:
    float x;
    float y;

public:
    MVec2() = default;

    MVec2(float _x,float _y):x(_x),y(_y){}

    MVec2(const MVec2 &vec2){this->x = vec2.getX(),this->y = vec2.getY();}

    MVec2& operator = (const MVec2 &vec2){this->x = vec2.getX(),this->y = vec2.getY();return *this;}

    inline MVec2 operator+(const MVec2 &v) const{
        return MVec2(x + v.getX(), y + v.getY());
    }

    inline MVec2 operator-(const MVec2 &v) const{
        return MVec2(x - v.getX(), y - v.getY());
    }

    inline MVec2 operator *(const float &v) const{
        return MVec2(x * v,y *v);
    }

    inline MVec2 operator /(const float &v) const{
        return MVec2(x / v,y /v);
    }

    inline static float dot(const MVec2 &v1, const MVec2 &v2){
        return v1.getX() * v2.getX() + v1.getY() * v2.getY();
    }

    inline float lengthSquare() const{
        return  x * x + y * y;
    }

    inline float length() const{
        return sqrtf(lengthSquare());
    }
    inline float getY() const{
        return y;
    }
    inline float getX() const{
        return x;
    }

    inline bool valid(){
        if(x == 0 && y ==0){
            return false;
        }
        return true;
    }

};

class MVec3{
private:
    float x;
    float y;
    float z;

public:
    MVec3() = default;

    MVec3(float _x,float _y,float _z):x(_x),y(_y),z(_z){}

    MVec3(const MVec3 &vec3){this->x = vec3.getX(),this->y = vec3.getY(),this->z = vec3.getZ();}

    MVec3& operator = (const MVec3 &vec3){this->x = vec3.getX(),this->y = vec3.getY(),this->z = vec3.getZ();return *this;}

    inline MVec3 operator+(const MVec3 &v) const{
        return MVec3(x + v.getX(), y + v.getY(),z + v.getZ());
    }

    inline MVec3 operator-(const MVec3 &v) const{
        return MVec3(x - v.getX(), y - v.getY(),z - v.getZ());
    }

    inline MVec3 operator *(const float &v) const{
        return MVec3(x * v,y *v,z *v);
    }

    inline MVec3 operator /(const float &v) const{
        return MVec3(x / v,y /v,z /v);
    }

    inline static float dot(const MVec3 &v1, const MVec3 &v2){
        return v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ();
    }

    inline float lengthSquare() const{
        return  x * x + y * y + z * z;
    }

    inline float length() const{
        return sqrtf(lengthSquare());
    }
    inline float getY() const{
        return y;
    }
    inline float getX() const{
        return x;
    }
    inline float getZ() const{
        return z;
    }

    inline bool valid(){
        if(x == 0 && y ==0 && z == 0){
            return false;
        }
        return true;
    }

};

double dms2Rad(double Dms);

double dms2D(double Dms);

double D2Dms(double D);

Position calculateellipse2plane(double B, double L);
LatLon Calculateplane2ellipse(double x, double y);

#endif // ALL_INCLUDE
