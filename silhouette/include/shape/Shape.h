#pragma once

#include <coord.h>
#include <mapcontent3d/BitMap.h>



namespace siu {
    namespace shape {


/**
* ������� - ��� common::Shaper, ������� ����� - ��� ����.
*
* @template SX ���-�� ����� �������� �� ��� X.
* @template SY ���-�� ����� �������� �� ��� Y.
* @template SZ ���-�� ����� �������� �� ��� Z.
*/
template< size_t SX, size_t SY, size_t SZ >
class Shape {
public:
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;


public:
    inline Shape() : bm() {
        static_assert( (SX != 0), "���������� ����� �� ��� X ��� �������� ���������� �������." );
        static_assert( (SY != 0), "���������� ����� �� ��� Y ��� �������� ���������� �������." );
        static_assert( (SZ != 0), "���������� ����� �� ��� Z ��� �������� ���������� �������." );
    }


    virtual inline ~Shape() {
    }




    /**
    * ������ ������� �����.
    */
    virtual bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t::ZERO,
        const typelib::coord_t& areaMax = typelib::coord_t::ZERO
    ) = 0;




    /**
    * @return ������������ ������ �����.
    *
    * @see sizeGrid()
    */
    virtual typelib::coord_t sizeMax() const = 0;




    /**
    * @return ������ ����� ������ �����. ������ ���������� �.�., ����� ������
    *         ��������� ���������� � ������ �������� (SX, SY, SZ).
    */
    virtual inline typelib::coord_t sizeGrid() const {
        return sizeMax() / typelib::coord_t( SX, SY, SZ );
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
        return !outside( x, y, z, areaMin, areaMax );
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
