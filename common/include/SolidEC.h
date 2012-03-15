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
    * Материя, из которой состоит блок.
    */
    typedef std::string uidMatter_t;
    const uidMatter_t matter;


    /**
    * @param nick Короткое имя для идентификации элемента внутри эскиза.
    *        Может не указываться, т.к. не всякому эскизу надо знать, как
    *        называется элемент. Поиск элементов в эскизе ведётся по любой
    *        характеристике элемента.
    */
    inline SolidEC(
        const uidMatter_t& matter
    ) : matter( matter ) {
    }



    inline ~SolidEC() {
    }



    /**
    * @see ElementCanvas
    */
    inline virtual cid_t cid() const {
        boost::hash< uidMatter_t >  hash;
        return hash( matter );
    }

};


}
