#pragma once


namespace siu {


/**
* Координаты.
* Введены для позициционирования элемента в эскизе.
*
* @see IntegerRelativeCoord
*/
class RelativeCoord {
public:
    inline RelativeCoord(
        // Достаточно верхнего горизонта: координата задаётся типом 'double'.
        // Т.о. границы эскиза не хранятся в относительной координате, в
        // отличие от IntegerRelativeCoord.
        int horizontCeil,
        double x, double y, double z
    ) :
        mHorizontCeil( horizontCeil ),
        mX( x ), mY( y ), mZ( z )
    {
    }



    inline ~RelativeCoord() {
    }



    inline int horizontCeil() const {
        return mHorizontCeil;
    }



    inline void horizontCeil( int hc ) {
        mHorizontCeil = hc;
    }



    inline double x() const {
        return mX;
    }



    inline void x( int cx ) {
        mX = cx;
    }



    inline double y() const {
        return mY;
    }



    inline void y( int cy ) {
        mY = cy;
    }



    inline double z() const {
        return mZ;
    }



    inline void z( int cz ) {
        mZ = cz;
    }





protected:
    /**
    * Горизонт: потолок.
    */
    int mHorizontCeil;

    /**
    * 3D-координата точности 'double'.
    */
    double mX;
    double mY;
    double mZ;
};


}
