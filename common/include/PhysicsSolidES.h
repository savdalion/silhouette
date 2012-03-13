#pragma once

#include "ElementSketch.h"


namespace siu {


/**
* Элемент эскиза в виде цельного физического тела.
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
    * Плотность тела, кг / м^3.
    */
    density_t density;



    /**
    * @return Масса тела.
    */
    virtual double mass() const = 0;


    /**
    * @return Объём тела, м^3.
    */
    virtual double volume() const = 0;

};


}
