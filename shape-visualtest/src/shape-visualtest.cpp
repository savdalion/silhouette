#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Sketch.h>
#include <RelativeCoord.h>
#include <EllipsoidES.h>
#include <Canvas.h>


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






    // �������� ����� �� ���������� 'shape' � �������� �� � ���� ����������




#ifdef SHAPE_VISUALTEST_ELLI
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
    //siu::EllipsoidES earth( "�����" );
    auto earth = std::shared_ptr< siu::EllipsoidES< true > >( new siu::EllipsoidES< true >( "�����" ) );
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
        const int hFloorCanvas = hCeilCanvas - 5;  // = 15 - 5 = 10;
        assert( (hFloorCanvas == 10) && "���� ��� �� ������, ��������� � ����� ��������� �������� ���������." );
        const int hObservation = 0;
        typedef siu::Canvas canvas_t;
        
#if 0
        // @1 ������������� ���, ����� � ������ ������ ��������� �����
        canvas_t earthC( hCeilCanvas, hFloorCanvas, hObservation, contentEarth.c );
#else        
        // @2 ������������� ���, ����� ����� �������� ��������� ��� ������
        const double earthShift = 6000.0 * 1000.0;
        canvas_t earthC( hCeilCanvas, hFloorCanvas, hObservation, coord_t(
            contentEarth.c.hCeil,
            contentEarth.c.x + earthShift,
            contentEarth.c.y + earthShift,
            contentEarth.c.z + earthShift
        ) );
#endif

        // �������� �� ����� ������
        // ����� ��� ����������� �������� ������ � �������� ������
        earthC << solarSystemS;


        // ��������� ���������� ����� � ������ VTK
        // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html
        const auto& content = earthC.content();
        assert( (content.size() == 1) && "��������� ������� �� ������ ������������ ����������." );

        // ��� �������������
        const float halfN = static_cast< float >( earthC.n() ) / 2.0f;
            
        auto points = vtkSmartPointer< vtkPoints >::New();
        auto vertices = vtkSmartPointer< vtkCellArray >::New();
        for (auto ctr = content.cbegin(); ctr != content.cend(); ++ctr) {
            // ������ ������ � ����� ���������
            const size_t N = ctr->bm->N;
            @todo ���������� �� do/while. ��. ����.
            for (size_t z = 0; z < N; ++z) {
                for (size_t y = 0; y < N; ++y) {
                    for (size_t x = 0; x < N; ++x) {
                        if ( ctr->bm->test( x, y, z ) ) {
                            const float c[3] = {
                                static_cast< float >( x ) - halfN,
                                static_cast< float >( y ) - halfN,
                                static_cast< float >( z ) - halfN
                            };
                            vtkIdType pid[ 1 ];
                            pid[ 0 ] = points->InsertNextPoint( c );
                            vertices->InsertNextCell( 1, pid );
                        }
                    }
                }
            } // for (int z = 0; z < N; ++z)

        } // for (auto ctr = content.cbegin(); ctr != content.cend(); ++ctr)


        // �������������
        auto point = vtkSmartPointer< vtkPolyData >::New();
 
        point->SetPoints( points );
        point->SetVerts( vertices );
 
        auto mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
#if VTK_MAJOR_VERSION <= 5
        mapper->SetInput( point );
#else
        mapper->SetInputData( point );
