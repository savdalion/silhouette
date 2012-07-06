#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* Ёллипосид.
*
* @template —м. Shape
*/
template< size_t Grid >
class Ellipsoid : public Shape< Grid > {
public:
    /**
    * –азмеры.
    */
    const float rx;
    const float ry;
    const float rz;

    /**
    * Ѕудет ли элипсоид заполнен внутри.
    */
    const bool fill;



public:
    template< typename IT >
    Ellipsoid(
        IT rx, IT ry, IT rz,
        bool fill
    );



    virtual ~Ellipsoid();

    

    /**
    * @virtual Shape
    *
    * Ёллипсоид строитс€ прохождением по *всем* €чейкам сетки G.
    */
    virtual bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t( 0.0f, 0.0f, 0.0f ),
        const typelib::coord_t& areaMax = typelib::coord_t( 0.0f, 0.0f, 0.0f )
    );


    
    virtual float sizeMax() const;


};


    } // shape

} // siu







#include "Ellipsoid.inl"
