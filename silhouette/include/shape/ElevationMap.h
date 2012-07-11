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
    * ������ �� ����� �����.
    */
    typedef std::shared_ptr< ElevationMap >  Ptr;
    typedef std::unique_ptr< ElevationMap >  UPtr;



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
        const typelib::coordInt_t& c = bm_t::undefinedCoord(),
        size_t OSX = SX, size_t OSY = SY, size_t OSZ = SZ
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
