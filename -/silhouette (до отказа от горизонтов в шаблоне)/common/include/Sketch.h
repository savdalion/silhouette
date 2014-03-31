#pragma once

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
* @template HCeil ������� ��������, ������������ �������.
* @template HFloor ������ ��������, ������������ �������. �������� ����������
*           ������� ����������� ������ ��� ���������� ������ �� �����.
* @template K �����������. �� ������� ��� ������ ����������� �������� ������
*           ������������. ������ ���� ��������.
*
* @see Canvas
*/
template< int HCeil, int HFloor, size_t K = 3 >
class Sketch {
public:
    /**
    * ���������� ������ - �������, ���������� � �����.
    */
    struct Content {
        ElementSketch es;
        RelativeCoord c;
        // @todo rotation

        Content(
            const ElementSketch& es,
            const RelativeCoord& c
        ) : es( es ), c( c ) {
        }
    };



public:
    /**
    * @param ���������� ������ � ������-��������.
    */
    inline Sketch( const RelativeCoord& rc ) {
        static_assert( ((K % 2) == 1), "����������� ������ ������ ���� ��������." );
    }



    virtual inline ~Sketch() {
    }



    /**
    * @return ������ ������ � ����� ���������, �.
    */
    static inline double n() {
        const int D = HCeil - HFloor;
        return std::pow( static_cast< double >( K ), D );
    }



    /**
    * ����������� ���������� �������� ���������� (������������).
    */
    static inline double min() {
        return -max();
    }



    /**
    * ������������ ���������� �������� ���������� (������������).
    */
    static inline double max() {
        return n() / 2.0;
    }



    /**
    * @return �������� ���������� ����� ������ ������.
    */
    static inline bool inside( double x, double y, double z ) {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return �������� ���������� ����� �� ��������� ������.
    */
    static inline bool outside( double x, double y, double z ) {
        return (
            (z < min()) || (z > max())
         || (y < min()) || (y > max())
         || (x < min()) || (x > max())
        );
    }




    /**
    * ��������� ������� � ������.
    *//* - �������� �� operator<<().
    inline void place( const ElementSketch& e, const RelativeCoord& c ) {
        mContent.push_back( Content( e, c ) );
    }
    */


    inline Sketch& operator<<( const Content& yet ) {
        mContent.push_back( yet );
        return *this;
    }




    /**
    * @return ���������� ������.
    */
    inline std::vector< Content >&  content() const {
        return mContent;
    }


    inline std::vector< Content >&  content() {
        return mContent;
    }






protected:
    /**
    * ���������� ������.
    * ������������ ����� ����� ��������� ������ ��������� �������, �����
    * � ���������.
    */
    std::vector< Content >  mContent;

};

}








/**
* ��������� � ������ �������.
*//* - @todo ����������� operator<< ������ place().
template< int HCeil, int HFloor, size_t K = 3 >
inline siu::Sketch&  operator<<( siu::Sketch< HCeil, HFloor, K >& s, const siu::Sketch::Content& yet ) {
    s.content().push_back( yet );
}
*/
