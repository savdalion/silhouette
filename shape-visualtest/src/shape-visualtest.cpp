#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Sketch.h>
#include <RelativeCoord.h>
#include <EllipsoidES.h>
#include <Canvas.h>


/**
* Визуальное тестирование для 'shape'.
*/
int main( int argc, char** argv ) {

    setlocale( LC_ALL, "Russian" );
    // Для разделителя '.' вместо ','
    setlocale( LC_NUMERIC, "C" );


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






    // Получаем формы из библиотеки 'shape' и помещаем их в окна рендеринга




#ifdef SHAPE_VISUALTEST_ELLI
{
    // Эскизы и холсты используют один коэффициент кратности горизонтов
    const size_t K = 3;

    // Эскиз - размером с Пояс Койпера в Солнечной системе
    // 3^27 ~ 7.6 млрд. км ~ 51 а.е. ~ 7 св. часов
    const int HCeilSketch = 27;
    // 3^0 = 1 м
    const int HFloorSketch = 0;
    typedef siu::Sketch< HCeilSketch, HFloorSketch, K >  beltSketch_t;
    const siu::RelativeCoord rcParent( HCeilSketch + 1, 0.0, 0.0, 0.0 );
    beltSketch_t solarSystemS( rcParent );

    // Добавляем к эскизу планету. Представим её как эллипсоид.
    // Радиусы указываем в метрах.
    // @source http://ru.wikipedia.org/wiki/%D0%97%D0%B5%D0%BC%D0%BB%D1%8F
    siu::EllipsoidES earth( "Земля" );
    earth.rz = 6356.8 * 1000.0;
    earth.rx = earth.ry = earth.rz * 1.0033528;
    earth.density = 5515.3;

    const beltSketch_t::Content contentEarth( earth, siu::RelativeCoord(
        HCeilSketch,
        // Солнце - центр системы - (0; 0), расстояние - в метрах
        (152097701.0 + 147098074.0) / 2.0 * 1000.0,
        0.0,
        0.0
    ) );
    solarSystemS << contentEarth;

    // Подготовим холст для визуализации эскиза

    // Размер холста:
    // [ ~ "пояс Койпера" (3^27 ~ 7.6 млрд. км);  ~ "орбита Меркурия" (3^22 ~ 31.4 млн. км) ]
    // Наблюдение за элементами размером с "сегмент" (3^13 ~ 1600 км) и больше.
    // Наблюдаемые элементы, размером от "сегмента" до "орбиты меркурия"
    // располагаются на холсте отдельно и без дополнительной обработки
    // (растрирования), т.к. их размер меньше ячейки дна этого холста.
    // Элементы, попав на холст, степень детализации которого меньше их
    // размеров, визуализируются точками.
    const int HCeilCanvas = HCeilSketch;
    // Глубина холста не превышает 5, т.к. на ПК 2012 г. обрабатывать
    // 3^5^3 = 243^3 = 14 348 907 ячеек близко к практическому пределу, если
    // планируем создать динамическую "реалтайм-систему", а не научную таблицу.
    const int HFloorCanvas = HCeilCanvas - 5;  // = 27 - 5 = 22;
    assert( (HFloorCanvas == 22) && "Исправьте и здесь значение дна горизонта, если это не ошибка." );
    const int HObservation = 13;
    typedef siu::Canvas< HCeilCanvas, HFloorCanvas, HObservation, K >  beltCanvas_t;
    beltCanvas_t solarSystemC();

    solarSystemC << solarSystemS;

}
#endif




#if 0

    // Переводим полученный объём в формат VTK
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
