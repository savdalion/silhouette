#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Sketch.h>
#include <RelativeCoord.h>
#include <EllipsoidES.h>
#include <ElevationMapES.h>
#include <Canvas.h>
#include <VTKVisual.h>


/**
* ���������� ������������ ��� 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // ��� ����������� '.' ������ ','
    setlocale( LC_NUMERIC, "C" );


    /* @example ������ ��������� �����
    // @source http://www.vtk.org/Wiki/VTK/Examples/Cxx/PolyData/PointSource

    auto pointSource = vtkSmartPointer< vtkPointSource >::New();
    pointSource->SetCenter( 0.0, 0.0, 0.0 );
    pointSource->SetNumberOfPoints( 500 );
    pointSource->SetRadius( 5.0 );
    pointSource->Update();
 
    // Create a mapper and actor
    auto mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
    mapper->SetInputConnection( pointSource->GetOutputPort() );
 
    auto actor = vtkSmartPointer< vtkActor >::New();
    actor->SetMapper( mapper );
 
    // Create a renderer, render window, and interactor
    auto renderer = vtkSmartPointer< vtkRenderer >::New();
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );
    auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );
 
    // Add the actor to the scene
    renderer->AddActor( actor );
    renderer->SetBackground( 0.3, 0.6, 0.3 );
 
    // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();
    */





    /* @example ���� �����
    const int N = 30;
    const int M = 20;
    const int L = 10;
    // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html
    auto points = vtkSmartPointer< vtkPoints >::New();
    auto vertices = vtkSmartPointer< vtkCellArray >::New();
    for (int z = 0; z < L; ++z) {
        for (int y = 0; y < M; ++y) {
            for (int x = 0; x < N; ++x) {
                //const int i = x + y * N + z * N * M;
                vtkIdType pid[ 1 ];
                pid[ 0 ] =
                    points->InsertNextPoint( (float)x, (float)y, (float)z );
                vertices->InsertNextCell( 1, pid );
            }
        }
    }

    // Create a polydata object
    auto point = vtkSmartPointer< vtkPolyData >::New();
 
    // Set the points and vertices we created as the geometry
    // and topology of the polydata
    point->SetPoints( points );
    point->SetVerts( vertices );
 
    // Visualize
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
    mapper->SetInput( point );
#else
    mapper->SetInputData( point );
#endif
 
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );
    actor->GetProperty()->SetPointSize( 1 );
 
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );
    renderWindow->SetSize( 700, 500 );
    auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );
 
    renderer->AddActor( actor );
 
    renderWindow->Render();
    renderWindowInteractor->Start();

    */








