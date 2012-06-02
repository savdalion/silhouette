#pragma once

#include <type.h>
#include <BitMapContent3D.h>
#include <math.h>



namespace siu {
    namespace shape {


/**
* Функтор - для common::Shaper, базовый класс - для форм.
*
* @template Grid Кол-во ячеек битовой карты.
*/
template< size_t Grid >
class Shape {
public:
    typedef common::BitMapContent3D< Grid >  bm_t;


public:
    inline Shape() : bm( false ) {
        static_assert( (Grid != 0), "Количество ячеек для битовой карты необходимо указать." );
    }


    virtual inline ~Shape() {
    }




    /**
    * Создаёт битовую карту.
    *
    * @param np 
    */
    virtual bm_t operator()() = 0;




protected:
    /**
    * Сформированная битовая карта.
    */
    bm_t bm;

};


    } // shape

} // siu
