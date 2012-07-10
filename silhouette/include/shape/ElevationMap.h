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
template< size_t SX, size_t SY, size_t SZ >
class ElevationMap : public Shape< SX, SY, SZ > {
public:
    /**
    * �������� ����� �����.
    * ��������, ���� � ����� PNG.
    */
    const std::string source;

    /**
    * ������� ������� ��������, ��� / ��.
    * ������������� ������� ��������������, ������� ��� ����� ���������
    * �������� �������� (����. ���������������) �� X � Y.
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
    /**
    * @template T ��������� ���������� ����� ����� ����� �������
    *           ���� (double, float, int).
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
        const typelib::coord_t& areaMin = typelib::coord_t( 0.0f, 0.0f, 0.0f ),
        const typelib::coord_t& areaMax = typelib::coord_t( 0.0f, 0.0f, 0.0f )
    );




    virtual typelib::coord_t sizeMax() const;





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
