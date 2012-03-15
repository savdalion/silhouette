#pragma once

#include <string>


namespace siu {


/**
* Элемент - основа для построения эскиза и холста.
*/
struct Element {

    /**
    * Материя, из которой состоит элемент.
    */
    typedef std::string matter_t;
    const matter_t matter;


    /**
    * @param nick Короткое имя для идентификации элемента.
    */
    inline Element(
        const matter_t& matter
    ) : matter( matter ) {
    }



    virtual inline ~Element() {
    }



};


}
