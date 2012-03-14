#pragma once

#include "PhysicsSolidES.h"


namespace siu {


/**
* ������� ������ � ���� �������� ����������� ����, ����� - ���������.
*/
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
