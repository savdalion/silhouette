#pragma once

#include <BitMap.h>
#include <other-type.h>

#include <vtkPointSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkCubeSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkCubeAxesActor.h>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkProperty.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <vtkSurfaceReconstructionFilter.h>



// @todo fine Оставить в VTKVisual только один виртуальны метод.


namespace siu {
    namespace io {


/**
* Визуализация средствами VTK в виде поверхности.
*
* @template sizeWindowT Размер окна визуализации, пкс. Окно - квадратное.
* @template sizePointT Размер точки, пкс.
* @template showCornerT Отмечать углы визуализируемого элемента.
* @template showAxesT Показывать оси декартовых координат.
*
* @source http://vtk.org
*/
template<
    size_t sizeWindowT,
    size_t sizePointT = 1,
    bool showCornerT = true,
    bool showAxesT = true
>
class SurfaceVTKVisual {
public:
    /**
    * Открывает окно для визуализации.
    */
    inline SurfaceVTKVisual(
    ) :
        renderer( vtkSmartPointer< vtkRenderer >::New() ),
        renderWindow( vtkSmartPointer< vtkRenderWindow >::New() ),
        hasAxes( false )
    {
        renderWindow->AddRenderer( renderer );
        renderWindow->SetSize( sizeWindowT, sizeWindowT );

        // Настраиваем камеру
        auto camera = renderer->GetActiveCamera();
        // перспективная проекция сильно искажает картину
        camera->SetParallelProjection( true );
        // направление осей: X - вправо, Y - к наблюдателю, Z - вверх
        camera->SetPosition( 0, -1, 0 );
        camera->SetFocalPoint( 0, 0, 0 );
        //camera->SetViewUp( 0, 1, 1 );

        /* @todo Добавляем освещение
        vtkLightCollection* lc = vtkLightCollection::New();

        vtkLight* lTop = vtkLight::New();
        lTop->SetPosition( 1.0, 0, 0 );
        lTop->SetColor( 0.5, 1.0, 0.5 );

        lc->AddItem( lTop );
        renderer->SetLightCollection( lc );
        */
    }



    virtual inline ~SurfaceVTKVisual() {
        // используются умные указатели, красота
    }




