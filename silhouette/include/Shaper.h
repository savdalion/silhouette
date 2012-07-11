#pragma once

#include "shape/Shape.h"
#include <coord.h>


namespace siu {


/**
* ������ �����, ��������� ��������� �������.
*
* @template SX ���-�� ����� �������� �� ��� X.
* @template SY ���-�� ����� �������� �� ��� Y.
* @template SZ ���-�� ����� �������� �� ��� Z.
*/
template< size_t SX, size_t SY, size_t SZ >
class Shaper {
public:
    /**
    * ��� ��� ������� �����.
    */
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;



public:
    inline Shaper( typename shape::Shape< SX, SY, SZ >::Ptr  functor ) : functor( functor ) {
    }


    virtual inline ~Shaper() {
    }



    /**
    * @return ����� � ���� ������ ����� (������� �����).
    */
    inline typename shape::Shape< SX, SY, SZ >::bm_t draw(
        const typelib::coordInt_t& c = bm_t::undefinedCoord(),
        // ������� �����, ����� ����� �������� ����������� ����� �����
        size_t OSX = SX, size_t OSY = SY, size_t OSZ = SZ
    ) const {

        const auto bm = ( *functor )( c, OSX, OSY, OSZ );

        // @test
        std::cout << "Points in " << typeid( *functor ).name() << " is " << bm.count() << std::endl;

        return bm;
    }




private:
    typename shape::Shape< SX, SY, SZ >::Ptr  functor;

};



} // siu
