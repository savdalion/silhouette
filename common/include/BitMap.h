#pragma once

#include <BitMapContent3D.h>


namespace siu {
    namespace common {


/**
* ���������� ������� �����.
*/
template< size_t Grid >
class BitMap :
    public typelib::BitMapContent3D< Grid, Grid, Grid >
{
public:
    inline BitMap() {
        set( false );
    }


    virtual inline ~BitMap() {
    }

};


    } // common

} // siu
