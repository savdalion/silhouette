#pragma once

#include <mapcontent3d/BitMap.h>


namespace siu {


/**
* ���������� ������� �����.
*/
template< size_t Grid >
class BitMap :
    public typelib::BitMap< Grid, Grid, Grid >
{
public:
    inline BitMap() {
        /* - ������: typelib::BitMap ����� �������� ������.
        set( false );
        */
    }


    virtual inline ~BitMap() {
    }

};



} // siu
