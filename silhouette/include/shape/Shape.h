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
    /**
    * ������ �� ����� �����.
    */
    typedef std::shared_ptr< Shape >  Ptr;
    typedef std::unique_ptr< Shape >  UPtr;

    /**
    * ��� ��� ������� �����.
    */
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;



public:
    inline Shape() : bm() {
        static_assert( (SX != 0), "���������� ����� �� ��� X ��� �������� ���������� �������." );
        static_assert( (SY != 0), "���������� ����� �� ��� Y ��� �������� ���������� �������." );
        static_assert( (SZ != 0), "���������� ����� �� ��� Z ��� �������� ���������� �������." );
    }


    inline Shape( const bm_t& bm ) : bm( bm ) {
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
        const typelib::coordInt_t& c = bm_t::undefinedCoord(),
        // ������� �����, ����� ����� �������� ����������� ����� �����
        size_t OSX = SX, size_t OSY = SY, size_t OSZ = SZ
    ) = 0;




    /**
    * @return �����, ���������������� �� ���������� ������� �����.
    *//* - @todo ?
    template< size_t OSX, size_t OSY, size_t OSZ >
    inline Shape< OSX, OSY, OSZ >::Ptr scale() const {
        const auto t = bm.scale< OSX, OSY, OSZ >();
        return Shape< OSX, OSY, OSZ >::Ptr( new Shape< OSX, OSY, OSZ >( t ) );
    }
    */



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
