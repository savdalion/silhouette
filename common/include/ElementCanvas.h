#pragma once


namespace siu {


/**
* Элемент холста.
*
* @see Sketch
*/
struct ElementCanvas {

    /**
    * Тип обобщённого идентификатора элемента.
    */
    typedef size_t cid_t;


    inline ElementCanvas() {
    }



    inline ~ElementCanvas() {
    }





    /**
    * @return Обобщённый идентификатор элемента.
    */
    virtual cid_t cid() const = 0;

};


}
