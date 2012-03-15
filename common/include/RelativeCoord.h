#pragma once

#include "default.h"


namespace siu {


/**
* ����������.
* ������� ��� ������������������ �������� � ������, ����������������
* ������ ������������ ��������� � �.�..
*
* @see IntegerRelativeCoord
*/
struct RelativeCoord {

    /**
    * ��������: �������.
    * ���������� ����������� ������������ ������ ����� ���������.
    */
    const int hCeil;

    /**
    * 3D-���������� � ��������� 'double'.
    */
    const double x;
    const double y;
    const double z;



    inline RelativeCoord(
        // ���������� �������� ���������: ���������� ������� ����� 'double'.
        // �.�. ������� ������ �� �������� � ������������� ����������, �
        // ������� �� IntegerRelativeCoord.
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
        assert( (hCeil == b.hCeil) && "�� ����� �������� � ������� �����������. @todo" );
        return RelativeCoord( hCeil,  x + b.x,  y + b.y,  z + b.z );
    }



    inline RelativeCoord operator-( const RelativeCoord& b ) const {
        assert( (hCeil == b.hCeil) && "�� ����� �������� � ������� �����������. @todo" );
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
