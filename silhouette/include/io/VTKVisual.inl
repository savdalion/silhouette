namespace siu {
    namespace io {

        
inline VTKVisual::VTKVisual(
    const io::VTKVisual::option_t& json
) :
    option( json ),
    renderer( vtkSmartPointer< vtkRenderer >::New() ),
    renderWindow( vtkSmartPointer< vtkRenderWindow >::New() ),
    hasAxes( false )
{
    renderWindow->AddRenderer( renderer );

    const size_t sizeWindow = option[ "size-window" ];
    renderWindow->SetSize( sizeWindow, sizeWindow );

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




inline VTKVisual::~VTKVisual() {
    // используются умные указатели, красота
}




template< size_t Grid >
inline VTKVisual& VTKVisual::operator<<(
    const BitMap< Grid >&  bm
) {
    if ( bm.empty() ) {
        // не рисуем ни точек, ни осей - абс. пустота
        return *this;
    }

    typedef BitMap< Grid >  bm_t;

    // Переводим полученный холст в формат VTK
    // @todo optimize http://vtk.1045678.n5.nabble.com/Filling-vtkPoints-and-vtkCellArray-fast-td1243607.html

    // для центрирования и отметок границ сетки
    const float halfN = static_cast< float >( Grid - 1 ) / 2.0f;
    const float shiftCenter = 0.0f;

    auto points = vtkSmartPointer< vtkPoints >::New();
    auto vertices = vtkSmartPointer< vtkCellArray >::New();


    // собираем облако

    // значения для раскраски цветом
    // @source http://vtk.1045678.n5.nabble.com/How-to-use-vtkRibbonFilter-to-show-a-scalar-field-td1237601.html
    auto data = vtkSmartPointer< vtkFloatArray >::New();
    data->Initialize();
    data->SetName( "ElevationData" );
    data->SetNumberOfComponents( 1 );
    data->SetNumberOfValues( bm.count() ); 

    // точки
    size_t n = 0;
    size_t i = bm.first();
    do {
        const typelib::coordInt_t c = bm_t::ci( i );
        const float cf[3] = {
            static_cast< float >( c.x ) + shiftCenter,
            static_cast< float >( c.y ) + shiftCenter,
            static_cast< float >( c.z ) + shiftCenter
        };
        vtkIdType pid[ 1 ];
        // @todo optimize Использовать более быстрое заполнение точками и вершинами.
        pid[ 0 ] = points->InsertNextPoint( cf );
        vertices->InsertNextCell( 1, pid );
        data->SetValue( n, static_cast< float >( c.z ) );

        i = bm.next( i );
        ++n;

#ifdef _DEBUG
        if (i >= bm_t::volume()) {
            // координата за пределами карты, такого быть не должно
            // смотрим, сколько ещё координат оказались вне
            // заданных картой границ
            size_t n = 0;
            do {
                ++n;
            } while (i != 0);
            assert( (n == 0) && "Обнаружены данные за пределами карты." );
            break;
        }
#endif

    } while (i != 0);


    // всё вместе
    auto point = vtkSmartPointer< vtkPolyData >::New(); 
    point->SetPoints( points );
    point->SetVerts( vertices );


    auto mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
#if VTK_MAJOR_VERSION <= 5
    mapper->SetInput( point );
#else
    mapper->SetInputData( point );
#endif

    // цвет точек

    // задан конкретный цвет
    const size_t rgba = option[ "rgba" ];
    const float r = static_cast< float >( (rgba >> 24) & 0x000000ff ) / 255.0f;
    const float g = static_cast< float >( (rgba >> 16) & 0x000000ff ) / 255.0f;
    const float b = static_cast< float >( (rgba >> 8)  & 0x000000ff ) / 255.0f;
    const float a = static_cast< float >( (rgba)       & 0x000000ff ) / 255.0f;
    const bool gradientColor = ((r == g) && (g == b) && (b == a) && (a == 0) );

    if ( gradientColor ) {
        // @source http://vtk.org/Wiki/VTK/Examples/Cxx/Utilities/ColorLookupTable
        point->GetPointData()->AddArray( data );

        auto lookupTable = vtkSmartPointer< vtkLookupTable >::New();
        const auto edgeZ = bm.edgeZ();
#ifdef _DEBUG
        std::cout << "Крайние значения высоты, ячейки: [ "
            << edgeZ.first << "; " << edgeZ.second << " ]"
        << std::endl;
#endif

        lookupTable->SetTableRange( edgeZ.first, edgeZ.second );
        lookupTable->SetHueRange( 0.667, 0.0 );

        /* @example Чёрно-белая картинка
        // @source http://wenku.baidu.com/view/c95242f69e31433239689326.html page 44
        lookupTable->SetHueRange( 0, 0 );
        lookupTable->SetSaturationRange( 0, 0 );
        lookupTable->SetValueRange( edgeZ.first, edgeZ.second );
        */

        // @example Красно-зелёно-синяя картинка
        // @source http://wenku.baidu.com/view/c95242f69e31433239689326.html page 44
        //lookupTable->SetHueRange( 0.0, 0.667 );

        // @test
        //lookupTable->SetTableRange( -100, 100 );
        //lookupTable->SetHueRange( 160.0 / 240.0, 0.0 );
        //lookupTable->SetSaturationRange( 1, 1 );
        //lookupTable->SetValueRange( -50, 50 );

        lookupTable->Build();

        mapper->SetLookupTable( lookupTable );
        mapper->SetScalarRange( edgeZ.first, edgeZ.second );
        mapper->ScalarVisibilityOn();
        mapper->SelectColorArray( "ElevationData" );
        mapper->SetScalarModeToUsePointFieldData();
        mapper->SetColorModeToMapScalars();

    } else {
        // цвет для всей картинки поставим Actor'ом ниже
    }

 
    auto contentActor = vtkSmartPointer< vtkActor >::New();
    contentActor->SetMapper( mapper );
    const size_t sizePoint = option[ "size-point" ];
    contentActor->GetProperty()->SetPointSize( sizePoint );
    if ( !gradientColor ) {
        contentActor->GetProperty()->SetColor( r, g, b );
    }
    // @todo contentActor->GetProperty()->SetAlpha( a );

    renderer->AddActor( contentActor );


    // Отмечаем границы холста
    auto cornerPoints = vtkSmartPointer< vtkPoints >::New();
    const bool showCorner = option[ "show-corner" ];
    if ( showCorner ) {
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
    const bool showAxes = option[ "show-axes" ];
    if ( !hasAxes && showAxes ) {
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
    renderer->GetActiveCamera()->ParallelProjectionOn();
    renderer->GetActiveCamera()->SetFocalPoint( 0, 0, 0 );
    //renderer->GetActiveCamera()->SetPosition( 0, -1, 0 );
    //renderer->GetActiveCamera()->SetViewUp( 0, 0, 1 ); 
    renderer->ResetCamera();

    renderWindow->Render();

    return *this;
}





inline void VTKVisual::wait() {
    auto renderWindowInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
}


    } // io

} // siu
