#pragma once

#include "shape/Shape.h"
#include <typelib/include/coord.h>


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
    /**
    * “ип дл€ битовой карты.
    */
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;



public:
    inline Shaper( typename shape::Shape< SX, SY, SZ >::Ptr  functor ) : functor( functor ) {
    }


    inline Shaper( typename const shape::Shape< SX, SY, SZ >&  functor ) : functor( functor.clone() ) {
    }


    virtual inline ~Shaper() {
    }



    /**
    * @return ‘орма в виде облака точек (битовой карты).
    */
    inline typename shape::Shape< SX, SY, SZ >::bm_t draw() const {

        const auto bm = ( *functor )();

        // @test
        std::cout << "Points in " << typeid( *functor ).name() << " is " << bm.count() << std::endl;

        return bm;
    }




private:
    typename shape::Shape< SX, SY, SZ >::Ptr  functor;

};



} // siu
