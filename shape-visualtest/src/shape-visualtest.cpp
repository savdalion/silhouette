#include "../include/StdAfx.h"
#include "../include/configure.h"
#include <Sketch.h>
#include <RelativeCoord.h>
#include <EllipsoidES.h>
#include <ElevationMapES.h>
#include <Canvas.h>
#include <VTKVisual.h>


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








#ifdef SIU_SHAPE_VISUALTEST_PLANET_EARTH
{
    // Эскиз - размером с Пояс Койпера в Солнечной системе
    // 3^27 ~ 7.6 млрд. км ~ 51 а.е. ~ 7 св. часов
    const int hCeilSketch = 27;
    // 3^0 = 1 м
    const int hFloorSketch = 0;
    typedef siu::Sketch sketch_t;
    typedef siu::RelativeCoord coord_t;
    const coord_t rcParent( hCeilSketch + 1, 0.0, 0.0, 0.0 );
    sketch_t solarSystemS( rcParent, hCeilSketch, hFloorSketch );

    // Добавляем к эскизу планету. Представим её как эллипсоид.
    // Радиусы указываем в метрах.
    // @source http://ru.wikipedia.org/wiki/%D0%97%D0%B5%D0%BC%D0%BB%D1%8F
    typedef siu::EllipsoidES< true >  ellipsoid_t;
    auto earth = std::shared_ptr< ellipsoid_t >(
        new ellipsoid_t( "Земля", "rock" )
    );
    earth->rz = 6356.8 * 1000.0;
    earth->rx = earth->ry = earth->rz * 1.0033528;
    earth->density = 5515.3;

    const sketch_t::Content contentEarth( earth, coord_t(
        hCeilSketch,
        // Солнце - центр системы - (0; 0), орбита планеты - в метрах
        (152097701.0 + 147098074.0) / 2.0 * 1000.0,
        0.0,
        0.0
    ) );
    solarSystemS << contentEarth;


    // Подготовим холст для визуализации эскиза

    // Холст "Солнечная система"
    {
        /* - @todo ...
        // Размер:
        // [ ~ "пояс Койпера" (3^27 ~ 7.6 млрд. км);  ~ "орбита Меркурия" (3^22 ~ 31.4 млн. км) ]
        // Наблюдение за элементами размером с "сегмент" (3^13 ~ 1600 км) и больше.
        // Наблюдаемые элементы, размером от "сегмента" до "орбиты меркурия"
        // располагаются на холсте отдельно и без дополнительной обработки
        // (растрирования), т.к. их размер меньше ячейки дна этого холста.
        // Элементы, попав на холст, степень детализации которого меньше их
        // размеров, визуализируются точками.
        const int hCeilCanvas = hCeilSketch;
        // Глубина холста не превышает 5, т.к. на ПК 2012 г. обрабатывать
        // 3^5^3 = 243^3 = 14 348 907 ячеек близко к практическому пределу, если
        // планируем создать динамическую "реалтайм-систему", а не научную таблицу.
        const int hFloorCanvas = hCeilCanvas - 5;  // = 27 - 5 = 22;
        assert( (hFloorCanvas == 22) && "Если это не ошибка, исправьте и здесь пороговое значение горизонта." );
        const int hObservation = 13;
        typedef siu::Canvas< K >  canvas_t;
        canvas_t solarSystemC( hCeilCanvas, hFloorCanvas, hObservation );

        solarSystemC << solarSystemS;
        ...
        */
    }


    // Холст "Земля"
    {
        // Размер:
        // [ ~ "планета" (3^15 ~ 14.3 тыс. км) ;  ~ "" (3^10 ~ 59 км) ]
        // Наблюдение за элементами размером с "эталон" (3^0 ~ 1 м) и больше.
        // Наблюдаемые элементы, размером от "эталона" до ~59 км (см. размер выше)
        // располагаются на холсте отдельно и без дополнительной обработки
        // (растрирования), т.к. их размер меньше ячейки дна этого холста.
        // Элементы, попав на холст, степень детализации которого меньше их
        // размеров, визуализируются точками.
        const int hCeilCanvas = 15;
        // Глубина холста не превышает 5, т.к. на ПК 2012 г. обрабатывать
        // 3^5^3 = 243^3 = 14 348 907 ячеек близко к практическому пределу, если
        // планируем создать динамическую "реалтайм-систему", а не научную таблицу.
        const int hFloorCanvas = hCeilCanvas - 5;  // = 15 - 5 = 10;
        assert( (hFloorCanvas == 10) && "Если это не ошибка, исправьте и здесь пороговое значение горизонта." );
        const int hObservation = 0;
        typedef siu::Canvas canvas_t;
        
#if 1
        // @1 Позиционируем так, чтобы в центре холста оказалась Земля
        const siu::RelativeCoord coordCanvasEarth(
            hCeilSketch,
            contentEarth.c.x,
            contentEarth.c.y,
            contentEarth.c.z
        );
        canvas_t earthC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasEarth );
#else        
        // @2 Позиционируем так, чтобы Земля частично оказалась вне холста
        const double earthShift = 6000.0 * 1000.0;
        const siu::RelativeCoord coordCanvasEarth(
            hCeilSketch,
            contentEarth.c.x + earthShift,
            contentEarth.c.y + earthShift,
            contentEarth.c.z + earthShift
        );
        canvas_t earthC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasEarth );
