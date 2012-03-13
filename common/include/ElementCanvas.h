#pragma once


namespace siu {


/**
* Элемент холста.
*
* @see Sketch
*/
struct ElementCanvas {

    inline ElementCanvas() {
    }



    inline ~ElementCanvas() {
    }





    /**
    * @return Обобщённый идентификатор элемента.
    */
    virtual size_t cid() const = 0;

};


}
