#pragma once

#include "type.h"
#include <memory>


namespace siu {
    namespace common {


/* - ���, ������ ��� ������.
class MapContent3D;
typedef std::shared_ptr< MapContent3D >  MapContent3DPtr;
*/


/**
* ����� ����������. 3D. ������� �����.
*
* @template S* ���-�� ����� �� �������� ���������������.
*
*
* �������:
*    - ������ - ����� ������, �������� �� ������ ����� 3-�� ����������� (3^3 =
*      27 ������). ���., tria (3) + planum (���������) = ���[���]���.
*    - ����� - ����� ������, �������� �� ������ ����� 9-��� ����������� (9^3 =
*      729 ������). ���., novem (9) + planum (���������) = ��[�����]���.
* @see ����� �� ������ > http://www.liveinternet.ru/users/2752294/post114441737
*
*
* ������� ���������� ������ 3D-������ (��������), �������� 3-�� �����������.
* ������� �� ��� OY �� ��� 3x3x3. ��� Z ���������� �����.
* ������� ������� (�����������) 2D ���� - �������� ��� �������� isc2D().
* ������� ������� - �������� �����. ������� ������� ���� - ����������.
*
    ���������, ������������ ������:

    ������� 2D ����
    26   19   23
    22   18   20
    25   21   24

    ����������� 2D ����
    8   1   5
    4   0   2
    7   3   6

    ������� 2D ����
    17   10   14
    13    9   11
    16   12   15
*
*/
template< typename T, size_t SX, size_t SY, size_t SZ >
class MapContent3D {
public:

    inline MapContent3D() {
        static_assert( ((SX % 2) == 1), "����� ������� �� X ������ ���� ��������." );
        static_assert( ((SY % 2) == 1), "����� ������� �� Y ������ ���� ��������." );
        static_assert( ((SZ % 2) == 1), "����� ������� �� Z ������ ���� ��������." );
    }



    inline virtual ~MapContent3D() {
    }




    /**
    * @return ������ ����� ���������� (3D, ����� ������).
    */
    static inline psizeInt_t size() {
        return psizeInt_t( static_cast< int >( SX ),  static_cast< int >( SY ),  static_cast< int >( SZ ) );
    }



    /**
    * @return ��� ������� ����� �����.
    */
    static inline bool cube() {
        return ( (SX == SY) && (SX == SZ) );
    }



    /**
    * @return ���������� ����� �� ������ �� ������ ����� ����������.
    */
    static inline size_t sizeCube() {
        static_assert( (SX == SY) && (SX == SZ), "����� �������� ������ ��� ���������� ����." );
        return SX;
    }



    /**
    * @return ����� �����. �� �� - ������ � 1D.
    */
    static inline size_t volume() {
        return (SX * SY * SZ);
    }



    /**
    * @return ����������� � ������������ ���������� �� �����.
    */
    static inline coordInt_t minCoord() {
        return -maxCoord();
    }

    static inline coordInt_t maxCoord() {
        return ( (size() - 1) / 2 ).toCoordInt();
    }




    /**
    * @return ���������� ����� �� �����.
    *
    * @alias !outside()
    */
    static inline bool inside( const coordInt_t& c ) {
        //return inside( ic( c ) );
        return (c >= minCoord()) && (c <= maxCoord());
    }

    static inline bool inside( int x, int y, int z ) {
        //return inside( ic( x, y, z ) );
        return inside( coordInt_t( x, y, z ) );
    }

    static inline bool inside( size_t i ) {
        return (i < volume());
    }




    /**
    * @return ���������� ����� �� ��������� �����.
    *
    * @alias !inside()
    */
    static inline bool outside( const coordInt_t& c ) {
        const coordInt_t mi = minCoord();
        const coordInt_t ma = maxCoord();
        return ( (c.x < mi.x) || (c.x > ma.x) )
            || ( (c.y < mi.y) || (c.y > ma.y) )
            || ( (c.z < mi.z) || (c.z > ma.z) )
        ;
    }

    static inline bool outside( int x, int y, int z ) {
        return outside( coordInt_t( x, y, z ) );
    }

    /* - ����� �� ����� � ���������� �������.
    static inline bool outside( size_t i ) {
        return (i >= volume());
    }
    */



    /**
    * @return ������ �� ������ ������� �����.
    *         (!) 1D-���������� � ������� �������� 'raw'.
    *
    * @see raw, next(), ic(), ci()
    */
    virtual size_t first() const = 0;



    /**
    * @return ��������� �������� ������� ����� �� 1D-���������� ���
    *         0, ���� ����. ��������� �������� �� ����� ���.
    *         (!) 1D-���������� � ������� �������� 'raw'.
    *
    * @see first(), ic(), ci()
    */
    virtual size_t next( size_t i ) const = 0;




    /**
    * @return ��������� � ��������� ������� ������ �� ������.
    *
    * ���� �������� ������ ������� �� ������� 3D-������� (������ ����� ��
    * �������), ���������� 'false'.
    *
    * @param � ��������������� ������.
    * @param k ����� �������� ������. ��������� ������ - ��. ����. � ������.
    */
    inline bool has( const coordInt_t& �, size_t k ) const {
        assert( inside( c ) && "���������� ����� �� ��������� �����." );
        const coordInt_t coordCell = � + isc( k );
        return inside( coordCell ) && test( coordCell );
    }



