#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* ���������.
*
* @template ��. Shape.
*/
template< size_t SX, size_t SY, size_t SZ >
class Ellipsoid : public Shape< SX, SY, SZ > {
public:
    /**
    * ������ �� ���������.
    */
    typedef std::shared_ptr< Ellipsoid >  Ptr;
    typedef std::unique_ptr< Ellipsoid >  UPtr;



public:
    /**
    * �������.
    */
    const float rx;
    const float ry;
    const float rz;

    /**
    * ����� �� �������� �������� ������.
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
    * ��������� �������� ������������ �� *����* ������� ����� G.
    */
    virtual bm_t operator()();


    
    virtual typelib::coord_t sizeMax() const;
    


    virtual typename Shape::Ptr clone() const;

};


    } // shape

} // siu







#include "Ellipsoid.inl"
