#pragma once

#include "ElementSketch.h"


namespace siu {


/**
* ������� ������ � ���� �������� ����������� ����.
*/
struct PhysicsSolidES : public ElementSketch {

    typedef double density_t;



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
    density_t density;



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
