#pragma once

#include "typedef.h"


namespace siu {


/**
* ����� ����� ��� ��������� ���������� ������� ������, ������.
* 
* @template H ��������. ����� ����. ������ ������������ ���������� �����.
* @template D �������. ����������� ������������.
* @template K �����������. �� ������� ��� ����������� �������� ������
*           ������������. ������ ���� ��������.
*
* H, D, K ���������� "�������".
*
* @see IntegerRelativeCoord
*
* V - ����. �� Volume, Visual
*/
template< int H, size_t D, size_t K = 3 >
class V {
public:
    inline V() {
        static_assert( ((K % 2) == 1), "����������� ������ ���� ��������." );
        assert( (D <= 19) && "������� �� ����� ��������� 19 (���������� �� ������������ �����������)." );
        // @todo fine stability ������� 'K' ����� ������� ������������ � ��� ������� 'D'.
    }



    inline ~V() {
    }



    /**
    * @return ���-�� ����� (�������) � ����� ���������.
    */
    static inline size_t n() {
        return static_cast< size_t >( powN< K, D >().value );
    }



    /**
    * ����������� ���������� �������� ���������� (������������).
    */
    static inline int min() {
        return -max();
    }



    /**
    * ������������ ���������� �������� ���������� (������������).
    */
    static inline int max() {
        return (static_cast< int >( n() ) - 1) / 2;
    }



    /**
    * @return �������� ���������� ����� ������ ������.
    */
    static inline bool inside( int x, int y, int z ) {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return �������� ���������� ����� �� ��������� ������.
    */
    static inline bool outside( int x, int y, int z ) {
        return (
            (z < min()) || (z > max())
         || (y < min()) || (y > max())
         || (x < min()) || (x > max())
        );
    }




    /**
    * @return ���������� ���������� �� ���������.
    *         ����� � ��������� [ 0; n()*n()*n() ).
    */
    static inline size_t ic( int x, int y, int z ) {
        const int a = static_cast< int >( n() );
        const int i = (
            (x + max())
          + (y + max()) * a
          + (z + max()) * a * a
        );
        assert( ( (i >= 0) && ( i < (a*a*a) ) )
            && "���������� ���������� ��� ����������� ���������." );
        return static_cast< size_t >( i );
    }


    /**
    * ��������� � 'coord' ��������� ���������� �� ����������.
    * ��������� ����� � ��������� [ min(); max() ].
    */
    static inline void ci( int* coord, size_t i ) {
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

};


}
