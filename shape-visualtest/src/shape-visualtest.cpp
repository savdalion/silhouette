#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <silhouette/include/ScaleBitMap.h>
#include <silhouette/include/Shaper.h>
#include <silhouette/include/shape/Ellipsoid.h>
#include <silhouette/include/shape/ElevationMap.h>
#include <typelib/include/mapcontent3d/BitMap.h>
#include <typelib/include/mapcontent3d/InverseFilterMap.h>
#include <typelib/include/mapcontent3d/NeightbourFilterMap.h>
#include <typelib/include/mapcontent3d/OutlineFilterMap.h>
#include <typelib/include/coord.h>
#include <typelib/include/size.h>
#include <silhouette/include/io/VolumeVTKVisual.h>
#include <silhouette/include/io/SurfaceVTKVisual.h>


/**
* Визуальное тестирование для 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // Для разделителя '.' вместо ','
    setlocale( LC_NUMERIC, "C" );


    using namespace siu;
    using namespace siu::shape;


    // (!) Форма и её визуализация, ничего лишнего


#ifdef ELLIPSOID_SHAPE_SIU_VISUALTEST
{
    const size_t SX = 81;
    const size_t SY = 81;
    const size_t SZ = SX;
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;

    const auto rz = 6000.0 * 1000.0;
    const auto rx = rz * 1.8;
    const auto ry = rz * 1.2;
    Ellipsoid< SX, SY, SZ >  elli( rx, ry, rz, true );
    const Shaper< SX, SY, SZ >  shaper( &elli );
    const bm_t bm = shaper.draw(
        //coord_t( -rx, 0.0f, -rz ),
        //coord_t( 0.0f, ry, rz )
    );

    // Визуализируем холст средствами VTK > http://vtk.org
/* - Переписано через json-параметризацию. См. ниже.
    siu::io::VolumeVTKVisual< 700, 2, true, true >  visual;
*/

    io::VolumeVTKVisual::option_t o;
    o[ "size-window" ] = 700;
    o[ "size-point" ] = 2;
    o[ "show-corner" ] = true;
    o[ "show-axes" ] = true;
    o[ "rgba" ] = 0x1111FFFF;

    io::VolumeVTKVisual visual( o );

    visual << bm;
    visual.wait();
}
#endif





#ifdef ELEVATION_MAP_SHAPE_SIU_VISUALTEST
{
    // @todo (!) Не корректно масштабирует при SX != SY и не пропорциональной исходной картинке.
    const size_t SX = 81;
    const size_t SY = 81;
    // т.к. высота много меньше размера поверхности, вводим "коэффициент для
    // наглядности"; чтобы увидеть реальный масштаб, пишем "clearness = 1".
    const size_t clearness = 5;
    const size_t SZ = SX * clearness;
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;

    //const std::string source = PATH_MEDIA + "test/a/gray-center-cavity.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-center-hill.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-max.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-min.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-middle.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-min-max.png";

    const std::string source = PATH_MEDIA + "mars/b/gray-elevation.png";
    // картинка "mars/b/gray-elevation.png" протяжённостью 400 пкс или ~200 км
    // (согласно масштабам Google Earth)
    // @source http://google.com/mars/#lat=-38.220919&lon=97.690429&zoom=7
    const double scaleXY = 200.0 / 400.0;
    // @todo Отрабатывать момент, когда размер по Z превышает размеры XY.
    const double hMin = -10.0;
    const double hMax = 20.0;
    ElevationMap< SX, SY, SZ >  elm( source, scaleXY, hMin, hMax, true );
    const Shaper< SX, SY, SZ >  shaper( &elm );
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
/* - Переписано через json-параметризацию. См. ниже.
#ifdef SURFACE_VISUAL_SIU_VISUALTEST
    io::SurfaceVTKVisual< 700, 1, true, true >  visual;

#else
    io::VolumeVTKVisual< 700, 2, true, true, 0x00000000 >  visual;
    //io::VolumeVTKVisual< 700, 2, true, true, 0xFFFFFFFF >  visual;

#endif
*/
    io::VolumeVTKVisual::option_t o;
    o[ "size-window" ] = 700;
    o[ "size-point" ] = 3;
    o[ "show-corner" ] = true;
    o[ "show-axes" ] = true;
    o[ "rgba" ] = 0x00000000;
    //o[ "rgba" ] = 0xFFFFFFFF;

    io::VolumeVTKVisual visual( o );

    visual << bm;

    visual.wait();
}
#endif





