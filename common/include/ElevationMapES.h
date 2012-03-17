#pragma once

#include "PhysicsSolidES.h"
#include <ElevationMap.h>


namespace siu {


/**
* ������� ������ � ���� �������� ����������� ����, ����� - ���������.
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
            && "����� � ������� ������ ������ ���� ��������� � ������ ���������." );

        // ����� �������� ������ ������� 1 ������ ������, �
        const double rsc1Cell = realSizeCanvas / static_cast< double >( nCanvas );
        // ��������� ������� ����� ����� �� "������" � "������ ������". �����
        // ����� ���� �������� �������, ��� �����, ������� �����������
        // �������������� �����������.

        const auto nsxy = static_cast< size_t >( std::ceil( sizeXY / rsc1Cell ) );
        const auto nh = static_cast< size_t >( std::ceil( height / rsc1Cell ) );
        // ������� ���������� ������ ����� ����� ������������ ������ ������
        const RelativeCoord center = (
            (coordCanvas - coordElementSketch) / rsc1Cell
        );

        // ��������� ����� ����� � �������� ������� ������
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
        // @todo ������� ����� ������.
        return sizeXY * sizeXY * height;
    }




    /**
    * ���� � ��������� - ����� PNG - ��� ���������� ������� �� ����� �����.
    */
    std::string source;


    /**
    * ������ �������� ������, ��������� ������ �����, �.
    *
    * @todo ��������� �� ������ ���������� �������.
    */
    double sizeXY;


    /**
    * ������ ���� ����� �����, �.
    */
    double height;

};


}
