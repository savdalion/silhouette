#pragma once

#include <BitMapContent3D.h>


namespace siu {


/**
* Кубическая битовая карта.
*/
template< size_t Grid >
class BitMap :
    public typelib::BitMapContent3D< Grid, Grid, Grid >
{
public:
    inline BitMap() {
        /* - Лишнее: typelib::BitMapContent3D сразу создаётся пустой.
        set( false );
        */
    }


    virtual inline ~BitMap() {
    }

};



} // siu
