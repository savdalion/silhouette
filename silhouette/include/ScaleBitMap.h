#pragma once

#include "shape/Shape.h"
#include "shape/ElevationMap.h"
#include <mapcontent3d/BitMap.h>
#include <coord.h>


namespace siu {


/**
* ������� ����� � ������������ ��������������� (�-��������).
*/
template< size_t SX, size_t SY, size_t SZ >
class ScaleBitMap {
public:
    /**
    * ������ �� �������������� ������� �����.
    */
    typedef std::shared_ptr< ScaleBitMap >  Ptr;
    typedef std::unique_ptr< ScaleBitMap >  UPtr;

    /**
    * ��� ��� ����������� ������� �����.
    */
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;

    /**
    * ��� ��� ������ �� ����� (��������).
    */
    typedef typename shape::Shape< SX, SY, SZ >::Ptr  shape_t;

    /**
    * ��� ��� ������ �� �-�������� (��������).
    */
    typedef Ptr scaleBitMap_t;




public:
    ScaleBitMap();

    virtual ~ScaleBitMap();



    /**
    * @return �������������� ������� �����.
    */
    bm_t const& draw() const;



    /**
    * ���������� �-�������� ���������� ��������� �����: ����� �����,
    * ���������, �������� � �.�..
    */
    template< size_t OSX, size_t OSY, size_t OSZ >
    ScaleBitMap& operator<<( const typename shape::Shape< OSX, OSY, OSZ >& source );




    /**
    * @return ������ �� �����, ������� ������������ ��� �-��������.
    */
    inline shape_t const& shape() const {
        return mShape;
    }







protected:
    /**
    * ����� ��� �������� ������� �����.
    */
    shape_t mShape;


    /**
    * ����������� ������� ����� (���).
    *
    * @see draw()
    */
    mutable bm_t bm_;

};



} // siu







#include "ScaleBitMap.inl"
