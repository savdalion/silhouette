#pragma once

#include "shape/Shape.h"
#include "BitMap.h"
#include <coord-type.h>


namespace siu {


/**
* ������ �����, ��������� ��������� �������.
*/
template< size_t Grid >
class Shaper {
public:
    inline Shaper( shape::Shape< Grid >* functor ) : functor( functor ) {
    }


    virtual inline ~Shaper() {
    }



    /**
    * @return ����� � ���� ������ ����� (������� �����).
    */
    inline typename shape::Shape< Grid >::bm_t draw(
        const typelib::coord_t& areaMin = typelib::coord_t::ZERO(),
        const typelib::coord_t& areaMax = typelib::coord_t::ZERO()
    ) const {
        const auto bm = ( *functor )( areaMin, areaMax );

        // @test
        std::cout << "Points in " << typeid( *functor ).name() << " is " << bm.count() << std::endl;

        return bm;
    }




private:
    shape::Shape< Grid >* functor;

};



} // siu
