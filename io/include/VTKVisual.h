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


// @todo extend ������� ��������� ������������ > http://www.bu.edu/tech/research/training/tutorials/vtk/#CONTOUR


namespace siu {
    namespace io {


/**
* ������������ ���������� VTK � ���� ������ �����.
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
    size_t sizePointT = 1,
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

        /* @todo ��������� ���������
        vtkLightCollection* lc = vtkLightCollection::New();

        vtkLight* lTop = vtkLight::New();
        lTop->SetPosition( 1.0, 0, 0 );
        lTop->SetColor( 0.5, 1.0, 0.5 );

        lc->AddItem( lTop );
        renderer->SetLightCollection( lc );
        */
    }



    virtual inline ~VTKVisual() {
        // ������������ ����� ���������, �������
    }




    /**
    * ������������� �����. ���� ���� ������������ ��� �� ���� �������, ���
    * ��������. �����, ����� ����������� � �������� ����.
    */
    template< size_t Grid >
    inline VTKVisual& operator<<( const common::BitMap< Grid >&  bm ) {

        typedef common::BitMap< Grid >  bm_t;

        // ��������� ���������� ����� � ������ VTK
        // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html

        // ��� ������������� � ������� ������ �����
        const float halfN = static_cast< float >( Grid - 1 ) / 2.0f;
        const float shiftCenter = 0.0f;
            
        auto points = vtkSmartPointer< vtkPoints >::New();
        auto vertices = vtkSmartPointer< vtkCellArray >::New();

        size_t i = bm.first();
        do {
            const typelib::coordInt_t c = bm_t::ci( i );
            const float cf[3] = {
                static_cast< float >( c.x ) + shiftCenter,
                static_cast< float >( c.y ) + shiftCenter,
                static_cast< float >( c.z ) + shiftCenter
            };
            vtkIdType pid[ 1 ];
            // @todo optimize ������������ ����� ������� ���������� ������� � ���������.
            pid[ 0 ] = points->InsertNextPoint( cf );
            vertices->InsertNextCell( 1, pid );

            i = bm.next( i );

#ifdef _DEBUG
            if (i >= bm_t::volume()) {
                // ���������� �� ��������� �����, ������ ���� �� ������
                // �������, ������� ��� ��������� ��������� ���
                // �������� ������ ������
                size_t n = 0;
                do {
                    ++n;
                } while (i != 0);
                assert( (n == 0) && "���������� ������ �� ��������� �����." );
                break;
            }
#endif

        } while (i != 0);


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
    vtkSmartPointer< vtkRenderer >  renderer;
    vtkSmartPointer< vtkRenderWindow >  renderWindow;

    /**
    * ��� ��������� ���������������.
    */
    bool hasAxes;

};


    } // io

} // siu
