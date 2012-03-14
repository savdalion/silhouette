#pragma once

#include "ElementSketch.h"


namespace siu {


/**
* ������� ������ � ���� �������� ����������� ����.
*/
struct PhysicsSolidES : public ElementSketch {

    explicit inline PhysicsSolidES(
        const std::string& nick
    ) :
        ElementSketch( nick ),
        density( 0.0 )
    {
    }



    inline ~PhysicsSolidES() {
    }

    


    /**
    * ��������� ����, �� / �^3.
    */
    double density;



    /**
    * @return ����� ����.
    */
    virtual double mass() const = 0;


    /**
    * @return ����� ����, �^3.
    */
    virtual double volume() const = 0;

};


}