#ifdef SIU_SHAPE_VISUALTEST_PLANET_EARTH
{
    // ����� - �������� � ���� ������� � ��������� �������
    // 3^27 ~ 7.6 ����. �� ~ 51 �.�. ~ 7 ��. �����
    const int hCeilSketch = 27;
    // 3^0 = 1 �
    const int hFloorSketch = 0;
    typedef siu::Sketch sketch_t;
    typedef siu::RelativeCoord coord_t;
    const coord_t rcParent( hCeilSketch + 1, 0.0, 0.0, 0.0 );
    sketch_t solarSystemS( rcParent, hCeilSketch, hFloorSketch );

    // ��������� � ������ �������. ���������� � ��� ���������.
    // ������� ��������� � ������.
    // @source http://ru.wikipedia.org/wiki/%D0%97%D0%B5%D0%BC%D0%BB%D1%8F
    typedef siu::EllipsoidES< false >  ellipsoid_t;
    auto earth = std::shared_ptr< ellipsoid_t >(
        new ellipsoid_t( "�����", "rock" )
    );
    earth->rz = 6356.8 * 1000.0;
    earth->rx = earth->ry = earth->rz * 1.0033528;
    earth->density = 5515.3;

    const sketch_t::Content contentEarth( earth, coord_t(
        hCeilSketch,
        // ������ - ����� ������� - (0; 0), ������ ������� - � ������
        (152097701.0 + 147098074.0) / 2.0 * 1000.0,
        0.0,
        0.0
    ) );
    solarSystemS << contentEarth;


    // ���������� ����� ��� ������������ ������

    // ����� "��������� �������"
    {
        /* - @todo ...
        // ������:
        // [ ~ "���� �������" (3^27 ~ 7.6 ����. ��);  ~ "������ ��������" (3^22 ~ 31.4 ���. ��) ]
        // ���������� �� ���������� �������� � "�������" (3^13 ~ 1600 ��) � ������.
        // ����������� ��������, �������� �� "��������" �� "������ ��������"
        // ������������� �� ������ �������� � ��� �������������� ���������
        // (�������������), �.�. �� ������ ������ ������ ��� ����� ������.
        // ��������, ����� �� �����, ������� ����������� �������� ������ ��
        // ��������, ��������������� �������.
        const int hCeilCanvas = hCeilSketch;
        // ������� ������ �� ��������� 5, �.�. �� �� 2012 �. ������������
        // 3^5^3 = 243^3 = 14�348�907 ����� ������ � ������������� �������, ����
        // ��������� ������� ������������ "��������-�������", � �� ������� �������.
        const int hFloorCanvas = hCeilCanvas - 5;  // = 27 - 5 = 22;
        assert( (hFloorCanvas == 22) && "���� ��� �� ������, ��������� � ����� ��������� �������� ���������." );
        const int hObservation = 13;
        typedef siu::Canvas< K >  canvas_t;
        canvas_t solarSystemC( hCeilCanvas, hFloorCanvas, hObservation );

        solarSystemC << solarSystemS;
        ...
        */
    }


    // ����� "�����"
    {
        // ������:
        // [ ~ "�������" (3^15 ~ 14.3 ���. ��) ;  ~ "" (3^10 ~ 59 ��) ]
        // ���������� �� ���������� �������� � "������" (3^0 ~ 1 �) � ������.
        // ����������� ��������, �������� �� "�������" �� ~59 �� (��. ������ ����)
        // ������������� �� ������ �������� � ��� �������������� ���������
        // (�������������), �.�. �� ������ ������ ������ ��� ����� ������.
        // ��������, ����� �� �����, ������� ����������� �������� ������ ��
        // ��������, ��������������� �������.
        const int hCeilCanvas = 15;
        // ������� ������ �� ��������� 5, �.�. �� �� 2012 �. ������������
        // 3^5^3 = 243^3 = 14�348�907 ����� ������ � ������������� �������, ����
        // ��������� ������� ������������ "��������-�������", � �� ������� �������.
        const int hFloorCanvas = hCeilCanvas - 5;
        assert( (hFloorCanvas == 10) && "15 - 5 = 10. ���� ��� �� ������, ��������� � ����� ��������� �������� ���������." );
        const int hObservation = 0;
        typedef siu::Canvas canvas_t;
        
#if 1
        // @1 ������������� ���, ����� � ������ ������ ��������� �����
        const siu::RelativeCoord coordCanvasEarth(
            hCeilSketch,
            contentEarth.c.x,
            contentEarth.c.y,
            contentEarth.c.z
        );
        canvas_t earthC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasEarth );
#else        
        // @2 ������������� ���, ����� ����� �������� ��������� ��� ������
        const double earthShift = 6000.0 * 1000.0;
        const siu::RelativeCoord coordCanvasEarth(
            hCeilSketch,
            contentEarth.c.x + earthShift,
            contentEarth.c.y + earthShift,
            contentEarth.c.z + earthShift
        );
        canvas_t earthC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasEarth );
#endif

        // �������� �� ����� ������
        // ����� ��� ����������� �������� ������ � �������� ������
        earthC << solarSystemS;


        // ������������� ����� ���������� VTK > http://vtk.org
        siu::io::VTKVisual< 700, 1, true, true >  visual;
        visual << earthC;
        visual.wait();

    } // ����� "�����"



}
#endif








#ifdef SIU_SHAPE_VISUALTEST_MAINLAND
{
    // ����� - �������� � �������:
    // 3^14 ~ 4.8 ���. ��
    const int hCeilSketch = 14;
    // 3^0 = 1 �
    const int hFloorSketch = 0;
    // ����� �� ����������� ������� � �������, ������� ����
    const siu::RelativeCoord rcParent( hCeilSketch + 1, 0.0, 0.0, 0.0 );
    siu::Sketch mainlandS( rcParent, hCeilSketch, hFloorSketch );

    // ��������� � ������ ����� �����.
    // ������� - � ������.
    typedef siu::ElevationMapES< false >  elevationMap_t;
    auto australia = std::shared_ptr< elevationMap_t >(
        new elevationMap_t( "���������", "soil" )
    );
    australia->source = siu::PATH_MEDIA + "elevation-australia.png";
    // ��������� ����������� � *���������* ����
    // @source http://geography.kz/kontinenty/obshhij-obzor-avstralii
    // max( 4100.0, 3200.0 ) ��
    australia->sizeXY = 4100.0 * 1000.0;
    // ������ ��� + ������� ������
    // @source http://ru.wikipedia.org/wiki/%D0%A1%D0%BD%D0%BE%D1%83%D0%B8_%28%D0%B3%D0%BE%D1%80%D1%8B%29
    australia->height = (2.2 + 0.3) * 1000.0;

    // ��������� ���������� � "�������� ��������"
    const siu::Sketch::Content contentAustralia( australia, siu::RelativeCoord(
        hCeilSketch,
        0.0,
        0.0,
        0.0
    ) );
    mainlandS << contentAustralia;


    // ���������� ����� ��� ������������ ������

    // ����� "���������"
    {
        // ������:
        // [ ~ "�������" (3^14 ~ 4.8 ���. ��) ;  ~ "�������" (3^9 ~ 20 ��) ]
        // ���������� �� ���������� �������� � "������" (3^0 ~ 1 �) � ������.
        // @see ����. � ������ "�����".
        const int hCeilCanvas = 14;
        const int hFloorCanvas = hCeilCanvas - 5;
        assert( (hFloorCanvas == 9) && "14 - 5 = 9. ���� ��� �� ������, ��������� � ����� ��������� �������� ���������." );
        const int hObservation = 0;
        typedef siu::Canvas canvas_t;
        
#if 1
        // @1 ������������� ���, ����� � ������ ������ ��������� ���������
        const siu::RelativeCoord coordCanvasAustralia(
            hCeilSketch,
            contentAustralia.c.x,
            contentAustralia.c.y,
            // Z-������� ����������� 0-� �� ���������, ��. ElevationMap::draw()
            contentAustralia.c.z
        );
        canvas_t australiaC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasAustralia );
#else        
        // @2 ������������� ���, ����� ��������� �������� ��������� ��� ������
        const double australiaShift = 1500.0 * 1000.0;
        const siu::RelativeCoord coordCanvasAustralia(
            hCeilSketch,
            contentAustralia.c.x + australiaShift,
            contentAustralia.c.y + australiaShift,
            contentAustralia.c.z + australiaShift
        );
        canvas_t australiaC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasAustralia );
