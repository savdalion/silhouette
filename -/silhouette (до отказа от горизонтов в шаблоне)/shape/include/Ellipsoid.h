#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* Эллипосид.
*/
template< size_t N, size_t aT, size_t bT, size_t cT >
class Ellipsoid : public Shape {
public:
    inline Ellipsoid() {
        static_assert( ( (aT * 2 <= N) && (bT * 2 <= N) && (cT * 2 <= N) ),
            "Эллипсоид не помещается на холст." );
    }


    virtual inline ~Ellipsoid() {
    }



    inline topology_t draw() const {
        bm::bvector<> r( N * N * N );

        // @test Посчитаем множественные обращения к одной ячейке.
        size_t many = 0;

        // соберём эллипсоид параметрическим уравнением
        const float a = (float)aT / 2.0f - 1.0f;
        const float b = (float)bT / 2.0f - 1.0f;
        const float c = (float)cT / 2.0f - 1.0f;
        // @todo optimize Можно выбрать лучший шаг.
        const float step = 1.0f / (a + b + c) / 2.0f;
        for (float u = -M_PI / 2.0f; u <= M_PI / 2.0; u += step) {
            const float cu = std::cosf( u );
            const float su = std::sinf( u );
            for (float v = -M_PI; v <= M_PI; v += step) {
                const float cv = std::cosf( v );
                const float sv = std::sinf( v );
                const int x = (int)( a * cu * cv ) + Canvas< N >::max();
                const int y = (int)( b * cu * sv ) + Canvas< N >::max();
                const int z = (int)( c * su      ) + Canvas< N >::max();

                // @todo verify Такого быть не должно.
                if ( Canvas< N >::outside( x, y, z ) ) {
                    continue;
                }

                const int i = Canvas< N >::ic( x, y, z );
                
                // @test
                if ( r.test( i ) ) {
                    ++many;
                }

                r.set( i );
            }
        }

        return r;
    }



};


    } // shape

} // siu