#ifdef SCALE_ELEVATION_MAP_SHAPE_SIU_VISUALTEST
{
    //const std::string source = PATH_MEDIA + "test/a/gray-center-cavity.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-center-hill.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-max.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-min.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-middle.png";
    const std::string source = PATH_MEDIA + "test/a/gray-min-max.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-zebra-1.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-zebra-2.png";
    //const std::string source = PATH_MEDIA + "test/a/gray-zebra-20.png";

    //const std::string source = PATH_MEDIA + "mars/b/gray-elevation.png";
    // картинка "mars/b/gray-elevation.png" протяжённостью 400 пкс или ~200 км
    // (согласно масштабам Google Earth)
    // @source http://google.com/mars/#lat=-38.220919&lon=97.690429&zoom=7
    // квадратная картинка
    const size_t sizeImage = 400;
    const double scaleXY = 200.0 / static_cast< double>( sizeImage );
    // @todo Отрабатывать момент, когда размер по Z превышает размеры XY.
    const double hMin = -10.0;
    const double hMax = 20.0;

    // верхний слой, всё изображение карты высот
    // @todo (!) Не корректно масштабирует при SX != SY и не пропорциональной исходной картинке.
    const size_t SX1 = 81;
    const size_t SY1 = 81;
    // т.к. высота много меньше размера поверхности, вводим "коэффициент для
    // наглядности"; чтобы увидеть реальный масштаб, пишем "clearness = 1".
    const size_t clearness1 = 3;
    const size_t SZ1 = SX1 * clearness1;
    typedef ScaleBitMap< SX1, SY1, SZ1 >  sbm1_t;

    sbm1_t a;
    a << ElevationMap< SX1, SY1, SZ1 >(
        source, scaleXY, hMin, hMax, true
    );

    // получаем образ верхнего слоя
    sbm1_t::bm_t bmA = a.draw();


    // нижний слой, часть изображения карты высот
    const size_t SX2 = SX1;
    const size_t SY2 = SY1;
    const size_t clearness2 = clearness1;
    const size_t SZ2 = SX2 * clearness2;
    typedef ScaleBitMap< SX2, SY2, SZ2 >  sbm2_t;

    // *каждая* ячейка верхнего слоя содержит SX2 * SY2 * SZ2 ячеек нижнего, поэтому:
    //   - два слоя содержат (SX2 * SY2 * SZ2) * (SX1 * SY1 * SZ1) ячеек
    //   - нижний слой в (SX2, SY2, SZ2) раз подробнее верхнего
    sbm2_t b;
    // указываем ячейку на верхнем слое, которую хотим отразить в нижнем
    // @todo Добавить правильное формирование частичной карты высот в зависимости от указанной высоты/глубины.
    const typelib::coordInt_t cA( 0, 0, 0 );
    // рассчитываем смещение и размер области в изображении-источнике карты высот
    const typelib::coordInt_t shiftB = cA + sbm2_t::bm_t::maxCoord();
    const typelib::psizeInt_t sizeB( sizeImage / SX2,  sizeImage / SY2,  0u );
    b << ElevationMap< SX1, SY1, SZ1 >(
        source, scaleXY, hMin, hMax, true,
        shiftB, sizeB
    );

    // получаем образ нижнего слоя
    sbm2_t::bm_t bmB = b.draw();



#if 0
    // Показываем всё
    // (не требует доп. кода)
#endif


#if 0
    // Оставляем только внешний контур
    typelib::OutlineFilterMapContent< true >  outline;
    outline( bmA );
    outline( bmB );
#endif


#if 0
    // Оставляем тех, рядом с которыми заданное кол-во соседей
    typelib::NeightbourFilterMapContent< 1, 10, true >  neighbour;
    outline( bmA );
    outline( bmB );
#endif


#if 0
    // Инвертируем
    typelib::InverseFilterMapContent  inverse;
    outline( bmA );
    outline( bmB );
#endif


#if 0
    // Эмулируем OutlineFilterMapContent без граничных стенок
    bm_t bmNeighbour( bm );
    typelib::NeightbourFilterMapContent< 26, 26, true >  neighbour;
    neighbour( bmNeighbour );
    bm ^= bmNeighbour;
#endif



    // Визуализируем холст средствами VTK > http://vtk.org
/* - Переписано через json-параметризацию. См. ниже.
#ifdef SURFACE_VISUAL_SIU_VISUALTEST
    io::SurfaceVTKVisual< 700, 1, true, true >  visual;

#else
    io::VolumeVTKVisual< 700, 2, true, true, 0x00000000 >  visual;
    //io::VolumeVTKVisual< 700, 2, true, true, 0xFFFFFFFF >  visual;

#endif
*/
    io::VolumeVTKVisual::option_t o;
    o[ "size-window" ] = 700;
    o[ "size-point" ] = 1;
    o[ "show-corner" ] = true;
    o[ "show-axes" ] = true;
    o[ "rgba" ] = 0x00000000;
    //o[ "rgba" ] = 0xFFFFFFFF;

#if 1
    io::VolumeVTKVisual visualA( o );
    visualA << bmA;
    visualA.wait();
#endif

#if 1
    io::VolumeVTKVisual visualB( o );
    visualB << bmB;
    visualB.wait();
#endif

}
#endif



    return 0;

} // main()
