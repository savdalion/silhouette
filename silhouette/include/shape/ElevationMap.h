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
template< size_t SX, size_t SY, size_t SZ >
class ElevationMap : public Shape< SX, SY, SZ > {
public:
    /**
    * Ссылки на карту высот.
    */
    typedef std::shared_ptr< ElevationMap >  Ptr;
    typedef std::unique_ptr< ElevationMap >  UPtr;



public:
    /**
    * Источник карты высот.
    * Например, путь к файлу PNG.
    */
    const std::string source;

    /**
    * Масштаб плоской картинки, пкс / км.
    * Задействованы афинные преобразования, поэтому нет нужды указывать
    * отдельно масштабы (коэф. масштабирования) по X и Y.
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
    /**
    * @template T Позволяет передавать карте высот числа разного
    *           типа (double, float, int).
    */
    template< typename T >
    ElevationMap(
        const std::string& source,
        T scaleXY,
        T hMin, T hMax,
        bool fill
    );



    virtual ~ElevationMap();





    /**
    * @virtual Shape
    */
    virtual bm_t operator()(
        const typelib::coordInt_t& c = bm_t::undefinedCoord(),
        size_t OSX = SX, size_t OSY = SY, size_t OSZ = SZ
    );




    virtual typelib::coord_t sizeMax() const;





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
