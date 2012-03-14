#pragma once

#include "typedef.h"
#include <boost/unordered_set.hpp>
#include "Sketch.h"
#include "ElementCanvas.h"


namespace siu {


/**
* �����. ����������� 3D-�����.
* ������������ ���������� ��� ��������� �������.
* ����� ����������� ���������� ������, ������ �������� ������ *�������������*
* � �������� ������.
*
* @template K �����������. �� ������� ��� ������ ����������� �������� ������
*           ������������. ������ ���� ��������.
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
template< size_t K = 3 >
class Canvas {
public:
    /**
    * C��������� ������ - ��������, ���������� �� �����.
    */
    struct Content {
        // ������ � �������� �������, ������� ������������ ��� ����������.
        // �������� ����, �.�. ����� ����� �������� �����.
        std::vector< Sketch< K > >  s;
        std::vector< std::shared_ptr< ElementSketch > >  es;

        // ������� ������ - ��������� ������� ��������� ������
        std::shared_ptr< ElementCanvas >  ec;

        // �����, ������������ ���������� ����� ������ ��������� ������
        // @todo ...

        Content(
            const std::vector< Sketch< K > >&  s,
            const std::vector< std::shared_ptr< ElementSketch > >&  es,
            const std::shared_ptr< ElementCanvas >  ec
        ) : s( s ), es( es ), ec( ec ) {
            assert( ( !s.empty() && !es.empty() )
                && "���������� ������ ������ ������������� �� ��������� ������� � �� ���������." );
            assert( ec );
        }

        /**
        * @alias ec.cid()
        */
        inline ElementCanvas::cid_t cid() const {
            return ec->cid();
        }

        bool operator==( const Content& content ) const {
            return ( cid() == content.cid() );
        }

