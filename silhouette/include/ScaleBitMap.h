#pragma once

#include "shape/ElevationMap.h"
#include <mapcontent3d/BitMap.h>
#include <coord.h>


namespace siu {


/**
* Битовая карта с возможностью масштабирования (м-биткарта).
*/
template< size_t SX, size_t SY, size_t SZ >
class ScaleBitMap {
public:
    /**
    * Ссылки на масштабируемую битовую карту.
    */
    typedef std::shared_ptr< ScaleBitMap >  Ptr;
    typedef std::unique_ptr< ScaleBitMap >  UPtr;

    /**
    * Тип для статической битовой карты.
    */
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;


public:
    ScaleBitMap();

    virtual ~ScaleBitMap();



    /**
    * @return Сформированная по заданной координате битовая карта.
    *         Биткарта верхнего уровня может быть сформирована без указания
    *         координаты (у верхней биткарты - одна координата (0; 0; 0) ).
    */
    bm_t const& draw() const;
    bm_t const& draw( int x, int y, int z ) const;
    bm_t const& draw( const typelib::coordInt_t& c ) const;



    /**
    * Источником м-биткарты становится карта высот.
    */
    template< size_t OSX, size_t OSY, size_t OSZ >
    ScaleBitMap& operator<<( const typename shape::ElevationMap< OSX, OSY, OSZ >& source );



    /**
    * Источником м-биткарты становится другая биткарта (уровнем выше).
    */
    template< size_t OSX, size_t OSY, size_t OSZ >
    ScaleBitMap& operator<<( const typename ScaleBitMap< OSX, OSY, OSZ >& source );





protected:
    /**
    * Координата и вычисленная для неё битовая карта (кеш).
    *
    * @see draw()
    */
    mutable typelib::coordInt_t c_;
    mutable bm_t bm_;

};



} // siu







#include "ScaleBitMap.inl"
