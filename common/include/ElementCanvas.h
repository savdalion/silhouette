#pragma once

#include "configure.h"
#include "Element.h"


namespace siu {


/**
* Элемент холста.
*
* @see Sketch
*/
struct ElementCanvas : public Element {

    /**
    * Тип обобщённого идентификатора элемента.
    */
    typedef size_t cid_t;


    inline ElementCanvas(
        const matter_t& matter
    ) : Element( matter ) {
    }



    virtual inline ~ElementCanvas() {
    }





    /**
    * @return Обобщённый идентификатор элемента холста.
    */
    virtual cid_t cid() const = 0;

};


}
