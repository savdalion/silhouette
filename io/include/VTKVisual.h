#pragma once

#include <Canvas.h>

#include <vtkPointSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
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



namespace siu {
    namespace io {


/**
* ������������ ���������� VTK.
*
* @template sizeWindowT ������ ���� ������������, ���. ���� - ����������.
* @template sizePointT ������ �����, ���.
* @template showCornerT �������� ���� ���������������� ��������.
* @template showAxesT ���������� ��� ���������� ���������.
*
* @source http://vtk.org
*/
template<
    size_t sizeWindowT,
    size_t sizePointT,
    bool showCornerT = true,
    bool showAxesT = true
>
class VTKVisual {
public:
    /**
    * ��������� ���� ��� ������������.
    */
    inline VTKVisual(
    ) :
        renderer( vtkSmartPointer< vtkRenderer >::New() ),
        renderWindow( vtkSmartPointer< vtkRenderWindow >::New() ),
        hasAxes( false )
    {
        renderWindow->AddRenderer( renderer );
        renderWindow->SetSize( sizeWindowT, sizeWindowT );

        // ����������� ������
        auto camera = renderer->GetActiveCamera();
        // ������������� �������� ������ �������� �������
        camera->SetParallelProjection( true );
        // ����������� ����: X - ������, Y - � �����������, Z - �����
        camera->SetPosition( 0, -1, 0 );
        camera->SetFocalPoint( 0, 0, 0 );
        //camera->SetViewUp( 0, 1, 1 );
    }



    virtual inline ~VTKVisual() {
        // ������������ ����� ���������, �������
    }




    /**
    * ������������� �����. ���� ���� ������������ ��� �� ���� �������, ���
    * ��������. �����, ����� ����������� � �������� ����.
    */
    inline VTKVisual& operator<<( const Canvas& canvas ) {

        // ��������� ���������� ����� � ������ VTK
        // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html
        const auto& content = canvas.content();
        assert( (content.size() == 1) && "��������� ������� �� ������ ������������ ����������." );

        // ��� �������������
        const float halfN = static_cast< float >( canvas.n() ) / 2.0f;
        const float shiftCenter = -halfN;
            
        auto points = vtkSmartPointer< vtkPoints >::New();
        auto vertices = vtkSmartPointer< vtkCellArray >::New();
        for (auto ctr = content.cbegin(); ctr != content.cend(); ++ctr) {
            // ������ ������ � ����� ���������
            const size_t N = ctr->bm->N;
            assert( (N == canvas.n()) && "������� ������� ������ ���������." );

            fillCloudPoints( points, vertices, shiftCenter, *ctr->bm );

        } // for (auto ctr = content.cbegin(); ctr != content.cend(); ++ctr)


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
        contentActor->GetProperty()->SetPointSize( sizePointT );
        renderer->AddActor( contentActor );


        // �������� ������� ������
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


        // ������ ���
        if ( !hasAxes && showAxesT ) {
            // ��� ������ ����� ������������, �.�. ������� �� ��������� ������ ����. ������.
            /*
            {
                auto axesActor = vtkSmartPointer< vtkAxesActor >::New();
                axesActor->SetAxisLabels( 0 );
                renderer->AddActor( axesActor );
            }
            */

            {
                auto cubeAxesActor = vtkSmartPointer< vtkCubeAxesActor >::New();
                cubeAxesActor->SetBounds(
                    showCornerT ? cornerPoints->GetBounds() : points->GetBounds()
                );
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


        // ��������� ��� ����������
        renderer->ResetCamera();
        renderWindow->Render();

        return *this;
    }





    /**
    * ������� �������� ���� ������������.
    */
    inline void wait() {
        auto renderWindowInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
        renderWindowInteractor->SetRenderWindow( renderWindow );
        renderer->ResetCamera();
        renderWindow->Render();
        renderWindowInteractor->Start();
    }





private:
    /**
    * ��������� ������� ����� ������� � ������ VTK.
    *
    * @param center �������� ��� ���� ����� ������������ ������.
    */
    static inline void fillCloudPoints(
        vtkSmartPointer< vtkPoints >  points,
        vtkSmartPointer< vtkCellArray >  vertices,
        float shiftCenter,
        const BitContent3D& bc
    ) {
        /* - ���������� �� do/while. �������. ��. ����.
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
        */

        if ( bc.empty() ) {
            return;
        }

        size_t i = bc.raw.get_first();
        do {
            size_t c[3];
            bc.ci( c, i );
            const float cf[3] = {
                static_cast< float >( c[0] ) + shiftCenter,
                static_cast< float >( c[1] ) + shiftCenter,
                static_cast< float >( c[2] ) + shiftCenter
            };
            vtkIdType pid[ 1 ];
            // @todo optimize ������������ ����� ������� ���������� ������� � ���������.
            pid[ 0 ] = points->InsertNextPoint( cf );
            vertices->InsertNextCell( 1, pid );

            i = bc.raw.get_next( i );

        } while (i != 0);

    }




private:
    vtkSmartPointer< vtkRenderer >  renderer;
    vtkSmartPointer< vtkRenderWindow >  renderWindow;

    /**
    * ��� ��������� ���������������.
    */
    bool hasAxes;

};


    } // io

} // siu
