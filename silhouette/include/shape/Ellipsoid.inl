namespace siu {
    namespace shape {


template< size_t SX, size_t SY, size_t SZ >
template< typename T, typename S >
inline Ellipsoid< SX, SY, SZ >::Ellipsoid(
    T rx, T ry, T rz,
    bool fill,
    S scale
) :
    rx( static_cast< float >( rx ) ),
    ry( static_cast< float >( ry ) ),
    rz( static_cast< float >( rz ) ),
    fill( fill ),
    scale( static_cast< float >( scale ) )
{
    assert( ( (rx > 0.0f) && (ry > 0.0f) && (rz > 0.0f) )
        && "Все три радиуса должны быть больше 0." );
    assert( (scale > 0.0f)
        && "Масштаб эллипсоида должен быть больше 0." );
}




template< size_t SX, size_t SY, size_t SZ >
inline Ellipsoid< SX, SY, SZ >::~Ellipsoid() {
}




#if 0
    // Эллипсоид строится параметрическим уравнением
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

#endif




#if 0
    /**
    * @virtual Shape
    *
    * Эллипсоид строится прохождением по ячейкам сетки G.
    */
    virtual inline bm_t operator()() {

        static const float grid = static_cast< float >( Grid );

        // реальный размер 1 ячейки
        const float sizeG = 2.0f * rMax / grid;

        // нормализованный размер
        const float normalRX = rx / rMax;
        const float normalRY = ry / rMax;
        const float normalRZ = rz / rMax;

        // граничные координаты сетки
        // (0, 0, 0) - центр
        const int maxGCoord = (static_cast< int >( Grid ) - 1) / 2;
        //const int minGCoord = -maxGCoord;

        // смещение относительно центра
        const typelib::coordInt_t shift( 0, 0, 0 );

        // Работаем с родительской структурой 'bm'

        // Соберём эллипсоид пробежав по всем ячейкам сетки, где он *может* располагаться
        const size_t a = static_cast< size_t >( std::floor( normalRX * grid + 0.001f ) );
        const size_t b = static_cast< size_t >( std::floor( normalRY * grid + 0.001f ) );
        const size_t c = static_cast< size_t >( std::floor( normalRZ * grid + 0.001f ) );
        const float a2 = static_cast< float >( a * a );
        const float b2 = static_cast< float >( b * b );
        const float c2 = static_cast< float >( c * c );
        const float step = 1.0f;
        for (float z = -static_cast< float >( c ); z <= static_cast< float >( c ); z += step) {
            const int zz = static_cast< int >( z ) + shift.z;
            if ( (zz < -maxGCoord) || (zz > maxGCoord) ) {
                // точка за границами карты
                continue;
            }
            const float z2 = z * z;
            const float zr = z2 / c2;
            for (float y = -static_cast< float >( b ); y <= static_cast< float >( b ); y += step) {
                const int yy = static_cast< int >( y ) + shift.y;
                if ( (yy < -maxGCoord) || (yy > maxGCoord) ) {
                    // точка за границами холста
                    continue;
                }
                const float y2 = y * y;
                const float yr = y2 / b2;
                for (float x = -static_cast< float >( a ); x <= static_cast< float >( a ); x += step) {
                    const int xx = static_cast< int >( x ) + shift.x;
                    if ( (xx < -maxGCoord) || (xx > maxGCoord) ) {
                        // точка за границами холста
                        continue;
                    }
                    const float x2 = x * x;
                    const float xr = x2 / a2;
                    const float t = xr + yr + zr;

                    if ( fill ) {
                        // заполнен целиком
                        if (t < 1.001f) {
                            bm.set( xx, yy, zz );
                        }
                    } else {
                        // только поверхность
                        if ( (t < 1.001f) && (t > 0.999f) ) {
                            bm.set( xx, yy,  zz );
                        }
                    }
                }
            }
        } // for (float z = -static_cast< float >( c ); z <= static_cast< float >( c ); z += step)

        return bm;
    }
#endif




template< size_t SX, size_t SY, size_t SZ >
inline typename Ellipsoid< SX, SY, SZ >::bm_t Ellipsoid< SX, SY, SZ >::operator()() {
    /* нормализованный размер
    const float normalRX = rx / rMax;
    const float normalRY = ry / rMax;
    const float normalRZ = rz / rMax;
    */

    // граничные координаты сетки
    // (0, 0, 0) - центр
    const typelib::coordInt_t maxCoord = bm_t::maxCoord();
    const typelib::coordInt_t minCoord = -maxCoord;

    // смещение относительно центра
    const typelib::coordInt_t shift( 0, 0, 0 );

    // Работаем с родительской структурой 'bm'

    const typelib::coord_t sizeG = sizeGrid();
    const float maxSizeG2 = sizeG.max() * sizeG.max();
    const float rx2 = rx * scale * rx * scale / maxSizeG2;
    const float ry2 = ry * scale * ry * scale / maxSizeG2;
    const float rz2 = rz * scale * rz * scale / maxSizeG2;
    for (int z = minCoord.z * scale; z <= maxCoord.z * scale; ++z) {
        const float tz = static_cast< float >( z * z ) / rz2;
        for (int y = minCoord.y * scale; y <= maxCoord.y * scale; ++y) {
            const float ty = static_cast< float >( y * y ) / ry2;
            for (int x = minCoord.x * scale; x <= maxCoord.x * scale; ++x) {
                /* - @todo Контролировать области. См. @todo выше.
                if ( outside( x, y, z, areaMin, areaMax ) ) {
                    continue;
                }
                */
                const float tx = static_cast< float >( x * x ) / rx2;
                const float t = tx + ty + tz;
                if ( fill ) {
                    // заполнен целиком
                    if (t < (1.0f + typelib::PRECISION)) {
                        bm.set( x, y, z );
                    }
                } else {
                    // только поверхность
                    //if ( (t < (1.0f + typelib::PRECISION)) && (t > (1.0f - typelib::PRECISION)) ) {
                    if ( typelib::between( t,  1.0f - typelib::PRECISION,  1.0f + typelib::PRECISION ) ) {
                        bm.set( x, y, z );
                    }
                }

            }
        }
    }

    return bm;
}





template< size_t SX, size_t SY, size_t SZ >
inline typelib::coord_t Ellipsoid< SX, SY, SZ >::sizeMax() const {
    return typelib::coord_t( rx, ry, rz ) * 2.0f;
}






template< size_t SX, size_t SY, size_t SZ >
inline typename Shape< SX, SY, SZ >::Ptr  Ellipsoid< SX, SY, SZ >::clone() const {
    return Ellipsoid< SX, SY, SZ >::Ptr( new Ellipsoid< SX, SY, SZ >( *this ) );
}



    } // shape

} // siu