    /**
    * Визуализирует холст. Если окно визуализации ещё не было создано, оно
    * создаётся. Иначе, холст добавляется к текущему окну.
    */
    template< size_t Grid >
    inline SurfaceVTKVisual& operator<<( const BitMap< Grid >&  bm ) {

        // Переводим полученный холст в формат VTK
        // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html

        // для центрирования и отметок границ секти
        const float halfN = static_cast< float >( Grid - 1 ) / 2.0f;
        const float shiftCenter = 0.0f;
            
        auto points = vtkSmartPointer< vtkPoints >::New();
        auto vertices = vtkSmartPointer< vtkCellArray >::New();

        size_t i = bm.raw.get_first();
        do {
            const typelib::coordInt_t c = bm.ci( i );
            const float cf[3] = {
                static_cast< float >( c.x ) + shiftCenter,
                static_cast< float >( c.y ) + shiftCenter,
                static_cast< float >( c.z ) + shiftCenter
            };
            vtkIdType pid[ 1 ];
            // @todo optimize Использовать более быстрое заполнение точками и вершинами.
            pid[ 0 ] = points->InsertNextPoint( cf );
            vertices->InsertNextCell( 1, pid );

            i = bm.raw.get_next( i );
        } while (i != 0);


        auto polydata = vtkSmartPointer< vtkPolyData >::New(); 
        polydata->SetPoints( points );
        polydata->SetVerts( vertices );
 

        // Строим поверхность из облака точек
        // @source http://www.vtk.org/Wiki/VTK/Examples/Cxx/Filters/SurfaceFromUnorganizedPoints
        auto surface = vtkSmartPointer< vtkSurfaceReconstructionFilter >::New();
#if VTK_MAJOR_VERSION <= 5
        surface->SetInput( polydata );
#else
        surface->SetInputData( polydata );
#endif
        auto cf = vtkSmartPointer< vtkContourFilter >::New();
        cf->SetInputConnection( surface->GetOutputPort() );
        cf->SetValue( 0, 0.0 );
 
        // Sometimes the contouring algorithm can create a volume whose gradient
        // vector and ordering of polygon (using the right hand rule) are
        // inconsistent. vtkReverseSense cures this problem.
        auto reverse = vtkSmartPointer< vtkReverseSense >::New();
        reverse->SetInputConnection( cf->GetOutputPort() );
        reverse->ReverseCellsOn();
        reverse->ReverseNormalsOn();


        auto mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
        mapper->SetInputConnection( reverse->GetOutputPort() );
        mapper->ScalarVisibilityOff();

        auto contentActor = vtkSmartPointer< vtkActor >::New();
        contentActor->SetMapper( mapper );
        contentActor->GetProperty()->SetPointSize( sizePointT );
        renderer->AddActor( contentActor );


        // Отмечаем границы холста
        auto cornerPoints = vtkSmartPointer< vtkPoints >::New();
        if ( showCornerT ) {
            auto cornerVertices = vtkSmartPointer< vtkCellArray >::New();
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
                pid[ i ] = cornerPoints->InsertNextPoint( p[i] );
            }
            cornerVertices->InsertNextCell( NP, pid );

            auto cornerPolydata = vtkSmartPointer< vtkPolyData >::New();
            cornerPolydata->SetPoints( cornerPoints );
            cornerPolydata->SetVerts( cornerVertices );
 
            auto cornerMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
#if VTK_MAJOR_VERSION <= 5
            cornerMapper->SetInput( cornerPolydata );
#else
            cornerMapper->SetInputData( cornerPolydata );
#endif
            auto cornerActor = vtkSmartPointer< vtkActor >::New();
            cornerActor->SetMapper( cornerMapper );
            cornerActor->GetProperty()->SetPointSize( 3.0f );
            cornerActor->GetProperty()->SetColor( 1.0, 0.0, 0.0 );

            renderer->AddActor( cornerActor );

        } // if ( showCornerT )


        // Рисуем оси
        if ( !hasAxes && showAxesT ) {
            // Оси рисуем после визуализации, т.к. процесс их отрисовки длится неск. секунд.
            /*
            {
                auto axesActor = vtkSmartPointer< vtkAxesActor >::New();
                axesActor->SetAxisLabels( 0 );
                renderer->AddActor( axesActor );
            }
            */

            {
                auto cubeAxesActor = vtkSmartPointer< vtkCubeAxesActor >::New();
                // Xmin,Xmax,Ymin,Ymax,Zmin,Zmax
                double bounds[6] = {
                    -halfN,  halfN,
                    -halfN,  halfN,
                    -halfN,  halfN
                };
                cubeAxesActor->SetBounds( bounds );
                //cubeAxesActor->SetBounds( points->GetBounds() );
                cubeAxesActor->SetCamera( renderer->GetActiveCamera() );
                cubeAxesActor->GetProperty()->SetColor( 0.3, 0.3, 0.3 );
                //cubeAxesActor->SetXTitle( "X" );
                //cubeAxesActor->SetYTitle( "Y" );
                //cubeAxesActor->SetZTitle( "Z" );
                cubeAxesActor->SetFlyModeToStaticEdges();
                renderer->AddActor( cubeAxesActor );
            }

            hasAxes = true;

        } // if ( showAxesT )


        // Обновляем что нарисовали
        renderer->ResetCamera();
        renderWindow->Render();

        return *this;
    }





    /**
    * Ожидает закрытия окна визуализации.
    */
    inline void wait() {
        auto renderWindowInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
        renderWindowInteractor->SetRenderWindow( renderWindow );
        renderer->ResetCamera();
        renderWindow->Render();
        renderWindowInteractor->Start();
    }







private:
    vtkSmartPointer< vtkRenderer >  renderer;
    vtkSmartPointer< vtkRenderWindow >  renderWindow;

    /**
    * Оси координат визуализированы.
    */
    bool hasAxes;

};


    } // io

} // siu
