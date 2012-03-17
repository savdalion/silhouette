#pragma once

#include "ElementSketch.h"


namespace siu {


/**
* Элемент эскиза в виде цельного физического тела.
*/
struct PhysicsSolidES : public ElementSketch {

    explicit inline PhysicsSolidES(
        const std::string& nick,
        const matter_t& matter
    ) :
        ElementSketch( nick, matter ),
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
    * @see PhysicsSolidES
    */
    inline virtual double mass() const {
        return density * volume();
    }



    /**
    * @return Объём тела, м^3.
    */
    virtual double volume() const = 0;

};


}
