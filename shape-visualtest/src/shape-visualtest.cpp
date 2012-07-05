#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Shaper.h>
#include <Ellipsoid.h>
#include <ElevationMap.h>
#include <InverseFilterMapContent.h>
#include <NeightbourFilterMapContent.h>
#include <OutlineFilterMapContent.h>
#include <VTKVisual.h>
#include <SurfaceVTKVisual.h>


/**
* Визуальное тестирование для 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // Для разделителя '.' вместо ','
    setlocale( LC_NUMERIC, "C" );


    using namespace siu;
    using namespace siu::common;
    using namespace siu::shape;


    // (!) Форма и её визуализация, ничего лишнего


#ifdef ELLIPSOID_SHAPE_SIU_VISUALTEST
{
    const size_t GRID = 81;
    const auto rz = 6000.0 * 1000.0;
    const auto rx = rz * 1.8;
    const auto ry = rz * 1.2;
    Ellipsoid< GRID >  elli( rx, ry, rz, true );
    const Shaper< GRID >  shaper( &elli );
    // извлекаем квадрант эллипса
    const auto bm = shaper.draw(
        //coord_t( -rx, 0.0f, -rz ),
        //coord_t( 0.0f, ry, rz )
    );

    // Визуализируем холст средствами VTK > http://vtk.org
    siu::io::VTKVisual< 700, 2, true, true >  visual;
    visual << bm;
    visual.wait();
}
#endif




#ifdef ELEVATION_MAP_SHAPE_SIU_VISUALTEST
{
    const size_t GRID = 81;
    typedef BitMap< GRID >  bm_t;

    //const std::string source = PATH_MEDIA + "test/a/gray-center-circle.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-max.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-min.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-middle.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-min-max.png";

    const std::string source = PATH_MEDIA + "mars/b/gray-elevation.png";
    // картинка "mars/b/gray-elevation.png" протяжённостью 400 пкс или ~200 км
    // (согласно масштабам Google Earth)
    // @source http://google.com/mars/#lat=-38.220919&lon=97.690429&zoom=7
    const double scaleXY = 200.0 / 400.0;
    // т.к. высота много меньше размера поверхности, вводим "коэффициент для
    // наглядности"; чтобы увидеть реальный масштаб, пишем "clearness = 1".
    // @todo Отрабатывать момент, когда размер по Z превышает размеры XY.
    const double clearness = 5;
    const double hMin = -10.0 * clearness;
    const double hMax = 20.0 * clearness;
    ElevationMap< GRID >  elm( source, scaleXY, hMin, hMax, true );
    const Shaper< GRID >  shaper( &elm );
    bm_t bm = shaper.draw();


#if 0
    // Показываем всё
    // (не требует доп. кода)
#endif


#if 1
    // Оставляем только внешний контур
    typelib::OutlineFilterMapContent< true >  outline;
    outline( bm );
#endif


#if 0
    // Оставляем тех, рядом с которыми заданное кол-во соседей
    typelib::NeightbourFilterMapContent< 1, 10, true >  neighbour;
    neighbour( bm );
#endif


#if 0
    // Инвертируем
    typelib::InverseFilterMapContent  inverse;
    inverse( bm );
#endif


#if 0
    // Эмулируем OutlineFilterMapContent без граничных стенок
    bm_t bmNeighbour( bm );
    typelib::NeightbourFilterMapContent< 26, 26, true >  neighbour;
    neighbour( bmNeighbour );
    bm ^= bmNeighbour;
#endif



    // Визуализируем холст средствами VTK > http://vtk.org
#ifdef SURFACE_VISUAL_SIU_VISUALTEST
    io::SurfaceVTKVisual< 700, 1, true, true >  visual;
#else
    io::VTKVisual< 700, 2, true, true, 0x00000000 >  visual;
    //io::VTKVisual< 700, 2, true, true, 0xFFFFFFFF >  visual;
#endif
    visual << bm;
    visual.wait();
}
#endif



    return 0;

} // main()
