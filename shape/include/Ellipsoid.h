#pragma once

#include "Shape.h"


namespace siu {
    namespace shape {

/**
* ���������.
*
* @template ��. Shape
*/
template< size_t Grid >
class Ellipsoid : public Shape< Grid > {
public:
    /**
    * �������.
    */
    const float rx;
    const float ry;
    const float rz;

    /**
    * ����� �� �������� �������� ������.
    */
    const bool fill;



public:
    template< typename IT >
    inline Ellipsoid(
        IT rx, IT ry, IT rz,
        bool fill
    ) :
        rx( static_cast< float >( rx ) ),
        ry( static_cast< float >( ry ) ),
        rz( static_cast< float >( rz ) ),
        fill( fill )
    {
        assert( ( (rx > 0.0f) && (ry > 0.0f) && (rz > 0.0f) )
            && "��� ��� ������� ������ ���� ������ 0." );
    }


    virtual inline ~Ellipsoid() {
    }



#if 0
    // ��������� �������� ��������������� ����������
    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && "���-�� ����� ������ ������ ���� ��������." );

        const int maxCanvas = (nCanvas - 1) / 2;
        BitContent3D bc( nCanvas );

        // @test ��������� ������������� ��������� � ����� ������.
        size_t many = 0;

        // ������ ��������� ��������������� ����������
        const float a = static_cast< float >( dx ) / 2.0f;
        const float b = static_cast< float >( dy ) / 2.0f;
        const float c = static_cast< float >( dz ) / 2.0f;
        // @todo optimize ����� ������� ������ ���.
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

                // ������� ������ � ������, �������� ������ ����� �������
                // ��������, ����� ���������� ���������� �������� ��
                // ��������� ������. ��� ���������, ����������.
                if ( bc.outside( x, y, z ) ) {
                    continue;
                }

                // ���������� �� ������
                const size_t i = bc.ic( x, y, z );
                assert( (i < bc.N3) && "i �� ��������� ������" );
                
                // @test
                if ( bc.test( i ) ) {
                    ++many;
                }

                bc.set( i );
            }
        }

        // @test
        std::cout << "�������� ����� � ����������: " << bc.count() << std::endl;
        std::cout << "��������� ��������� ��� ���������� ����������: " << many << std::endl;

        return bc;
    }

#endif



#if 0
    /**
    * @virtual Shape
    *
    * ��������� �������� ������������ �� ������� ����� G.
    */
    virtual inline bm_t operator()() {

        static const float grid = static_cast< float >( Grid );

        // �������� ������ 1 ������
        const float sizeG = 2.0f * rMax / grid;

        // ��������������� ������
        const float normalRX = rx / rMax;
        const float normalRY = ry / rMax;
        const float normalRZ = rz / rMax;

        // ��������� ���������� �����
        // (0, 0, 0) - �����
        const int maxGCoord = (static_cast< int >( Grid ) - 1) / 2;
        //const int minGCoord = -maxGCoord;

        // �������� ������������ ������
        const typelib::coordInt_t shift( 0, 0, 0 );

        // �������� � ������������ ���������� 'bm'

        // ������ ��������� �������� �� ���� ������� �����, ��� �� *�����* �������������
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
                // ����� �� ��������� �����
                continue;
            }
            const float z2 = z * z;
            const float zr = z2 / c2;
            for (float y = -static_cast< float >( b ); y <= static_cast< float >( b ); y += step) {
                const int yy = static_cast< int >( y ) + shift.y;
                if ( (yy < -maxGCoord) || (yy > maxGCoord) ) {
                    // ����� �� ��������� ������
                    continue;
                }
                const float y2 = y * y;
                const float yr = y2 / b2;
                for (float x = -static_cast< float >( a ); x <= static_cast< float >( a ); x += step) {
                    const int xx = static_cast< int >( x ) + shift.x;
                    if ( (xx < -maxGCoord) || (xx > maxGCoord) ) {
                        // ����� �� ��������� ������
                        continue;
                    }
                    const float x2 = x * x;
                    const float xr = x2 / a2;
                    const float t = xr + yr + zr;

                    if ( fill ) {
                        // �������� �������
                        if (t < 1.001f) {
                            bm.set( xx, yy, zz );
                        }
                    } else {
                        // ������ �����������
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



    /**
    * @virtual Shape
    *
    * ��������� �������� ������������ �� *����* ������� ����� G.
    */
    virtual inline bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t( 0.0f, 0.0f, 0.0f ),
        const typelib::coord_t& areaMax = typelib::coord_t( 0.0f, 0.0f, 0.0f )
    ) {
        /* ��������������� ������
        const float normalRX = rx / rMax;
        const float normalRY = ry / rMax;
        const float normalRZ = rz / rMax;
        */

        // ��������� ���������� �����
        // (0, 0, 0) - �����
        const int maxGCoord = (static_cast< int >( Grid ) - 1) / 2;
        const int minGCoord = -maxGCoord;

        // ������� ����� ������, ���� ������� �� ������ �������
        if ( !empty(areaMin - areaMax) ) {
            // (!) ������ ����������, ����� � ����� ��������� �������� ���� ����� ������.
            assert( false && "@todo �� �����������." );
        }

        // �������� ������������ ������
        const typelib::coordInt_t shift( 0, 0, 0 );

        // �������� � ������������ ���������� 'bm'

        const float sizeG = sizeGrid();
        const float rx2 = rx * rx / (sizeG * sizeG);
        const float ry2 = ry * ry / (sizeG * sizeG);
        const float rz2 = rz * rz / (sizeG * sizeG);
        for (int z = minGCoord; z <= maxGCoord; ++z) {
            const float tz = static_cast< float >( z * z ) / rz2;
            for (int y = minGCoord; y <= maxGCoord; ++y) {
                const float ty = static_cast< float >( y * y ) / ry2;
                for (int x = minGCoord; x <= maxGCoord; ++x) {
                    /* - @todo �������������� �������. ��. @todo ����.
                    if ( outside( x, y, z, areaMin, areaMax ) ) {
                        continue;
                    }
                    */
                    const float tx = static_cast< float >( x * x ) / rx2;
                    const float t = tx + ty + tz;
                    if ( fill ) {
                        // �������� �������
                        if (t < 1.001f) {
                            bm.set( x, y, z );
                        }
                    } else {
                        // ������ �����������
                        if ( (t < 1.001f) && (t > 0.999f) ) {
                            bm.set( x, y,  z );
                        }
                    }

                }
            }
        }

        return bm;
    }





    virtual inline float sizeMax() const {
        const float rMax = std::max( std::max( rx, ry ), rz );
        return 2.0f * rMax;
    }


};


    } // shape

} // siu
