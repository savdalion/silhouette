#pragma once


namespace siu {


/**
* ����������.
* ������� ��� ������������������ �������� � ������.
*
* @see IntegerRelativeCoord
*/
class RelativeCoord {
public:
    inline RelativeCoord(
        // ���������� �������� ���������: ���������� ������� ����� 'double'.
        // �.�. ������� ������ �� �������� � ������������� ����������, �
        // ������� �� IntegerRelativeCoord.
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
    * ��������: �������.
    */
    int mHorizontCeil;

    /**
    * 3D-���������� �������� 'double'.
    */
    double mX;
    double mY;
    double mZ;
};


}
