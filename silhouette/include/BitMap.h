#pragma once

#include <mapcontent3d/BitMap.h>


namespace siu {


/**
* Кубическая битовая карта.
*/
template< size_t Grid >
class BitMap :
    public typelib::BitMap< Grid, Grid, Grid >
{
public:
    inline BitMap() {
        /* - Лишнее: typelib::BitMap сразу создаётся пустой.
        set( false );
        */
    }


    virtual inline ~BitMap() {
    }

};



} // siu