#endif

        // Помещаем на холст эскизы
        // Холст сам преобразует элементы эскиза в элементы холста
        earthC << solarSystemS;


        // Визуализируем холст средствами VTK > http://vtk.org
        siu::io::VTKVisual< 700, true, true >  visual;
        visual << earthC;
        visual.wait();

    } // Холст "Земля"



}
#endif








#ifdef SIU_SHAPE_VISUALTEST_MAINLAND
{
    // Эскиз - размером с материк:
    // 3^14 ~ 4.8 тыс. км
    const int hCeilSketch = 14;
    // 3^0 = 1 м
    const int hFloorSketch = 0;
    // здесь не привязываем материк к планете, простой тест
    const siu::RelativeCoord rcParent( hCeilSketch + 1, 0.0, 0.0, 0.0 );
    siu::Sketch mainlandS( rcParent, hCeilSketch, hFloorSketch );

    // Добавляем к эскизу карту высот.
    // Размеры - в метрах.
    typedef siu::ElevationMapES< false >  elevationMap_t;
    auto australia = std::shared_ptr< elevationMap_t >(
        new elevationMap_t( "Австралия", "soil" )
    );
    australia->source = siu::PATH_MEDIA + "elevation-australia.png";
    // континент располагаем в *привычном* виде
    // @source http://geography.kz/kontinenty/obshhij-obzor-avstralii
    // max( 4100.0, 3200.0 ) км
    australia->sizeXY = 4100.0 * 1000.0;
    // высота гор + глубина океана
    // @source http://ru.wikipedia.org/wiki/%D0%A1%D0%BD%D0%BE%D1%83%D0%B8_%28%D0%B3%D0%BE%D1%80%D1%8B%29
    australia->height = (2.2 + 0.3) * 1000.0;

    // Австралия помещается в "горизонт материка"
    const siu::Sketch::Content contentAustralia( australia, siu::RelativeCoord(
        hCeilSketch,
        0.0,
        0.0,
        0.0
    ) );
    mainlandS << contentAustralia;


    // Подготовим холст для визуализации эскиза

    // Холст "Австралия"
    {
        // Размер:
        // [ ~ "материк" (3^14 ~ 4.8 тыс. км) ;  ~ "участок" (3^9 ~ 20 км) ]
        // Наблюдение за элементами размером с "эталон" (3^0 ~ 1 м) и больше.
        // @see Комм. к холсту "Земля".
        const int hCeilCanvas = 14;
        const int hFloorCanvas = hCeilCanvas - 5;  // = 15 - 5 = 10;
        assert( (hFloorCanvas == 9) && "Если это не ошибка, исправьте и здесь пороговое значение горизонта." );
        const int hObservation = 0;
        typedef siu::Canvas canvas_t;
        
#if 1
        // @1 Позиционируем так, чтобы в центре холста оказалась Австралия
        const siu::RelativeCoord coordCanvasAustralia(
            hCeilSketch,
            contentAustralia.c.x - australia->sizeXY / 2.0,
            contentAustralia.c.y - australia->sizeXY / 2.0,
            // Z-позиция принимается 0-й по умолчанию, см. ElevationMap::draw()
            contentAustralia.c.z
        );
        canvas_t australiaC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasAustralia );
#else        
        // @2 Позиционируем так, чтобы Австралия частично оказалась вне холста
        const double australiaShift = 2000.0 * 1000.0;
        const siu::RelativeCoord coordCanvasAustralia(
            hCeilSketch,
            contentAustralia.c.x - sc.get<0>() + australiaShift,
            contentAustralia.c.y - sc.get<1>() + australiaShift,
            contentAustralia.c.z - sc.get<2>() + australiaShift
        );
        canvas_t australiaC( hCeilCanvas, hFloorCanvas, hObservation, coordCanvasAustralia );
#endif

        // Помещаем на холст эскизы
        // Холст сам преобразует элементы эскиза в элементы холста
        australiaC << mainlandS;


        // Визуализируем холст средствами VTK > http://vtk.org
        siu::io::VTKVisual< 700, true, true >  visual;
        visual << australiaC;
        visual.wait();

    } // Холст "Австралия"



}
#endif



    return 0;

} // main()
