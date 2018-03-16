#ifndef ALL_INCLUDE
#define ALL_INCLUDE

////////////////////
/// \brief: this header is a user header, and it will inplement some user-defined struct
////////////////////

// a struct for Vector include two
class MVec{
private:
    float x;
    float y;

public:
    MVec() = default;

    MVec(float _x,float _y):x(_x),y(_y){}

    inline MVec operator+(const MVec &v) const{
        return MVec(x + v.getX(), y + v.getY());
    }

    inline MVec operator-(const MVec &v) const{
        return MVec(x - v.getX(), y - v.getY());
    }

    inline MVec operator *(const float &v) const{
        return MVec(x * v,y *v);
    }

    inline MVec operator /(const float &v) const{
        return MVec(x / v,y /v);
    }

    inline static float dot(const MVec &v1, const MVec &v2){
        return v1.getX() * v2.getX() + v1.getY() * v2.getY();
    }

    inline float lengthSquare() const{
        return  x * x + y * y;
    }

    inline float length() const{
        return sqrtf(lengthSquare());
    }
    inline float getY() const{
        return x;
    }
    inline float getX() const{
        return y;
    }

    inline bool valid(){
        if(x == 0 && y ==0){
            return false;
        }
        return true;
    }

};


#endif // ALL_INCLUDE
