#pragma once

#include "configure.h"
#include "RelativeCoord.h"
#include "BitContent3D.h"
#include <boost/tuple/tuple.hpp>
#include "Element.h"


namespace siu {


/**
* Элемент эскиза.
*
* Чтобы упростить добавление элементов в эскиз, физ. размеры элементов
* указываются в метрах.
*
* @see Sketch
*/
struct ElementSketch : public Element {

    /**
    * Физический размер элемента эскиза по осям XYZ.
    */
    typedef boost::tuple< double, double, double >  psize_t;



    /**
    * @param nick Короткое имя для идентификации элемента внутри эскиза.
    *        Может не указываться, т.к. не всякому эскизу надо знать, как
    *        называется элемент. Поиск элементов в эскизе ведётся по любой
    *        характеристике элемента.
    */
    explicit inline ElementSketch(
        const std::string& nick,
        const matter_t& matter
    ) :
        Element( matter ), nick( nick )
    {
    }



    virtual inline ~ElementSketch() {
    }



    /**
    * @return Физический размер элемента по осям X, Y, Z. Необходим для
    *         определения, попадает ли элемент эскиза на холст.
    */
    virtual psize_t psize() const = 0;



    /**
    * Каждый элемент эскиза умеет показать свою форму в виде битовой маски.
    *
    * @param realSizeCanvas Реальный размер холста, м.
    * @param nCanvas Кол-во неделимых ячеек, на которые разбит холст.
    * @param coordCanvas Координата центра холста.
    * @param coordElementSketch Координата, куда следует поместить элемент эскиза.
    */
    virtual BitContent3D form(
        double realSizeCanvas,
        size_t nCanvas,
        const RelativeCoord& coordCanvas,
        const RelativeCoord& coordElementSketch
    ) const = 0;



    virtual bool operator==( const ElementSketch& b ) const {
        return (nick == b.nick)
            && (matter == b.matter)
            && ( std::abs( psize().get<0>() - b.psize().get<0>() ) < d::PRECISION)
            && ( std::abs( psize().get<1>() - b.psize().get<1>() ) < d::PRECISION)
            && ( std::abs( psize().get<2>() - b.psize().get<2>() ) < d::PRECISION);
    }



    /**
    * Краткое имя для обращения к элементу эскиза.
    */
    const std::string nick;

};


}
