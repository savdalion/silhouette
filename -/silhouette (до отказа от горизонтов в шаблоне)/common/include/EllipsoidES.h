#pragma once

#include "PhysicsSolidES.h"


namespace siu {


/**
* Элемент эскиза в виде цельного физического тела, форма - эллипсоид.
*/
struct EllipsoidES : public PhysicsSolidES {
    /**
    * @param nick Короткое имя для идентификации элемента внутри эскиза.
    *        Может не указываться, т.к. не всякому эскизу надо знать, как
    *        называется элемент. Поиск элементов в эскизе ведётся по любой
    *        характеристике элемента.
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
    * Реальные физические радиусы эллипсоида, м.
    */
    double rx;
    double ry;
    double rz;

};


}
