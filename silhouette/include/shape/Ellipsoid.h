#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* Ёллипосид.
*
* @template —м. Shape.
*/
template< size_t SX, size_t SY, size_t SZ >
class Ellipsoid : public Shape< SX, SY, SZ > {
public:
    /**
    * —сылки на эллипсоид.
    */
    typedef std::shared_ptr< Ellipsoid >  Ptr;
    typedef std::unique_ptr< Ellipsoid >  UPtr;



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
    template< typename T >
    Ellipsoid(
        T rx, T ry, T rz,
        bool fill
    );



    virtual ~Ellipsoid();

    

    /**
    * @virtual Shape
    *
    * Ёллипсоид строитс€ прохождением по *всем* €чейкам сетки G.
    */
    virtual bm_t operator()(
        const typelib::coordInt_t& c = bm_t::undefinedCoord(),
        size_t OSX = SX, size_t OSY = SY, size_t OSZ = SZ
    );


    
    virtual typelib::coord_t sizeMax() const;


};


    } // shape

} // siu







#include "Ellipsoid.inl"