#endif
 
        auto contentActor = vtkSmartPointer< vtkActor >::New();
        contentActor->SetMapper( mapper );
        contentActor->GetProperty()->SetPointSize( 1 );
 
        auto renderer = vtkSmartPointer< vtkRenderer >::New();
        auto renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
        renderWindow->AddRenderer( renderer );
        renderWindow->SetSize( 700, 700 );
        auto renderWindowInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
        renderWindowInteractor->SetRenderWindow( renderWindow );
 
        renderer->AddActor( contentActor );

        /*
        // ������ �������
        {
            auto cubeSource = vtkSmartPointer< vtkCubeSource >::New();
            auto cubeMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
            cubeMapper->SetInputConnection( cubeSource->GetOutputPort() );
            auto cubeActor = vtkSmartPointer< vtkActor >::New();
            cubeActor->SetScale( 100.0 );
            cubeActor->SetMapper( cubeMapper );
            renderer->AddActor( cubeActor );
        }
        */


        // ������ ������� ������
        auto canvasPoints = vtkSmartPointer< vtkPoints >::New();
        auto canvasVertices = vtkSmartPointer< vtkCellArray >::New();
        {
            const size_t NP = 1 + 8;
            const float p[ NP ][ 3 ] = {
                {  0,      0,      0     },
                {  halfN,  halfN,  halfN },
                {  halfN,  halfN, -halfN },
                {  halfN, -halfN,  halfN },
                {  halfN, -halfN, -halfN },
                { -halfN,  halfN,  halfN },
                { -halfN,  halfN, -halfN },
                { -halfN, -halfN,  halfN },
                { -halfN, -halfN, -halfN }
            };
            vtkIdType pid[ NP ];
            for (size_t i = 0; i < NP; ++i) {
                pid[ i ] = canvasPoints->InsertNextPoint( p[i] );
            }
            canvasVertices->InsertNextCell( NP, pid );

            auto canvasPolydata = vtkSmartPointer< vtkPolyData >::New();
            canvasPolydata->SetPoints( canvasPoints );
            canvasPolydata->SetVerts( canvasVertices );
 
            auto canvasMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
#if VTK_MAJOR_VERSION <= 5
            canvasMapper->SetInput( canvasPolydata );
#else
            canvasMapper->SetInputData( canvasPolydata );
#endif
            auto canvasActor = vtkSmartPointer< vtkActor >::New();
            canvasActor->SetMapper( canvasMapper );
            canvasActor->GetProperty()->SetPointSize( 3.0f );
            canvasActor->GetProperty()->SetColor( 1.0, 0.0, 0.0 );

            renderer->AddActor( canvasActor );
        }


        // ������ ������� ������
        /*
        {
            auto polygonSource = vtkSmartPointer< vtkRegularPolygonSource >::New(); 
            //polygonSource->GeneratePolygonOff();
            polygonSource->SetNumberOfSides( 4 );
            polygonSource->SetRadius( 100 );
            polygonSource->SetCenter( 0, 0, 0 );
            polygonSource->Update();

            auto psMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
            psMapper->SetInputConnection( polygonSource->GetOutputPort() );
            auto psActor = vtkSmartPointer< vtkActor >::New();
            psActor->SetMapper( psMapper );

            renderer->AddActor( psActor );
        }
        */


        // ������ ���
        /*
        auto axesActor = vtkSmartPointer< vtkAxesActor >::New();
        axesActor->SetAxisLabels( 0 );
        //axesActor->SetScale( 10000.0 );
        renderer->AddActor( axesActor );
        */

        // ������ ���
        {
            auto cubeAxesActor = vtkSmartPointer< vtkCubeAxesActor >::New();
            cubeAxesActor->SetBounds(
                (canvasPoints->GetNumberOfPoints() > 0)
                  ? canvasPoints->GetBounds() : points->GetBounds()
            );
            cubeAxesActor->SetCamera( renderer->GetActiveCamera() );
            cubeAxesActor->GetProperty()->SetColor( 0.3, 0.3, 0.3 );
            renderer->AddActor( cubeAxesActor );
        }


        renderer->ResetCamera();

        renderWindow->Render();
        renderWindowInteractor->Start();
    }



}
#endif




#if 0

    // ��������� ���������� ����� � ������ VTK
    // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html
    const auto topology = canvas.topology();
    auto points = vtkSmartPointer< vtkPoints >::New();
    auto vertices = vtkSmartPointer< vtkCellArray >::New();
    size_t i = l0.second.get_first();
    do {
        float c[3];
        siu::Canvas< N >::ci( c, i );
        vtkIdType pid[ 1 ];
        pid[ 0 ] = points->InsertNextPoint( c );
        vertices->InsertNextCell( 1, pid );
        i = l0.second.get_next( i );
    } while ( true );



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

#endif


    return 0;

} // main()
