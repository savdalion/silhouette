#pragma once

#include "typedef.h"
#include "Canvas.h"
#include "EllipsoidES.h"


#if 0

namespace siu {


/**
* Творит.
*/
template< size_t N >
class Creator {
public:
    inline Creator() {
    }


    virtual inline ~Creator() {
    }



    inline Canvas< N >* canvas() {
        return mCanvas;
    }



    inline void canvas( Canvas< N >*  canvas ) {
        mCanvas = canvas;
    }





protected:
    Canvas< N >*  mCanvas;

};

}






/**
* Творец берёт указанный холст для работы: все посл. действия происходят
* на взятом холсте.
*/
template< size_t N >
inline siu::Creator< N >& operator<<( siu::Creator< N >&  creator,  siu::Canvas< N >*  canvas ) {
    creator.canvas( canvas );
    return creator;
}





/**
* Помещает на холст эллипсоид.
*/
template< size_t N >
inline siu::Creator< N >& operator<<( siu::Creator< N >&  creator,  const siu::layer_t& layer ) {
    siu::Canvas< N >* canvas = creator.canvas();
    const auto& uid = layer.first;
    const auto& topology = layer.second;
    canvas->topology().insert( std::make_pair( uid, layer ) );
    return creator;
}

#endif
