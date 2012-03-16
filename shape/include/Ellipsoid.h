#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* Ёллипосид.
*
* @template FillT Ёллипсоид будет заполнен.
*/
template< bool FillT >
class Ellipsoid : public Shape {
public:
    /**
    * Ёллипсоид создаЄтс€ по *диаметрам*: может понадобитс€ построить
    * эллипсоид размером в 1 целую €чейку.
    */
    inline Ellipsoid(
        size_t dx, size_t dy, size_t dz
    ) : dx( dx ), dy( dy ), dz( dz ) {
        assert( ( (dx > 0) && (dy > 0) && (dz > 0) )
            && "¬се три диаметра должны быть указаны." );
    }


    virtual inline ~Ellipsoid() {
    }



#if 0
    // Ёллипсоид строитс€ параметрическим уравнением
    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && " ол-во €чеек холста должно быть нечЄтным." );

        const int maxCanvas = (nCanvas - 1) / 2;
        BitContent3D bc( nCanvas );

        // @test ѕосчитаем множественные обращени€ к одной €чейке.
        size_t many = 0;

        // —оберЄм эллипсоид параметрическим уравнением
        const float a = static_cast< float >( dx ) / 2.0f;
        const float b = static_cast< float >( dy ) / 2.0f;
        const float c = static_cast< float >( dz ) / 2.0f;
        // @todo optimize ћожно выбрать лучший шаг.
        const float step = 1.0f / (a + b + c) / 2.0f;
        for (float u = -M_PI / 2.0f; u <= M_PI / 2.0; u += step) {
            const float cu = std::cosf( u );
            const float su = std::sinf( u );
            for (float v = -M_PI; v <= M_PI; v += step) {
                const float cv = std::cosf( v );
                const float sv = std::sinf( v );
                const int x = static_cast< int >( a * cu * cv ) + cx + maxCanvas;
                const int y = static_cast< int >( b * cu * sv ) + cy + maxCanvas;
                const int z = static_cast< int >( c * su      ) + cz + maxCanvas;

                // –азмеры фигуры и холста, смещение фигуры могут создать
                // ситуацию, когда полученна€ координата окажетс€ за
                // пределами холста. Ёто нормально, пропускаем.
                if ( bc.outside( x, y, z ) ) {
                    continue;
                }

                // координата на холсте
                const size_t i = bc.ic( x, y, z );
                assert( (i < bc.N3) && "i за пределами холста" );
                
                // @test
                if ( bc.test( i ) ) {
                    ++many;
                }

                bc.set( i );
            }
        }

        // @test
        std::cout << "ќтмечено точек в эллипсоиде: " << bc.count() << std::endl;
        std::cout << "ѕовторных попаданий при построении эллипсоида: " << many << std::endl;

        return bc;
    }

#else

    // Ёллипсоид строитс€ прохождением по сетке холста
    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && " ол-во €чеек холста должно быть нечЄтным." );

        const int maxCanvas = (nCanvas - 1) / 2;
        BitContent3D bc( nCanvas );

        // —оберЄм эллипсоид пробежав по всем €чейкам сетки, где он может располагатьс€
        const size_t a = static_cast< size_t >( std::floor( static_cast< float >( dx ) / 2.0f + 0.001 ) );
        const size_t b = static_cast< size_t >( std::floor( static_cast< float >( dy ) / 2.0f + 0.001 ) );
        const size_t c = static_cast< size_t >( std::floor( static_cast< float >( dz ) / 2.0f + 0.001 ) );
        const float a2 = static_cast< float >( a * a );
        const float b2 = static_cast< float >( b * b );
        const float c2 = static_cast< float >( c * c );
        const float step = 1.0f;
        for (float z = -static_cast< float >( c ); z <= static_cast< float >( c ); z += step) {
            const int zz = static_cast< int >( z ) + cz;
            if ( (zz < -maxCanvas) || (zz > maxCanvas) ) {
                // точка за границами холста
                continue;
            }
            const float z2 = z * z;
            const float zr = z2 / c2;
            for (float y = -static_cast< float >( b ); y <= static_cast< float >( b ); y += step) {
                const int yy = static_cast< int >( y ) + cy;
                if ( (yy < -maxCanvas) || (yy > maxCanvas) ) {
                    // точка за границами холста
                    continue;
                }
                const float y2 = y * y;
                const float yr = y2 / b2;
                for (float x = -static_cast< float >( a ); x <= static_cast< float >( a ); x += step) {
                    const int xx = static_cast< int >( x ) + cx;
                    if ( (xx < -maxCanvas) || (xx > maxCanvas) ) {
                        // точка за границами холста
                        continue;
                    }
                    const float x2 = x * x;
                    const float xr = x2 / a2;
                    const float t = xr + yr + zr;

                    if ( FillT ) {
                        // заполнен целиком
                        if (t < 1.001f) {
                            bc.set( xx + maxCanvas,  yy + maxCanvas,  zz + maxCanvas );
                        }
                    } else {
                        // только поверхность
                        if ( (t < 1.001f) && (t > 0.999) ) {
                            bc.set( xx + maxCanvas,  yy + maxCanvas,  zz + maxCanvas );
                        }
                    }
                }
            }
        } // for (float z = -static_cast< float >( c ); z <= static_cast< float >( c ); z += step)

        // @test
        std::cout << "ќтмечено точек в эллипсоиде: " << bc.count() << std::endl;

        return bc;
    }

#endif






protected:
    const size_t dx;
    const size_t dy;
    const size_t dz;

};


    } // shape

} // siu
