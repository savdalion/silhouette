#pragma once

#include "Shape.h"


/* - Заменено на GraphicsMagick
#define PNG_DEBUG 3
#include <png.h>
*/

#include <Magick++.h>


namespace siu {
    namespace shape {

/**
* Карта высот.
*
* (!) Расстояния указываются в км.
*
* @template См. Shape
*/
template< size_t Grid >
class ElevationMap : public Shape< Grid > {
public:
    /**
    * Источник карты высот.
    * Например, путь к файлу PNG.
    */
    const std::string source;

    /**
    * Масштаб плоской картинки, пкс / км.
    */
    const float scaleXY;

    /**
    * Мин. высота (макс. глубина) на карте, км.
    */
    const float hMin;

    /**
    * Макс. высота на карте, км.
    */
    const float hMax;

    /**
    * Высота карты, км.
    */
    const float hReal;

    /**
    * Будет ли карта заполняться внутри (дно - плоскость XY).
    */
    const bool fill;


public:
    template< typename IT >
    ElevationMap(
        const std::string& source,
        IT scaleXY,
        IT hMin, IT hMax,
        bool fill
    );



    virtual ~ElevationMap();





    /**
    * @virtual Shape
    */
    virtual bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t( 0.0f, 0.0f, 0.0f ),
        const typelib::coord_t& areaMax = typelib::coord_t( 0.0f, 0.0f, 0.0f )
    );




    virtual float sizeMax() const;





protected:
    /**
    * Ширина и высота изображения - в rSizeImage.
    */
    static void sizeImage(
        std::pair< size_t, size_t >* rSizeImage,
        const std::string& source
    );




private:
    /**
    * Кеш размера изображения.
    */
    mutable std::pair< size_t, size_t >  sizeImage_;


    /**
    * Попытка @todo избавиться от Warning C4252, безуспешно.
    * @source http://www.windows-api.com/microsoft/VC-Language/30952961/a-solution-to-warning-c4251--class-needs-to-have-dllinterface.aspx
    */
    struct Wrapper {
        Magick::Image image;
        Magick::Color color;
    };

};


    } // shape

} // siu








#include "ElevationMap.inl"
