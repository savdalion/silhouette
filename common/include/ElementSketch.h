#pragma once

#include <boost/tuple/tuple.hpp>


namespace siu {


/**
* Элемент эскиза.
*
* Чтобы упростить добавление элементов в эскиз, физ. размеры элементов
* указываются в метрах.
*
* @see Sketch
*/
struct ElementSketch {

    /**
    * Физический размер элемента по осям XYZ.
    */
    typedef boost::tuple< double, double, double >  psize_t;



    /**
    * @param nick Короткое имя для идентификации элемента внутри эскиза.
    *        Может не указываться, т.к. не всякому эскизу надо знать, как
    *        называется элемент. Поиск элементов в эскизе ведётся по любой
    *        характеристике элемента.
    */
    explicit inline ElementSketch(
        const std::string& nick
    ) :
        nick( nick )
    {
    }



    inline ~ElementSketch() {
    }



    /**
    * @return Физический размер элемента по осям X, Y, Z. Необходим для
    *         определения, попадает ли элемент эскиза на холст.
    */
    virtual psize_t psize() const = 0;




    /**
    * Краткое имя для обращения к элементу эскиза.
    */
    const std::string nick;

};


}
