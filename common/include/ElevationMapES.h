#pragma once

#include "PhysicsSolidES.h"
#include <ElevationMap.h>


namespace siu {


/**
* Элемент эскиза в виде цельного физического тела, форма - эллипсоид.
*/
template< bool FillT >
struct ElevationMapES : public PhysicsSolidES {

    inline ElevationMapES(
        const std::string& nick,
        const matter_t& matter
    ) :
        PhysicsSolidES( nick, matter ),
        source( "" ),
        sizeXY( 0.0 ),
        height( 0.0 )
    {
    }



    inline ~ElevationMapES() {
    }



    /**
    * @see ElementSketch
    */
    virtual psize_t psize() const {
        return psize_t( sizeXY, sizeXY, height );
    }



    /**
    * @see ElementSketch
    */
    virtual inline BitContent3D form(
        double realSizeCanvas,
        size_t nCanvas,
        const RelativeCoord& coordCanvas,
        const RelativeCoord& coordElementSketch
    ) const {
        assert( (coordCanvas.hCeil == coordElementSketch.hCeil)
            && "Холст и элемент эскиза должны быть привязаны к одному горизонту." );

        // Какой реальный размер вмещает 1 ячейка холста, м
        const double rsc1Cell = realSizeCanvas / static_cast< double >( nCanvas );
        // Переводим размеры карты высот из "метров" в "ячейки холста". Холст
        // может быть меньшего размера, чем эскиз, поэтому понадобится
        // дополнительная детализация.

        const auto nsxy = static_cast< size_t >( std::ceil( sizeXY / rsc1Cell ) );
        const auto nh = static_cast< size_t >( std::ceil( height / rsc1Cell ) );
        // Находим координату центра карты высот относительно центра холста
        const RelativeCoord center = (
            (coordCanvas - coordElementSketch) / rsc1Cell
        );

        // Формируем карту высот в заданном битовом объёме
        const shape::ElevationMap< FillT >  elevationMap( source, nsxy, nh );
        const BitContent3D bc = elevationMap.draw(
            nCanvas,
            static_cast< int >( std::floor( center.x + 0.001 ) ),
            static_cast< int >( std::floor( center.y + 0.001 ) ),
            static_cast< int >( std::floor( center.z + 0.001 ) )
        );

        return bc;
    }




    /**
    * @see PhysicsSolidES
    */
    inline virtual double volume() const {
        // @todo Считать объём точнее.
        return sizeXY * sizeXY * height;
    }




    /**
    * Путь к источнику - файлу PNG - для построения объекта по карте высот.
    */
    std::string source;


    /**
    * Размер элемента эскиза, заданного картой высот, м.
    *
    * @todo Позволять не только квадратные размеры.
    */
    double sizeXY;


    /**
    * Высота всей карты высот, м.
    */
    double height;

};


}
