#pragma once

#include "default.h"


namespace siu {


/**
*  оординаты.
* ¬ведены дл€ позициционировани€ элемента в эскизе, позиционировани€
* холста относительно горизонта и т.п..
*
* @see IntegerRelativeCoord
*/
struct RelativeCoord {

    /**
    * √оризонт: потолок.
    *  оординаты указываютс€ относительно центра этого горизонта.
    */
    const int hCeil;

    /**
    * 3D-координата с точностью 'double'.
    */
    const double x;
    const double y;
    const double z;



    inline RelativeCoord(
        // ƒостаточно верхнего горизонта: координата задаЄтс€ типом 'double'.
        // “.о. границы эскиза не хран€тс€ в относительной координате, в
        // отличие от IntegerRelativeCoord.
        int hCeil,
        double x, double y, double z
    ) :
        hCeil( hCeil ),
        x( x ), y( y ), z( z )
    {
    }



    explicit
    inline RelativeCoord(
        int hCeil
    ) :
        hCeil( hCeil ),
        x( 0.0 ), y( 0.0 ), z( 0.0 )
    {
    }



    inline RelativeCoord operator+( const RelativeCoord& b ) const {
        assert( (hCeil == b.hCeil) && "Ќе умеем работать с разными горизонтами. @todo" );
        return RelativeCoord( hCeil,  x + b.x,  y + b.y,  z + b.z );
    }



    inline RelativeCoord operator-( const RelativeCoord& b ) const {
        assert( (hCeil == b.hCeil) && "Ќе умеем работать с разными горизонтами. @todo" );
        return RelativeCoord( hCeil,  x - b.x,  y - b.y,  z - b.z );
    }



    inline RelativeCoord operator*( double k ) const {
        return RelativeCoord( hCeil,  x * k,  y * k,  z * k );
    }



    inline RelativeCoord operator/( double k ) const {
        return RelativeCoord( hCeil,  x / k,  y / k,  z / k );
    }



    inline bool operator==( const RelativeCoord& b ) const {
        return (hCeil == b.hCeil)
            && ( std::abs(x - b.x) < d::PRECISION )
            && ( std::abs(y - b.y) < d::PRECISION )
            && ( std::abs(z - b.z) < d::PRECISION );
    }

};


}