        size_t hash_value() const {
            return cid();
        }
    };



public:
    /*
    * @param hCeil ������� ��������, ������������ �������.
    * @param hFloor ������ ��������, ������������ �������.
    * @param HObservation ����������� ��������.
    * @param c ���������� ������ ������������ ������������� ���������.
    *        ������������ �������� ������ ������ 'c'.
    */
    inline Canvas(
        int hCeil, int hFloor, int hObservation,
        const RelativeCoord< K >& c
    ) : hCeil( hCeil ), hFloor( hFloor ), hObservation( hObservation ), c( c ) {
        static_assert( ((K % 2) == 1),
            "����������� ������ ������ ���� ��������." );
        assert( (hCeil >= hFloor)
            && "������� �������� ������ ���� ������ ��� ����� �������." );
        assert( (hObservation <= hFloor)
            && "�������� ���������� ���� ������� ���������." );
        assert( ((hCeil - hFloor) <= 19)
            && "������� ������ �� ����� ��������� 19 (���������� �� ������������ ����������� ��� K == 3)." );
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
    inline size_t depth() const {
        return static_cast< size_t >( hCeil - hFloor );
    }



    /**
    * @return ���-�� ����� (������������� �������) � ����� ���������.
    */
    inline size_t n() const {
        return static_cast< size_t >(
            std::pow( static_cast< double >( K ),  static_cast< int >( depth() ) )
        );
    }



    /**
    * ����������� ���������� �������� ������������� ���������� (������������).
    */
    inline int min() const {
        return -max();
    }



    /**
    * ������������ ���������� �������� ������������� ���������� (������������).
    */
    inline int max() const {
        return (static_cast< int >( n() ) - 1) / 2;
    }



    /**
    * @return �������� ������������� ���������� ����� ������ ������.
    */
    inline bool inside( int x, int y, int z ) const {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return �������� ������������� ���������� ����� �� ��������� ������.
    */
    inline bool outside( int x, int y, int z ) const {
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
    inline size_t ic( int x, int y, int z ) const {
        const int a = static_cast< int >( n() );
        // @todo optimize ����� ��� ����������� max()?
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
    inline void ci( int* coord, size_t i ) const {
        const size_t a = n();
        const size_t aa = a * a;
        const size_t z = i / aa;
        const size_t kz = i - z * aa;
        const size_t y = kz / a;
        const size_t x = i - y * a - kz;
        // @todo optimize ����� ��� ��������� max()?
        coord[0] = (int)x - max();
        coord[1] = (int)y - max();
        coord[2] = (int)z - max();
    }




    /**
    * ��������� ����� � ������.
    */
    inline Canvas& operator<<( const Sketch< K >&  sketch ) {
        // @todo optimize? ��������, ��� ����� �� �������� ���������,
        // ������ ������� ��������� ������ ������ ������. �������
        // ����� ��������� �����, ���� �� ��� ��� ����� �� ��������
        // �� �����.
        
        // �������� ������ ������������� � �������� ������

        // ���������� ������� �������� ������ (��. assert � todo � ����� ����)
        const double sizeCeilC = std::pow(
            static_cast< double >( K ),  static_cast< int >( hCeil )
        );
        const double boxCeilMinC[] = {
            c.x - sizeCeilC / 2.0,
            c.y - sizeCeilC / 2.0,
            c.z - sizeCeilC / 2.0
        };
        const double boxCeilMaxC[] = {
            c.x + sizeCeilC / 2.0,
            c.y + sizeCeilC / 2.0,
            c.z + sizeCeilC / 2.0
        };

        // ���������� ������� ��� ��������� �� ����������� � ������
        // (����� �� �������) �������
        // @todo ...

        for (auto ctr = sketch.content().cbegin(); ctr != sketch.content().cend(); ++ctr) {
            // �������� �� ������� / ����� �������� ������ �� �����? ��� �����
            // ������� ���������� � ������ �������� ������ � �������� �
            // �������� ������.
            const auto& coordES = ctr->c;
            const auto& sizeES = ctr->es->psize();
            const double boxMinES[] = {
                coordES.x - sizeES.get<0>() / 2.0,
                coordES.y - sizeES.get<1>() / 2.0,
                coordES.z - sizeES.get<2>() / 2.0
            };
            const double boxMaxES[] = {
                coordES.x + sizeES.get<0>() / 2.0,
                coordES.y + sizeES.get<1>() / 2.0,
                coordES.z + sizeES.get<2>() / 2.0
            };

            // @todo ��������� �������� � ������� �����������.
            assert( (coordES.hCeil == c.hCeil) && "��������� ������ � ������ �� ���������. @todo" );

            // ���� ��������� ���������, ��������� ����� �������
            const auto intersectBox =
                [ &boxCeilMinC, &boxCeilMaxC, &boxMinES, &boxMaxES ] () -> bool {
                    return (boxCeilMinC[0] <= boxMaxES[0])
                        && (boxCeilMaxC[0] >= boxMinES[0])
                        && (boxCeilMinC[1] <= boxMaxES[1])
                        && (boxCeilMaxC[1] >= boxMinES[1])
                        && (boxCeilMinC[2] <= boxMaxES[2])
                        && (boxCeilMaxC[2] >= boxMinES[2]);
                };
            if ( !intersectBox() ) {
                // ������� ������ �� ����� ����� �� ��������
                continue;
            }

            // ������� ��� ��� �����, ��������, ������� �� �����
            // ��������� ������� �� �����
            place( ctr->es, coordES );
            
        } // for (auto ctr = sketch.content().cbegin(); ctr != sketch.content().cend(); ++ctr)


        // ��������� �����
        // Content&  yet;
        // @todo ...

        /*
        // ��������� ����������
        auto ftr = mContent.find( yet );
        if (ftr == mContent.cend()) {
            // cid() ��� ��� � ����������
            mContent.insert( yet );
        } else {
            // ��� ���� ������ � ��� �� cid()
            // @todo ftr->bm. ...
        }
        */

        return *this;
    }






protected:
    /**
    * ����������� ������� ������ � �������, ���������� ��� ��������� � �����.
    * ��������� ���������� ������� �� �����, �������� ��������� ��������
    * ������ ������.
    */
    inline void place(
        const std::shared_ptr< ElementSketch >  esPlace,
        const RelativeCoord< K >&  cPlace
    ) {
        // ���������� ������ �������� ������ ������
        //@todo ...

        // @test
        const auto& t1 = typeid( PhysicsSolidES ).name();
        const auto& t2 = typeid( *esPlace ).name();

        if (typeid( EllipsoidES ) == typeid( *esPlace )) {
            auto ellipsoidES = static_cast< EllipsoidES* >( esPlace.get() );
            // @todo ...

        } else {
            // @todo ...
        }

        // @todo ...

    }






protected:
    /**
    * ��������� ������.
    */
    const int hCeil;
    const int hFloor;
    const int hObservation;

    /**
    * ���������� ������.
    */
    const RelativeCoord< K >  c;


    /**
    * ���������� (���������) ������.
    * ������������ ����� ��������������� �� cid() �������� ������ �
    * ��������� ������-��������, ��� ������������� ��� ��������.
    */
    boost::unordered_set< Content >  mContent;

};

}