#endif

        // �������� �� ����� ������
        // ����� ��� ����������� �������� ������ � �������� ������
        australiaC << mainlandS;


        // ������������� ����� ���������� VTK > http://vtk.org
        siu::io::VTKVisual< 700, 1, true, true >  visual;
        visual << australiaC;
        visual.wait();

    } // ����� "���������"



}
#endif








#ifdef SIU_SHAPE_VISUALTEST_PART_MAINLAND
{
    // �� ��, ��� SIU_SHAPE_VISUALTEST_MAINLAND, �� ����� ������
    // �������� �������

    // ����� - �������� � �������:
    // 3^14 ~ 4.8 ���. ��
    const int hCeilSketch = 14;
    // 3^0 = 1 �
    const int hFloorSketch = 0;
    // ����� �� ����������� ������� � �������, ������� ����
    const siu::RelativeCoord rcParent( hCeilSketch + 1, 0.0, 0.0, 0.0 );
    siu::Sketch mainlandS( rcParent, hCeilSketch, hFloorSketch );

    // ��������� � ������ ����� �����.
    // ������� - � ������.
    typedef siu::ElevationMapES< false >  elevationMap_t;
    auto australia = std::shared_ptr< elevationMap_t >(
        new elevationMap_t( "���������", "soil" )
    );
    australia->source = siu::PATH_MEDIA + "elevation-australia.png";
    // ��������� ����������� � *���������* ����
    // @source http://geography.kz/kontinenty/obshhij-obzor-avstralii
    // max( 4100.0, 3200.0 ) ��
    australia->sizeXY = 4100.0 * 1000.0;
    // ������ ��� + ������� ������
    // @source http://ru.wikipedia.org/wiki/%D0%A1%D0%BD%D0%BE%D1%83%D0%B8_%28%D0%B3%D0%BE%D1%80%D1%8B%29
    australia->height = (2.2 + 0.3) * 1000.0;

    // ��������� ���������� � "�������� ��������"
    const siu::Sketch::Content contentAustralia( australia, siu::RelativeCoord(
        hCeilSketch,
        0.0,
        0.0,
        0.0
    ) );
    mainlandS << contentAustralia;


    // ���������� ����� ��� ������������ ������

    // ����� "����� ���������"
    {
        // ������:
        // [ ~ "�������" (3^9 ~ 20 ��); "�������" (3^4 ~ 81 �) ]
        // ���������� �� ���������� �������� � "������" (3^0 ~ 1 �) � ������.
        // @see ����. � ������ "�����".
        const int hCeilCanvas = 9;
        const int hFloorCanvas = hCeilCanvas - 5;
        assert( (hFloorCanvas == 4) && "9 - 5 = 4. ���� ��� �� ������, ��������� � ����� ��������� �������� ���������." );
        const int hObservation = 0;
        typedef siu::Canvas canvas_t;
        
#if 1
        // @1 ����� ����������� �.�., ����� ������ ����� ����� ���������
        const siu::RelativeCoord coordCanvasAustralia(
            // ������������� ����� ������������ ��������� "�������" (3^14)
            hCeilSketch,
            contentAustralia.c.x,
            contentAustralia.c.y,
            // Z-������� ����������� 0-� �� ���������, ��. ElevationMap::draw()
            contentAustralia.c.z
        );
        canvas_t australiaC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasAustralia );
#else        
        // @2 ...
#endif

        // �������� �� ����� ������
        // ����� ��� ����������� �������� ������ � �������� ������
        australiaC << mainlandS;


        // ������������� ����� ���������� VTK > http://vtk.org
        siu::io::VTKVisual< 700, 1, true, true >  visual;
        visual << australiaC;
        visual.wait();

    } // ����� "���������"
    

}
#endif



    return 0;

} // main()
