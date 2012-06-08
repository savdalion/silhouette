#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Shaper.h>
#include <Ellipsoid.h>
#include <ElevationMap.h>
#include <OutlineFilterMapContent.h>
#include <VTKVisual.h>
#include <SurfaceVTKVisual.h>


/**
* ���������� ������������ ��� 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // ��� ����������� '.' ������ ','
    setlocale( LC_NUMERIC, "C" );


    using namespace siu;
    using namespace siu::common;
    using namespace siu::shape;


    // (!) ����� � � ������������, ������ �������


#ifdef ELLIPSOID_SHAPE_SIU_VISUALTEST
{
    const size_t GRID = 81;
    const auto rz = 6000.0 * 1000.0;
    const auto rx = rz * 1.8;
    const auto ry = rz * 1.2;
    Ellipsoid< GRID >  elli( rx, ry, rz, true );
    const Shaper< GRID >  shaper( &elli );
    // ��������� �������� �������
    const auto bm = shaper.draw(
        //coord_t( -rx, 0.0f, -rz ),
        //coord_t( 0.0f, ry, rz )
    );

    // ������������� ����� ���������� VTK > http://vtk.org
    siu::io::VTKVisual< 700, 2, true, true >  visual;
    visual << bm;
    visual.wait();
}
#endif




#ifdef ELEVATION_MAP_SHAPE_SIU_VISUALTEST
{
    const size_t GRID = 81;
    const std::string source = PATH_MEDIA + "mars/b/gray-elevation.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-max.png";
    // �������� ������������� 400 ��� ��� ~200 �� �������� Google Earth
    // @source http://google.com/mars/#lat=-38.220919&lon=97.690429&zoom=7
    const auto scaleXY = 200.0 / 400.0;
    // @todo �� ������������ ��������� �������. ���������.
    const auto hMin = -10.0;
    const auto hMax = 20.0;
    ElevationMap< GRID >  elm( source, scaleXY, hMin, hMax, true );
    const Shaper< GRID >  shaper( &elm );
    auto bm = shaper.draw();

    // ��������� ������ ������� ������
    OutlineFilterMapContent fmc;
    fmc( bm );


    // ������������� ����� ���������� VTK > http://vtk.org
#ifdef SURFACE_VISUAL_SIU_VISUALTEST
    siu::io::SurfaceVTKVisual< 700, 1, true, true >  visual;
#else
    siu::io::VTKVisual< 700, 1, true, true >  visual;
#endif
    visual << bm;
    visual.wait();
}
#endif



    return 0;

} // main()
