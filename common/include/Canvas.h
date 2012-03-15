#pragma once

#include "configure.h"
#include <boost/assign/list_of.hpp>
#include <boost/unordered_set.hpp>
#include "Sketch.h"
#include "ElementCanvas.h"
#include "BitContent3D.h"
#include "SolidEC.h"


namespace siu {


/**
* �����. ����������� 3D-�����.
* ������������ ���������� ��� ��������� �������.
* ����� ����������� ���������� ������, ������ �������� ������ *�������������*
* � �������� ������.
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
class Canvas {
public:
    /**
    * C��������� ������ - ��������, ���������� �� �����.
    */
    struct Content {
        /* - @todo ��������.
        // ������ � �������� �������, ������� ������������ ��� ����������.
        // �������� ����, �.�. ����� ����� �������� �����. ������� �����
        // ��������� �������� ��������� � � ��������� ����������� ������.
        std::vector< Sketch >  s;
        std::vector< std::shared_ptr< ElementSketch > >  es;
        */

        // ������� ������ - ��������� ������� ��������� ������
        std::shared_ptr< ElementCanvas >  ec;

        // �����, ������������ ���������� ����� ������ ��������� ������
        std::shared_ptr< BitContent3D >  bm;

        Content(
            //const std::vector< Sketch >&  s,
            const std::vector< std::shared_ptr< ElementSketch > >&  es,
            const std::shared_ptr< ElementCanvas >  ec,
            const std::shared_ptr< BitContent3D >  bm
        ) : ec( ec ), bm( bm ) {
            /*
            assert( ( !s.empty() && !es.empty() )
                && "���������� ������ ������ ������������� �� ��������� ������� � �� ���������." );
            */
            assert( ec );
        }

        /**
        * @alias ec.cid()
        */
        inline ElementCanvas::cid_t cid() const {
            return ec->cid();
        }

        inline bool operator==( const Content& b ) const {
            return ( cid() == b.cid() );
        }

        /* - �������� � namespace boost.
        inline size_t hash_value() const {
            return cid();
        }
        */
    };



    /**
    * ���������� (���������) ������.
    * ������������ ����� ��������������� �� cid() �������� ������ �
    * ��������� ������-��������, ��� ������������� ��� ��������.
    */
    typedef boost::unordered_set< Content >  topology_t;



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
        const RelativeCoord& c
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




    inline boost::unordered_set< Content > const&  content() const {
        return mContent;
    }



    /**
    * @return �������� ������ � ������, ������� ���������� �����.
    */
    inline double realSize() const {
        return std::pow(
            static_cast< double >( K ),  static_cast< int >( hCeil )
        );
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
    * ��������� � 'coord[3]' ��������� ���������� �� ����������.
    * ��������� ����� � ��������� [ min(); max() ].
    */
    inline void ci( int* coord, size_t i ) const {
        const size_t a = n();
        const size_t aa = a * a;
        const size_t z = i / aa;
        const size_t kz = z * aa;
        const size_t y = (i - kz) / a;
        const size_t x = i - y * a - kz;
        // @todo optimize ����� ��� ��������� max()?
        coord[0] = static_cast< int >( x ) - max();
        coord[1] = static_cast< int >( y ) - max();
        coord[2] = static_cast< int >( z ) - max();
    }




    /**
    * ��������� ����� � ������.
    */
    inline Canvas& operator<<( const Sketch& sketch ) {
        // @todo optimize? ��������, ��� ����� �� �������� ���������,
        // ������ ������� ��������� ������ ������ ������. �������
        // ����� ��������� �����, ���� �� ��� ��� ����� �� ��������
        // �� �����.
        
        // �������� ������ ������������� � �������� ������

        // ���������� ������� �������� ������ (��. assert � todo � ����� ����)
        const double realSizeC = realSize();
        const double boxCeilMinC[] = {
            c.x - realSizeC / 2.0,
            c.y - realSizeC / 2.0,
            c.z - realSizeC / 2.0
        };
        const double boxCeilMaxC[] = {
            c.x + realSizeC / 2.0,
            c.y + realSizeC / 2.0,
            c.z + realSizeC / 2.0
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
            place( sketch, ctr->es, coordES );
            
        } // for (auto ctr = sketch.content().cbegin(); ctr != sketch.content().cend(); ++ctr)

        return *this;
    }






protected:
    /**
    * ����������� ������� ������ � �������, ���������� ��� ��������� � �����.
    * ��������� ���������� ������� �� �����, �������� ��������� ��������
    * ������ ������.
    */
    inline void place(
        const Sketch& sketch,
        const std::shared_ptr< ElementSketch >  esPlace,
        const RelativeCoord& cPlace
    ) {
        // ������ ������� ����� ������, �������� � ����� � �������
        const double rs = realSize();
        const size_t N = n();
        const BitContent3D b = esPlace->form(
            // �������� ������ ������, �
            rs,
            // ���-�� ��������� �����, �� ������� ������ �����
            N,
            // ���������� ������ ������
            c,
            // ����������, ���� ������� ��������� ������� ������
            cPlace
        );

        // ��������� ����� ����������
        //const std::vector< Sketch >  s = boost::assign::list_of( sketch );
        const std::vector< std::shared_ptr< ElementSketch > >  es =
            boost::assign::list_of( esPlace );
        // ������� ������ - ��������� ������� ��������� ������
        const auto ec =
            std::shared_ptr< ElementCanvas >( new SolidEC( esPlace->matter ) );
        // �����, ������������ ���������� ����� ������ ��������� ������
        const auto bm =
            std::shared_ptr< BitContent3D >( new BitContent3D( b )  );
        const Content yet( /*s,*/ es, ec, bm );

        // ��������� ����������
        auto ftr = mContent.find( yet );
        if (ftr == mContent.cend()) {
            // cid() ��� ��� � ���������:
            // ��������� ���������� �������
            mContent.insert( yet );

        } else {
            // ��� ���� ������ � ��� �� cid():
            // ������� ����������� ����������

            /* - @todo ������.
            // @test
            for (auto itr = yet.s.begin(); itr != yet.s.end(); ++itr) {
                auto s = *itr;
                (*ftr).s.push_back( s );
            }

            //ftr->s.insert( ftr->s.end(), yet.s.begin(), yet.s.end() );
            //ftr->es.insert( ftr->es.end(), yet.es.begin(), yet.es.end() );
            *ftr->bm |= *yet.bm;
            */

            *ftr->bm |= *yet.bm;
        }

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
    const RelativeCoord  c;


    /**
    * ��������� ������.
    */
    topology_t mContent;

};



/*
inline bool operator==( const siu::Canvas::Content& a, const siu::Canvas::Content& b ) {
    return ( a.cid() == b.cid() );
}
*/

}





namespace boost {

inline size_t hash_value( const siu::Canvas::Content& a ) {
    return a.cid();
}

}