    /**
    * @return ���������� �� ���� 6-�� ������ ������ - �� ������.
    *         ����������� ������ 1, 2, 3, 4, 9, 18 - ��. ����. � ������.
    *
    * @param c ��������������� ������.
    */
    inline bool has6( const coordInt_t& c ) const {
        return
            // �������
            has( c, 1 )
            // ������
         && has( c, 2 )
            // ������
         && has( c, 3 )
            // �����
         && has( c, 4 )
            // �������
         && has( c, 9 )
            // �������
         && has( c, 18 );
    }





    /**
    * @param i ���������� ����������.
    *
    * @see ic()
    */
    virtual void set( size_t i, const T value ) = 0;


    virtual void set( const T value ) = 0;


    inline void set( const coordInt_t& c, const T value ) {
        set( ic( c ),  value );
    }


    inline bool set( int x, int y, int z, const T value ) {
        return set( ic( x, y, z ),  value );
    }



    /**
    * @param i ���������� ����������.
    *
    * @see ic()
    */
    void reset( size_t i ) {
        set( i, T() );
    }


    inline void reset( const coordInt_t& c ) {
        reset( ic( c ) );
    }


    inline bool reset( int x, int y, int z ) {
        return reset( ic( x, y, z ) );
    }


    inline void reset() {
        set( T() );
    }




    
    /**
    * @return ���������� ��������� ������ - �� ������.
    */
    virtual bool test( size_t i ) const = 0;

    virtual bool test( const coordInt_t& c ) const = 0;

    virtual bool test( int x, int y, int z ) const = 0;




    /**
    * @return ���������� �������� ������.
    */
    virtual size_t count() const = 0;



    /**
    * @return ����� �� �������� ������ (��� ������ - ������).
    */
    inline bool empty() const {
        return (count() == 0);
    }




    /**
    * @return 3D-����������, ���������� � 1D.
    */
    static inline size_t ic( const coordInt_t& c ) {
        return ic( c.x, c.y, c.z );
    }

    static inline size_t ic( int x, int y, int z ) {
        /* - �������� ������ �����������.
             ������: IzoOgre3DVisual::operator<<( Picture )
        assert( inside( x, y, z ) && "���������� ����� �� ��������� �����." );
        */
        const size_t i = (
            static_cast< size_t >(x + maxCoord().x)
          + static_cast< size_t >(y + maxCoord().y) * SX
          + static_cast< size_t >(z + maxCoord().z) * SX * SY
        );
        return i;
    }



    /**
    * @return ��������� ���������� �� ����������.
    *         ��������� ����� � ��������� [ minCoord(); maxCoord() ].
    */
    static inline coordInt_t ci( size_t i ) {
        assert( inside( i ) && "���������� 'i' ����� �� ��������� �����." );
        // @todo optimize ����� ���������� �� ������� �������.
        const size_t z = i / (SX * SY);
        const size_t kz = z * SX * SY;
        const size_t y = (i - kz) / SX;
        const size_t x = i - y * SX - kz;

        const coordInt_t c(
            static_cast< int >( x ) - maxCoord().x,
            static_cast< int >( y ) - maxCoord().y,
            static_cast< int >( z ) - maxCoord().z
        );
        assert( inside( c ) && "����������� 3D ���������� ����� �� ��������� �����." );

        return c;
    }



    /**
    * @return ������ �������� � ������� ��������� ������.
    *         ���� ��������� ������� ����, ���������� (0; 0).
    *
    * @see ��������� � ����. � ������.
    */
    static inline coordInt_t isc( size_t cell ) {
        const static coordInt_t a[27] = {
            // ����������� 2D ����: 0-8
            coordInt_t(  0,  0,  0 ),
            coordInt_t(  0,  0, +1 ),
            coordInt_t( +1,  0,  0 ),
            coordInt_t(  0,  0, -1 ),
            coordInt_t( -1,  0,  0 ),
            coordInt_t( +1,  0, +1 ),
            coordInt_t( +1,  0, -1 ),
            coordInt_t( -1,  0, -1 ),
            coordInt_t( -1,  0, +1 ),
            // ������� 2D ����: 9-17
            coordInt_t(  0, -1,  0 ),
            coordInt_t(  0, -1, +1 ),
            coordInt_t( +1, -1,  0 ),
            coordInt_t(  0, -1, -1 ),
            coordInt_t( -1, -1,  0 ),
            coordInt_t( +1, -1, +1 ),
            coordInt_t( +1, -1, -1 ),
            coordInt_t( -1, -1, -1 ),
            coordInt_t( -1, -1, +1 ),
            // ������� 2D ����: 18-26
            coordInt_t(  0, +1,  0 ),
            coordInt_t(  0, +1, +1 ),
            coordInt_t( +1, +1,  0 ),
            coordInt_t(  0, +1, -1 ),
            coordInt_t( -1, +1,  0 ),
            coordInt_t( +1, +1, +1 ),
            coordInt_t( +1, +1, -1 ),
            coordInt_t( -1, +1, -1 ),
            coordInt_t( -1, +1, +1 )
        };

        return a[ cell ];
    }


};



    } // common

} // siu
