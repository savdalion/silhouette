#pragma once

#include "ElementCanvas.h"
#include <string>
#include <boost/functional/hash.hpp>


namespace siu {


/**
* Элемент холста в виде цельного блока однородной материи.
*/
struct SolidEC : public ElementCanvas {
    /**
    * @param nick Короткое имя для идентификации элемента внутри эскиза.
    *        Может не указываться, т.к. не всякому эскизу надо знать, как
    *        называется элемент. Поиск элементов в эскизе ведётся по любой
    *        характеристике элемента.
    */
    inline SolidEC() {
    }



    inline ~SolidEC() {
    }



    /**
    * @see ElementCanvas
    */
    inline virtual size_t cid() const {
        boost::hash< uidMatter_t >  hash;
        return hash( matter );
    }



    /**
    * Материя, из которой состоит блок.
    */
    typedef std::string uidMatter_t;
    uidMatter_t matter;

};


}
