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
    // ������ � ������ ���������� ���� ����������� ��������� ����������
    const size_t K = 3;

    // ����� - �������� � ���� ������� � ��������� �������
    // 3^27 ~ 7.6 ����. �� ~ 51 �.�. ~ 7 ��. �����
    const int HCeilSketch = 27;
    // 3^0 = 1 �
    const int HFloorSketch = 0;
    typedef siu::Sketch< HCeilSketch, HFloorSketch, K >  beltSketch_t;
    const siu::RelativeCoord rcParent( HCeilSketch + 1, 0.0, 0.0, 0.0 );
    beltSketch_t solarSystemS( rcParent );

    // ��������� � ������ �������. ���������� � ��� ���������.
    // ������� ��������� � ������.
    // @source http://ru.wikipedia.org/wiki/%D0%97%D0%B5%D0%BC%D0%BB%D1%8F
    siu::EllipsoidES earth( "�����" );
    earth.rz = 6356.8 * 1000.0;
    earth.rx = earth.ry = earth.rz * 1.0033528;
    earth.density = 5515.3;

    const beltSketch_t::Content contentEarth( earth, siu::RelativeCoord(
        HCeilSketch,
        // ������ - ����� ������� - (0; 0), ���������� - � ������
        (152097701.0 + 147098074.0) / 2.0 * 1000.0,
        0.0,
        0.0
    ) );
    solarSystemS << contentEarth;

    // ���������� ����� ��� ������������ ������

    // ������ ������:
    // [ ~ "���� �������" (3^27 ~ 7.6 ����. ��);  ~ "������ ��������" (3^22 ~ 31.4 ���. ��) ]
    // ���������� �� ���������� �������� � "�������" (3^13 ~ 1600 ��) � ������.
    // ����������� ��������, �������� �� "��������" �� "������ ��������"
    // ������������� �� ������ �������� � ��� �������������� ���������
    // (�������������), �.�. �� ������ ������ ������ ��� ����� ������.
    // ��������, ����� �� �����, ������� ����������� �������� ������ ��
    // ��������, ��������������� �������.
    const int HCeilCanvas = HCeilSketch;
    // ������� ������ �� ��������� 5, �.�. �� �� 2012 �. ������������
    // 3^5^3 = 243^3 = 14�348�907 ����� ������ � ������������� �������, ����
    // ��������� ������� ������������ "��������-�������", � �� ������� �������.
    const int HFloorCanvas = HCeilCanvas - 5;  // = 27 - 5 = 22;
    assert( (HFloorCanvas == 22) && "��������� � ����� �������� ��� ���������, ���� ��� �� ������." );
    const int HObservation = 13;
    typedef siu::Canvas< HCeilCanvas, HFloorCanvas, HObservation, K >  beltCanvas_t;
    beltCanvas_t solarSystemC();

    solarSystemC << solarSystemS;

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
