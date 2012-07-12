#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <mapcontent3d/BitMap.h>
#include "ScaleBitMap.h"
#include <Shaper.h>
#include <shape/Ellipsoid.h>
#include <shape/ElevationMap.h>
#include <mapcontent3d/InverseFilterMap.h>
#include <mapcontent3d/NeightbourFilterMap.h>
#include <mapcontent3d/OutlineFilterMap.h>
#include <coord.h>
#include <size.h>
#include <io/VolumeVTKVisual.h>
#include <io/SurfaceVTKVisual.h>


/**
* ���������� ������������ ��� 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // ��� ����������� '.' ������ ','
    setlocale( LC_NUMERIC, "C" );


    using namespace siu;
    using namespace siu::shape;


    // (!) ����� � � ������������, ������ �������


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

    // ������������� ����� ���������� VTK > http://vtk.org
/* - ���������� ����� json-��������������. ��. ����.
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
    // @todo (!) �� ��������� ������������ ��� SX != SY � �� ���������������� �������� ��������.
    const size_t SX = 81;
    const size_t SY = 81;
    // �.�. ������ ����� ������ ������� �����������, ������ "����������� ���
    // �����������"; ����� ������� �������� �������, ����� "clearness = 1".
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
    // �������� "mars/b/gray-elevation.png" ������������� 400 ��� ��� ~200 ��
    // (�������� ��������� Google Earth)
    // @source http://google.com/mars/#lat=-38.220919&lon=97.690429&zoom=7
    const double scaleXY = 200.0 / 400.0;
    // @todo ������������ ������, ����� ������ �� Z ��������� ������� XY.
    const double hMin = -10.0;
    const double hMax = 20.0;
    ElevationMap< SX, SY, SZ >  elm( source, scaleXY, hMin, hMax, true );
    const Shaper< SX, SY, SZ >  shaper( &elm );
    bm_t bm = shaper.draw();


#if 0
    // ���������� ��
    // (�� ������� ���. ����)
#endif


#if 1
    // ��������� ������ ������� ������
    typelib::OutlineFilterMapContent< true >  outline;
    outline( bm );
#endif


#if 0
    // ��������� ���, ����� � �������� �������� ���-�� �������
    typelib::NeightbourFilterMapContent< 1, 10, true >  neighbour;
    neighbour( bm );
#endif


#if 0
    // �����������
    typelib::InverseFilterMapContent  inverse;
    inverse( bm );
#endif


#if 0
    // ��������� OutlineFilterMapContent ��� ��������� ������
    bm_t bmNeighbour( bm );
    typelib::NeightbourFilterMapContent< 26, 26, true >  neighbour;
    neighbour( bmNeighbour );
    bm ^= bmNeighbour;
#endif



    // ������������� ����� ���������� VTK > http://vtk.org
/* - ���������� ����� json-��������������. ��. ����.
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
    // @todo (!) �� ��������� ������������ ��� SX != SY � �� ���������������� �������� ��������.
    const size_t SX1 = 81;
    const size_t SY1 = 81;
    // �.�. ������ ����� ������ ������� �����������, ������ "����������� ���
    // �����������"; ����� ������� �������� �������, ����� "clearness = 1".
    const size_t clearness1 = 1;
    const size_t SZ1 = SX1 * clearness1;
    typedef ScaleBitMap< SX1, SY1, SZ1 >  sbm1_t;

    const size_t SX2 = SX1;
    const size_t SY2 = SY1;
    const size_t clearness2 = clearness1;
    const size_t SZ2 = SX2 * clearness2;
    typedef ScaleBitMap< SX2, SY2, SZ2 >  sbm2_t;

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
    // �������� "mars/b/gray-elevation.png" ������������� 400 ��� ��� ~200 ��
    // (�������� ��������� Google Earth)
    // @source http://google.com/mars/#lat=-38.220919&lon=97.690429&zoom=7
    // ���������� ��������
    const size_t sizeImage = 400;
    const double scaleXY = 200.0 / static_cast< double>( sizeImage );
    // @todo ������������ ������, ����� ������ �� Z ��������� ������� XY.
    const double hMin = -10.0;
    const double hMax = 20.0;

    // ������� ����, �� ����������� ����� �����
    sbm1_t a;
    a << ElevationMap< SX1, SY1, SZ1 >(
        source, scaleXY, hMin, hMax, true
    );

    // ������ ����, ����� ����������� ����� �����
    // *������* ������ �������� ���� �������� SX2 * SY2 * SZ2 ����� �������, �������:
    //   - ��� ���� �������� (SX2 * SY2 * SZ2) * (SX1 * SY1 * SZ1) �����
    //   - ������ ���� � (SX2, SY2, SZ2) ��� ��������� ��������
    sbm2_t b;
    // ��������� ������ �� ������� ����, ������� ����� �������� � ������
    // @todo �������� ���������� ������������ ��������� ����� ����� � ����������� �� ��������� ������/�������.
    const typelib::coordInt_t cA( 0, 0, 0 );
    // ������������ �������� � ������ ������� � �����.-��������� ����� �����
    const typelib::coordInt_t shiftB = cA + sbm2_t::bm_t::maxCoord();
    const typelib::psizeInt_t sizeB( sizeImage / SX2,  sizeImage / SY2,  0u );
    b << ElevationMap< SX1, SY1, SZ1 >(
        source, scaleXY, hMin, hMax, true,
        shiftB, sizeB
    );

    // �������� ����� �������� ����
    sbm1_t::bm_t bmA = a.draw();

    // �������� ����� ������� ����
    sbm2_t::bm_t bmB = b.draw();



#if 0
    // ���������� ��
    // (�� ������� ���. ����)
#endif


#if 0
    // ��������� ������ ������� ������
    typelib::OutlineFilterMapContent< true >  outline;
    outline( bmA );
    outline( bmB );
#endif


#if 0
    // ��������� ���, ����� � �������� �������� ���-�� �������
    typelib::NeightbourFilterMapContent< 1, 10, true >  neighbour;
    outline( bmA );
    outline( bmB );
#endif


#if 0
    // �����������
    typelib::InverseFilterMapContent  inverse;
    outline( bmA );
    outline( bmB );
#endif


#if 0
    // ��������� OutlineFilterMapContent ��� ��������� ������
    bm_t bmNeighbour( bm );
    typelib::NeightbourFilterMapContent< 26, 26, true >  neighbour;
    neighbour( bmNeighbour );
    bm ^= bmNeighbour;
#endif



    // ������������� ����� ���������� VTK > http://vtk.org
/* - ���������� ����� json-��������������. ��. ����.
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

#if 0
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
