#pragma once

#include <type.h>
#include <BitMapContent3D.h>
#include <math.h>



namespace siu {
    namespace shape {


/**
* ������� - ��� common::Shaper, ������� ����� - ��� ����.
*
* @template Grid ���-�� ����� ������� �����.
*/
template< size_t Grid >
class Shape {
public:
    typedef common::BitMapContent3D< Grid >  bm_t;


public:
    inline Shape() : bm( false ) {
        static_assert( (Grid != 0), "���������� ����� ��� ������� ����� ���������� �������." );
    }


    virtual inline ~Shape() {
    }




    /**
    * ������ ������� �����.
    *
    * @param np 
    */
    virtual bm_t operator()() = 0;




protected:
    /**
    * �������������� ������� �����.
    */
    bm_t bm;

};


    } // shape

} // siu
