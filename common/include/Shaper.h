#pragma once

#include "type.h"
#include "BitMapContent3D.h"
#include <Shape.h>
#include <math.h>



namespace siu {
    namespace common {


/**
* –исует форму, использу€ указанный функтор.
*/
template< size_t Grid >
class Shaper {
public:
    inline Shaper( shape::Shape< Grid >* functor ) : functor( functor ) {
    }


    virtual inline ~Shaper() {
    }



    /**
    * @return ‘орма в виде облака точек (битовой карты).
    */
    inline BitMapContent3D< Grid > draw(
        /* - @todo
        const coord_t& areaMin,
        const coord_t& areaMax,
        */
    ) const {
        const auto bm = ( *functor )();

        // @test
        std::cout << "Points in " << typeid( *functor ).name() << " is " << bm.count() << std::endl;

        return bm;
    }




private:
    shape::Shape< Grid >* functor;

};


    } // common

} // siu
