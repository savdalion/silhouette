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
    inline Shaper( shape::Shape< SX, SY, SZ >* functor ) : functor( functor ) {
    }


    virtual inline ~Shaper() {
    }



    /**
    * @return ����� � ���� ������ ����� (������� �����).
    */
    inline typename shape::Shape< SX, SY, SZ >::bm_t draw(
        const typelib::coord_t& areaMin = typelib::coord_t::ZERO(),
        const typelib::coord_t& areaMax = typelib::coord_t::ZERO()
    ) const {
        const auto bm = ( *functor )( areaMin, areaMax );

        // @test
        std::cout << "Points in " << typeid( *functor ).name() << " is " << bm.count() << std::endl;

        return bm;
    }




private:
    shape::Shape< SX, SY, SZ >*  functor;

};



} // siu
