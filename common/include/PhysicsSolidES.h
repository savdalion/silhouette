#pragma once

#include "ElementSketch.h"


namespace siu {


/**
* Элемент эскиза в виде цельного физического тела.
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
    * Плотность тела, кг / м^3.
    */
    double density;



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
