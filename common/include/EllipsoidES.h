#pragma once

#include "PhysicsSolidES.h"
#include <Ellipsoid.h>


namespace siu {


/**
* ������� ������ � ���� �������� ����������� ����, ����� - ���������.
*/
template< bool FillT >
struct EllipsoidES : public PhysicsSolidES {
    /**
    * @param nick �������� ��� ��� ������������� �������� ������ ������.
    *        ����� �� �����������, �.�. �� ������� ������ ���� �����, ���
    *        ���������� �������. ����� ��������� � ������ ������ �� �����
    *        �������������� ��������.
    */
    explicit inline EllipsoidES(
        const std::string& nick = ""
    ) :
        PhysicsSolidES( nick ),
        rx( 0.0 ),
        ry( 0.0 ),
        rz( 0.0 )
    {
    }



    inline ~EllipsoidES() {
    }



    /**
    * @see PhysicsSolidES
    */
    virtual psize_t psize() const {
        return psize_t(
            rx * 2.0,
            ry * 2.0,
            rz * 2.0
        );
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
        // ��������� ������� ���������� �� "������" � "������ ������"
        const auto ndx = static_cast< size_t >( std::ceil( rx / rsc1Cell * 2.0 + 0.001 ) );
        const auto ndy = static_cast< size_t >( std::ceil( ry / rsc1Cell * 2.0 + 0.001 ) );
        const auto ndz = static_cast< size_t >( std::ceil( rz / rsc1Cell * 2.0 + 0.001 ) );
        // ������� ���������� ������ ���������� ������������ ������ ������
        const RelativeCoord center = (
            (coordCanvas - coordElementSketch) / rsc1Cell
        );

        // ��������� ��������� � �������� ������� ������
        const shape::Ellipsoid< FillT >  ellipsoid( ndx, ndy, ndz );
        const BitContent3D bc = ellipsoid.draw(
            nCanvas,
            static_cast< int >( std::ceil( center.x + 0.001 ) ),
            static_cast< int >( std::ceil( center.y + 0.001 ) ),
            static_cast< int >( std::ceil( center.z + 0.001 ) )
        );

        // @test
        //const size_t count = bc.count();

        return bc;
    }




    /**
    * @see PhysicsSolidES
    */
    inline virtual double mass() const {
        return density * volume();
    }



    /**
    * @see PhysicsSolidES
    */
    inline virtual double volume() const {
        return 4.0 / 3.0 * M_PI * rx * ry * rz;
    }




    /**
    * �������� ���������� ������� ����������, �.
    */
    double rx;
    double ry;
    double rz;

};


}
