#pragma once

#include "configure.h"
#include "RelativeCoord.h"
#include "ElementSketch.h"


namespace siu {


/**
* �����.
* ������������� �������� ���� ��� ��� �����.
*
* @see V
*
* ���������� ��������� � ������ �������� ������ �������.
* ���������� ���������� ����� � ��������� [ min(), max() ].
*
* @see Canvas
*/
class Sketch {
public:
    /**
    * ���������� ������ - �������, ���������� � �����.
    */
    struct Content {
        std::shared_ptr< ElementSketch >  es;
        RelativeCoord c;
        // @todo rotation

        Content(
            const std::shared_ptr< ElementSketch >  es,
            const RelativeCoord&  c
        ) : es( es ), c( c ) {
            assert( es && "������� ������ �� �����." );
        }

        inline bool operator==( const Content& b ) const {
            return (*es == *b.es) && (c == b.c);
        }

    };



public:
    /**
    * @param rc ���������� ������ � ������-��������.
    * @param hCeil ������� ��������, ������������ �������.
    * @param hFloor ������ ��������, ������������ �������.
    */
    inline Sketch(
        const RelativeCoord&  rcParent,
        int hCeil, int hFloor
    ) : rcParent( rcParent ), hCeil( hCeil ), hFloor( hFloor ) {
        static_assert( ((K % 2) == 1),
            "����������� ������ ������ ���� ��������." );
        assert( (hCeil >= hFloor)
            && "������� �������� ������ ���� ������ ��� ����� �������." );
    }



    virtual inline ~Sketch() {
    }



    /**
    * @return ������� ������.
    */
    inline size_t depth() const {
        return static_cast< size_t >( hCeil - hFloor );
    }



    /**
    * @return ������ ������ � ����� ���������, �.
    */
    inline double n() const {
        return std::pow( static_cast< double >( K ),  static_cast< int >( depth() ) );
    }



    /**
    * ����������� ���������� �������� ���������� (������������).
    */
    inline double min() const {
        return -max();
    }



    /**
    * ������������ ���������� �������� ���������� (������������).
    */
    inline double max() const {
        return n() / 2.0;
    }



    /**
    * @return �������� ���������� ����� ������ ������.
    */
    inline bool inside( double x, double y, double z ) const {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return �������� ���������� ����� �� ��������� ������.
    */
    inline bool outside( double x, double y, double z ) const {
        return (
            (z < min()) || (z > max())
         || (y < min()) || (y > max())
         || (x < min()) || (x > max())
        );
    }




    /**
    * ��������� ������� � ������.
    * ������ �������� �� ����� ��������� ������ ������. ���������, �����
    * ������� (�� �� ��� �����!) ������� �� ������� ������.
    *//* - �������� �� operator<<().
    inline void place( const ElementSketch& e, const RelativeCoord& c ) {
        mContent.push_back( Content( e, c ) );
    }
    */

    inline Sketch& operator<<( const Content& yet ) {
        assert( (yet.es->psize().get<0>() <= n() )
             && (yet.es->psize().get<1>() <= n() )
             && (yet.es->psize().get<2>() <= n() )
             && "������ �������� �� ����� ��������� ������ ������." );

        mContent.push_back( yet );

        return *this;
    }




    /**
    * @return ���������� ������.
    */
    inline const std::vector< Content >&  content() const {
        return mContent;
    }


    inline std::vector< Content >&  content() {
        return mContent;
    }




    inline bool operator==( const Sketch& b ) const {
        return (hCeil == b.hCeil)
            && (hFloor == b.hFloor)
            && (rcParent == b.rcParent)
            && (mContent == b.mContent);
    }





public:
    /**
    * ��������� ������.
    */
    const int hCeil;
    const int hFloor;

    /**
    * ���������� ������������ ������������� ������.
    */
    const RelativeCoord rcParent;



protected:
    /**
    * ���������� ������.
    * ������������ ����� ����� ��������� ������ ��������� �������, �����
    * � ���������.
    */
    std::vector< Content >  mContent;

};

}
