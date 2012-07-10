#pragma once

#include "Shape.h"


/* - �������� �� GraphicsMagick
#define PNG_DEBUG 3
#include <png.h>
*/

#include <Magick++.h>


namespace siu {
    namespace shape {

/**
* ����� �����.
*
* (!) ���������� ����������� � ��.
*
* @template ��. Shape
*/
template< size_t Grid >
class ElevationMap : public Shape< Grid > {
public:
    /**
    * �������� ����� �����.
    * ��������, ���� � ����� PNG.
    */
    const std::string source;

    /**
    * ������� ������� ��������, ��� / ��.
    */
    const float scaleXY;

    /**
    * ���. ������ (����. �������) �� �����, ��.
    */
    const float hMin;

    /**
    * ����. ������ �� �����, ��.
    */
    const float hMax;

    /**
    * ������ �����, ��.
    */
    const float hReal;

    /**
    * ����� �� ����� ����������� ������ (��� - ��������� XY).
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
    * ������ � ������ ����������� - � rSizeImage.
    */
    static void sizeImage(
        std::pair< size_t, size_t >* rSizeImage,
        const std::string& source
    );




private:
    /**
    * ��� ������� �����������.
    */
    mutable std::pair< size_t, size_t >  sizeImage_;


    /**
    * ������� @todo ���������� �� Warning C4252, ����������.
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
