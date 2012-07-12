#pragma once

#include "shape/Shape.h"
#include "shape/ElevationMap.h"
#include <typelib/include/mapcontent3d/BitMap.h>
#include <typelib/include/coord.h>


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

    /**
    * Тип для ссылки на форму (источник).
    */
    typedef typename shape::Shape< SX, SY, SZ >::Ptr  shape_t;

    /**
    * Тип для ссылки на м-биткарту (источник).
    */
    typedef Ptr scaleBitMap_t;




public:
    ScaleBitMap();

    virtual ~ScaleBitMap();



    /**
    * @return Сформированная битовая карта.
    */
    bm_t const& draw() const;



    /**
    * Источником м-биткарты становится указанная форма: карта высот,
    * эллипосид, биткарта и т.п..
    */
    template< size_t OSX, size_t OSY, size_t OSZ >
    ScaleBitMap& operator<<( const typename shape::Shape< OSX, OSY, OSZ >& source );




    /**
    * @return Ссылка на форму, которой инициирована эта м-биткарта.
    */
    inline shape_t const& shape() const {
        return mShape;
    }







protected:
    /**
    * Форма для создания битовой карты.
    */
    shape_t mShape;


    /**
    * Вычисленная битовая карта (кеш).
    *
    * @see draw()
    */
    mutable bm_t bm_;

};



} // siu







#include "ScaleBitMap.inl"
