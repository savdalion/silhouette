#pragma once

#include "shape/Shape.h"
#include <coord.h>


namespace siu {


/**
* –исует форму, использу€ указанный функтор.
*
* @template SX  ол-во €чеек биткарты по оси X.
* @template SY  ол-во €чеек биткарты по оси Y.
* @template SZ  ол-во €чеек биткарты по оси Z.
*/
template< size_t SX, size_t SY, size_t SZ >
class Shaper {
public:
    inline Shaper( shape::Shape< SX, SY, SZ >* functor ) : functor( functor ) {
    }


    virtual inline ~Shaper() {
    }



    /**
    * @return ‘орма в виде облака точек (битовой карты).
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
