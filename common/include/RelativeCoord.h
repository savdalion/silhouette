#pragma once


namespace siu {


/**
* Координаты.
* Введены для позициционирования элемента в эскизе, позиционирования
* холста относительно горизонта и т.п..
*
* @see IntegerRelativeCoord
*/
template< size_t K = 3 >
struct RelativeCoord {

    /**
    * Горизонт: потолок.
    * Координаты указываются относительно центра этого горизонта.
    */
    const int hCeil;

    /**
    * 3D-координата с точностью 'double'.
    */
    const double x;
    const double y;
    const double z;



    explicit inline RelativeCoord(
        // Достаточно верхнего горизонта: координата задаётся типом 'double'.
        // Т.о. границы эскиза не хранятся в относительной координате, в
        // отличие от IntegerRelativeCoord.
        int hCeil,
        double x = 0.0, double y = 0.0, double z = 0.0
    ) :
        hCeil( hCeil ),
        x( x ), y( y ), z( z )
    {
    }

};


}
