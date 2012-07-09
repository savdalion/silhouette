#pragma once

#include <coord.h>
#include "../BitMap.h"



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
    typedef BitMap< Grid >  bm_t;


public:
    inline Shape() : bm() {
        static_assert( (Grid != 0), "���������� ����� ��� ������� ����� ���������� �������." );
    }


    virtual inline ~Shape() {
    }




    /**
    * ������ ������� �����.
    *
    * @param np 
    */
    virtual bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t::ZERO,
        const typelib::coord_t& areaMax = typelib::coord_t::ZERO
    ) = 0;




    /**
    * @return ������������ ������ ����� �� ����� �� �����. ����.
    *
    * @see sizeGrid()
    */
    virtual float sizeMax() const = 0;




    /**
    * @return ������ ����� ������ �����. ������ ���������� �.�., ����� ������
    *         ��������� ���������� � ���� �� �������� Grid.
    */
    virtual inline float sizeGrid() const {
        return sizeMax() / static_cast< float >( Grid );
    }



    /**
    * @return �������� ��������� ����� � ��������� �������� ����� ������.
    *
    * @see outside()
    */
    inline bool inside(
        int x, int y, int z,
        const typelib::coord_t& areaMin,
        const typelib::coord_t& areaMax
    ) {
        return !outside( x, y, z, araeMin, areaMax );
    }





    /**
    * @return �������� ��������� ����� �� ��������� ����� ������.
    */
    inline bool outside(
        // ���������� ������
        int x, int y, int z,
        // ����������� ������� (��������� = � �������� �������)
        const typelib::coord_t& areaMin,
        const typelib::coord_t& areaMax
    ) {
        assert( !empty(areaMin - areaMax)
            && "����������� ������� �� ����� ���� ������." );
        assert( (areaMin < areaMax)
            && "����������� ������� ������� ������� � ������� ������������ �������. ������ � ����� �������." );

        const float sizeG = sizeGrid();
        const float wx = static_cast< float >( x ) * sizeG;
        const float wy = static_cast< float >( y ) * sizeG;
        const float wz = static_cast< float >( z ) * sizeG;
        return ( (wx < areaMin.x) || (wx > areaMax.x) )
            || ( (wy < areaMin.y) || (wy > areaMax.y) )
            || ( (wz < areaMin.z) || (wz > areaMax.z) )
        ;
    }





protected:
    /**
    * �������������� ������� �����.
    */
    bm_t bm;

};


    } // shape

} // siu
