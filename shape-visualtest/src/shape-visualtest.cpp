#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Shaper.h>
#include <Ellipsoid.h>
#include <VTKVisual.h>


/**
* Визуальное тестирование для 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // Для разделителя '.' вместо ','
    setlocale( LC_NUMERIC, "C" );


    using namespace siu::common;
    using namespace siu::shape;


    /* @example Облако случайных точек
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





    /* @example Блок точек
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




    // (!) Форма и её визуализация, ничего лишнего



#ifdef SIU_SHAPE_VISUALTEST_ELLIPSOID
{
    const size_t GRID = 81;
    const auto rz = 6000.0 * 1000.0;
    const auto rx = rz * 1.8;
    const auto ry = rz * 1.2;
    Ellipsoid< GRID >  elli( rx, ry, rz, true );
    const Shaper< GRID >  shaper( &elli );
    const auto bm = shaper.draw( /* @todo область */ );

    // Визуализируем холст средствами VTK > http://vtk.org
    siu::io::VTKVisual< 700, 2, true, true >  visual;
    visual << bm;
    visual.wait();
}
#endif



    return 0;

} // main()
