#pragma once

#include "typedef.h"


namespace siu {

    
/**
* ����������, �������������� � ���� ����� �����.
* ������� ��� ������������������ �������� �� ������.
*
* @template K �����������. �� ������� ��� ����������� �������� ������
*           ������������. ������ ���� ��������.
*
* @see IntegerRelativeCoord
*/
template< size_t K = 3 >
class IntegerRelativeCoord {
public:
    inline IntegerRelativeCoord(
        int horizontCeil, int horizontFloor,
        int x, int y, int z
    ) :
        mHorizontCeil( horizontCeil ), mHorizontFloor( horizontFloor ),
        mX( x ), mY( y ), mZ( z )
    {
        static_assert( ((K % 2) == 1), "����������� ������ ���� ��������." );
        assert( ((horizontCeil - horizontFloor) <= 19)
            && "������� �� ����� ��������� 19 (���������� �� ������������ �����������)." );
        assert( inside() && "���������� ��� ����������� ���������." );
    }



    inline ~IntegerRelativeCoord() {
    }



    inline int horizontCeil() const {
        return mHorizontCeil;
    }



    inline void horizontCeil( int hc ) {
        mHorizontCeil = hc;
    }



    inline int horizontFloor() const {
        return mHorizontFloor;
    }



    inline void horizontFloor( int hf ) {
        mHorizontFloor = hf;
    }



    /**
    * @return ������� ����������.
    */
    inline size_t depth() const {
        return (mHorizontCeil - mHorizontFloor);
    }



    static inline size_t k() {
        return K;
    }







    inline int x() const {
        return mX;
    }



    inline void x( int cx ) {
        mX = cx;
    }



    inline int y() const {
        return mY;
    }



    inline void y( int cy ) {
        mY = cy;
    }



    inline int z() const {
        return mZ;
    }



    inline void z( int cz ) {
        mZ = cz;
    }



    /**
    * @return ���-�� ����� (�������) � ����� ���������.
    */
    inline size_t n() const {
        return static_cast< size_t >(
            (mHorizontCeil > 0) ? std::pow( (float)K, mHorizontCeil )
          : (mHorizontCeil == 0) ? 1
          : std::pow( (float)K, -mHorizontCeil )
        );
    }



    /**
    * ������� - �� ��������� � ������ n() - ����������.
    *
    * @see n() ��� �������
    *//* - ���������� V<>::n().
    template< size_t D >
    static inline size_t n() {
        return static_cast< size_t >( powN< K, D >().value );
    }
    */



    /**
    * @return ����������� ���������� �������� ���������� (������������).
    */
    inline int min() const {
        return -max();
    }



    /**
    * ������������ ���������� �������� ���������� (������������).
    */
    inline int max() const {
        // n() - ������ �������� ��������� 'K', ��. �����������
        return (n() - 1) / 2;
    }



    /**
    * @return �������� ���������� - ������ ������.
    */
    inline bool inside() const {
        return (
            (x() >= min()) && (x() <= max())
         && (y() >= min()) && (y() <= max())
         && (z() >= min()) && (z() <= max())
        );
    }



    /**
    * @return �������� ���������� - �� ��������� ������.
    */
    inline bool outside() const {
        return (
            (z() < min()) || (z() > max())
         || (y() < min()) || (y() > max())
         || (x() < min()) || (x() > max())
        );
    }




    /**
    * @return ���������� ����������.
    *         ����� � ��������� [ 0; n()*n()*n() ).
    */
    inline size_t ic() {
        const int a = static_cast< int >( n() );
        const int i = (
            (x() + max())
          + (y() + max()) * a
          + (z() + max()) * a * a
        );
        assert( ( (i >= 0) && ( i < (a*a*a) ) )
            && "���������� ���������� ��� ����������� ���������." );
        return static_cast< size_t >( i );
    }


    /**
    * ��������� 'coord[3]' ��������� ����������� �� �������� ����������.
    * ��������� ����� � ��������� [ min(); max() ].
    */
    inline void ci( int* coord, size_t i ) {
        const size_t a = n();
        const size_t aa = a * a;
        const size_t z = i / aa;
        const size_t kz = i - z * aa;
        const size_t y = kz / a;
        const size_t x = i - y * a - kz;
        coord[0] = (int)x - max();
        coord[1] = (int)y - max();
        coord[2] = (int)z - max();
    }





protected:
    /**
    * ��������: ������� (����) � ��� (���).
    */
    int mHorizontCeil;
    int mHorizontFloor;

    /**
    * ������������� 3D-����������.
    */
    int mX;
    int mY;
    int mZ;
};


}
