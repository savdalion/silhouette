#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* Эллипосид.
*
* @see Sphere
*
* @template См. Shape.
*/
template< size_t SX, size_t SY, size_t SZ >
class Ellipsoid : public Shape< SX, SY, SZ > {
public:
    /**
    * Ссылки на эллипсоид.
    */
    typedef std::shared_ptr< Ellipsoid >  Ptr;
    typedef std::unique_ptr< Ellipsoid >  UPtr;



public:
    /**
    * Размеры.
    */
    const float rx;
    const float ry;
    const float rz;

    /**
    * Будет ли элипсоид заполнен внутри.
    */
    const bool fill;

    /**
    * Коэффициент масштабирования. Благодаря ему, размер эллипсоид будет
    * изменён перед формированием биткарты. При (scale == 1.0) эллипосид
    * вписывается в размер SX x SY x SZ.
    * Допустимо указывать значение > 1.0.
    */
    const float scale;



public:
    template< typename T, typename S >
    Ellipsoid(
        T rx, T ry, T rz,
        bool fill,
        S scale = 1.0f
    );



    virtual ~Ellipsoid();

    

    /**
    * @virtual Shape
    *
    * Эллипсоид строится прохождением по *всем* ячейкам сетки G.
    */
    virtual bm_t operator()();


    
    virtual typelib::coord_t sizeMax() const;
    


    virtual typename Shape::Ptr clone() const;

};


    } // shape

} // siu







#include "Ellipsoid.inl"
