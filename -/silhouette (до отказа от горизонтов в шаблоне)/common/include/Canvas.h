#pragma once

#include "typedef.h"
#include <boost/unordered_map.hpp>
#include "Sketch.h"


namespace siu {


/**
* �����. ����������� 3D-�����.
* ����� ����������� ���������� ������, ������ 
* ������������ ���������� ��� ��������� �������.
*
* @template HCeil ������� ��������, ������������ �������.
* @template HFloor ������ ��������, ������������ �������.
* @template K �����������. �� ������� ��� ������ ����������� �������� ������
*           ������������. ������ ���� ��������.
* @template HObservation ����������� ��������.
* 
* ������� ����� ������� � ������ ����������� ���������� ������� ������.
* ���������� ��������� �� ������ �������� ������ �������.
* ���������� ���������� �� ������ ����� � ��������� diapason().
*
* ������.
* ������ ������: [ f( HCeil ); f( HFloor ) ]
* [ ~ "���� �������" (3^27 ~ 7.6 ����. ��);  ~ "������ ��������" (3^22 ~ 31.4 ���. ��) ]
* ���������� �� ���������� �������� � "�������" (3^13 ~ 1600 ��) � ������.
* ����������� �������� (HObservation), �������� �� "��������" �� "������
* ��������" ������������� �� ������ �������� � ��� �������������� ���������
* (�������������), �.�. �� ������ ������ ������ ��� (HFloor) ����� ������.
* ��������, ����� ��������� HObservation �� �����, ������� �����������
* �������� ������ �� ��������, ��������������� �������.
*
* @see Sketch
*/
template< int HCeil, int HFloor, int HObservation, size_t K = 3 >
class Canvas {
public:
    /**
    * C��������� ������ - ��������, ���������� �� �����.
    */
    struct Content {
        // ������ � �������� �������, ������� ������������ ��� ����������.
        // �������� ����, �.�. ����� ����� �������� �����.
        std::vector< Sketch >  s;
        std::vector< ElementSketch >  es;

        // ������� ������ - ��������� ������� ��������� ������
        ElementCanvas  ec;

        // �����, ������������ ���������� ����� ������ ��������� ������
        // @todo ...

        BitContent(
            const std::vector< Sketch >&  s,
            const std::vector< ElementSketch >&  es,
            const ElementCanvas&  ec
        ) : es( es ), ec( ec ) {
            assert( ( !s.empty() && !es.empty() )
                && "���������� ������ ������ ������������� �� ��������� ������� � �� ���������." );
        }

        /**
        * @alias ec.cid()
        */
        inline ElementCanvas::cid_t cid() const {
            return ec.cid();
        }

        bool operator==( const Content& c ) const {
            return ( cid() == c.cid() );
        }

        size_t hash_value( const Content& c ) const {
            return cid();
        }
    };



public:
    inline Canvas() {
        static_assert( ((K % 2) == 1),
            "����������� ������ ������ ���� ��������." );
        static_assert( (depth() <= 19),
            "������� ������ �� ����� ��������� 19 (���������� �� ������������ ����������� ��� K == 3)." );
        // ���� ����� ������� ������������ �������, ������� ������ �� ������
        // ��������� 5, �.�. �� �� 2012 �. ������������ 3^5^3 = 243^3 =
        // = 14�348�907 ����� - �������� ������� ��������.
        // ��� ������� = 6, ���-�� ����� = 3^6^3 = 729^3 = 387 420 489.
    }



    virtual inline ~Canvas() {
    }



    /**
    * @return ������� ������.
    */
    static inline size_t depth() {
        return static_cast< size_t >( std::abs( HCeil - HFloor ) );
    }



    /**
    * @return ���-�� ����� (�������) � ����� ���������.
    */
    static inline size_t n() {
        return static_cast< size_t >( powN< K, depth() >().value );
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




    inline Canvas& operator<<( const Content&  yet ) {
        auto ftr = mContent.find( yet );
        if (ftr == mContent.cend()) {
            // cid() ��� ��� � ����������
            mContent.insert( yet );
        } else {
            // ��� ���� ������ � ��� �� cid()
            // @todo ftr->bm. ...
        }
        return *this;
    }






protected:
    /**
    * ���������� (���������) ������.
    * ������������ ����� ��������������� �� cid() �������� ������ �
    * ��������� ������-��������, ��� ������������� ��� ��������.
    */
    boost::unordered_set< Content >  mContent;

};

}
