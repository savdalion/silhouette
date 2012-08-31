#pragma once

#include "Ellipsoid.h"


namespace siu {
    namespace shape {

/**
* —фера.
*
* @see Sphere
*
* @template —м. Shape.
*/
template< size_t SX, size_t SY, size_t SZ >
class Sphere : public Ellipsoid< SX, SY, SZ > {
public:
    /**
    * —сылки на сферу.
    */
    typedef std::shared_ptr< Sphere >  Ptr;
    typedef std::unique_ptr< Sphere >  UPtr;



public:
    template< typename T, typename S >
    Sphere(
        T rx,
        bool fill,
        S scale = 1.0f
    );



    virtual ~Sphere();

    

    /**
    * @virtual Shape
    *
    * Ёллипсоид строитс€ прохождением по *всем* €чейкам сетки G.
    */
    virtual bm_t operator()();


    
    virtual typename Shape::Ptr clone() const;

};


    } // shape

} // siu







#include "Sphere.inl"
