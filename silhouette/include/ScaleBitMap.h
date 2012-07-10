#pragma once

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


public:
    ScaleBitMap();

    virtual ~ScaleBitMap();



    /**
    * @return �������������� �� �������� ���������� ������� �����.
    *         �������� �������� ������ ����� ���� ������������ ��� ��������
    *         ���������� (� ������� �������� - ���� ���������� (0; 0; 0) ).
    */
    bm_t const& draw() const;
    bm_t const& draw( int x, int y, int z ) const;
    bm_t const& draw( const typelib::coordInt_t& c ) const;



    /**
    * ���������� �-�������� ���������� ����� �����.
    */
    template< size_t OSX, size_t OSY, size_t OSZ >
    ScaleBitMap& operator<<( const typename shape::ElevationMap< OSX, OSY, OSZ >& source );



    /**
    * ���������� �-�������� ���������� ������ �������� (������� ����).
    */
    template< size_t OSX, size_t OSY, size_t OSZ >
    ScaleBitMap& operator<<( const typename ScaleBitMap< OSX, OSY, OSZ >& source );





protected:
    /**
    * ���������� � ����������� ��� �� ������� ����� (���).
    *
    * @see draw()
    */
    mutable typelib::coordInt_t c_;
    mutable bm_t bm_;

};



} // siu







#include "ScaleBitMap.inl"
