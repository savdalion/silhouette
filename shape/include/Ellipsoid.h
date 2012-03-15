#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* Эллипосид.
*/
template< bool FillT >
class Ellipsoid : public Shape {
public:
    /**
    * Эллипсоид создаётся по *диаметрам*: может понадобится построить эллипсоид
    * размером в 1 целую ячейку.
    */
    inline Ellipsoid(
        size_t dx, size_t dy, size_t dz
    ) : dx( dx ), dy( dy ), dz( dz ) {
        assert( ( (dx > 0) && (dy > 0) && (dz > 0) )
            && "Все три диаметра должны быть указаны." );
    }


    virtual inline ~Ellipsoid() {
    }



#if 0
    // Поверхность строится параметрическим уравнением
    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && "Кол-во ячеек холста должно быть нечётным." );

        const int maxCanvas = (nCanvas - 1) / 2;
        BitContent3D bc( nCanvas );

        // @test Посчитаем множественные обращения к одной ячейке.
        size_t many = 0;

        // Соберём эллипсоид параметрическим уравнением
        const float a = static_cast< float >( dx ) / 2.0f;
        const float b = static_cast< float >( dy ) / 2.0f;
        const float c = static_cast< float >( dz ) / 2.0f;
        // @todo optimize Можно выбрать лучший шаг.
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

                // Размеры фигуры и холста, смещение фигуры могут создать
                // ситуацию, когда полученная координата окажется за
                // пределами холста. Это нормально, пропускаем.
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
        std::cout << "Отмечено точек в эллипсоиде: " << bc.count() << std::endl;
        std::cout << "Повторных попаданий при построении эллипсоида: " << many << std::endl;

        return bc;
    }

#else

    // Поверхность строится прохождением по сетке холста
    /**
    * @template FillT Эллипсоид будет заполнен.
    */
    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && "Кол-во ячеек холста должно быть нечётным." );

        const int maxCanvas = (nCanvas - 1) / 2;
        BitContent3D bc( nCanvas );

        // Соберём эллипсоид пробежав по всем ячейкам сетки, где он может располагаться
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
        std::cout << "Отмечено точек в эллипсоиде: " << bc.count() << std::endl;

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
