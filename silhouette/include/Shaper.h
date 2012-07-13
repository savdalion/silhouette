#pragma once

#include "shape/Shape.h"
#include <typelib/typelib.h>


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
    /**
    * ��� ��� ������� �����.
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
    * @return ����� � ���� ������ ����� (������� �����).
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
