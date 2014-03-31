namespace siu {
    namespace shape {


template< size_t SX, size_t SY, size_t SZ >
template< typename T >
inline ElevationMap< SX, SY, SZ >::ElevationMap(
    const std::string& source,
    T scaleXY,
    T hMin, T hMax,
    bool fill,
    const typelib::coordInt_t& shiftArea,
    const typelib::sizeInt_t& sizeArea
) :
    source( source ),
    scaleXY( static_cast< float >( scaleXY ) ),
    hMin( static_cast< float >( hMin ) ),
    hMax( static_cast< float >( hMax ) ),
    hReal( static_cast< float >( hMax - hMin ) ),
    fill( fill ),
    shiftArea( shiftArea ),
    sizeArea( sizeArea ),
    // размер изображения: необходимо для работы метода size()
    sizeImage_( std::make_pair( 0, 0 ) )
{
    assert( !typelib::empty( scaleXY ) && "Масштаб не указан." );
    assert( (scaleXY > 0.0f)
        && "Масштаб должен быть положительным значением." );
    assert( !source.empty()
        && "Не указан источник для формирования карты высот." );
    assert( (hMin <= hMax)
        && "Минимальная высота не может превышать максимальную." );
    assert( ( shiftArea.undefined() || (shiftArea >= typelib::coordInt_t::ZERO()) )
        && "Смещение области карты высот должно задаваться положительными координатами." );
    assert( ( (sizeArea.x > 0) && (sizeArea.y > 0) )
        && "Размер области должен быть задан." );
}




template< size_t SX, size_t SY, size_t SZ >
inline ElevationMap< SX, SY, SZ >::~ElevationMap() {
}






template< size_t SX, size_t SY, size_t SZ >
inline typelib::coord_t ElevationMap< SX, SY, SZ >::sizeMax() const {
    if (sizeImage_.first == 0) {
        sizeImage( &sizeImage_, source );
        assert( (sizeImage_.first != 0)
            && "Изображение с картой высот не получено или не распознано." );
        assert( (sizeImage_.first == sizeImage_.second)
            && "Работать умеем только с равносторонней картой." );
    }
    //const float ti = std::max( sizeImage_.first, sizeImage_.second ) * scaleXY;
    //return std::max( hReal, ti );
    return typelib::coord_t( sizeImage_.first * scaleXY,  sizeImage_.second * scaleXY,  hReal );
}











#if 0
// - Правильней было бы проходить по сетке и брать высоту из областей,
//   попадающих в ячейку. Переписано. См. ниже.

template< size_t Grid >
typename siu::shape::ElevationMap< Grid >::bm_t siu::shape::ElevationMap< Grid >::operator()(
    const typelib::coord_t& areaMin,
    const typelib::coord_t& areaMax
) {
    // Получаем высоты и прямо при получении формируем бит-карту

    FILE* fp = fopen( source.c_str(), "rb" );
    // @todo fine Писать ошибки в лог-файл.
    assert( fp && "Не удалось открыть файл." );

    unsigned char sig[8];
    fread( sig, 1, 8, fp );
    const auto checkSig = png_check_sig( sig, 8 );
    assert( checkSig && "Источник не является файлом в формате PNG." );

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    assert( png && "Структура PNG не распознана." );

    png_infop info = png_create_info_struct( png );
    assert( info && "Информация о PNG не получена." );

    const auto jmpbufError = setjmp( png_jmpbuf( png) );
    assert( !jmpbufError && "Информация о PNG не получена." );

    png_init_io( png, fp );
    png_set_sig_bytes( png, 8 );

    // @info Если выбрасывает ошибку, вероятно библиотека lpng
    //       скомпилирована в другой среде. Сделать одинаковыми.
    png_read_info( png, info );

    // Выясняем размер файла образа
    const int aAllImage = png_get_image_width( png, info );
    const int bAllImage = png_get_image_height( png, info );


    // Резервируем место под образ...
    auto image = (png_bytep*)malloc( sizeof( png_bytep ) * bAllImage );
    for (int j = 0; j < bAllImage; ++j) {
        image[j] = (png_byte*)malloc( png_get_rowbytes( png, info ) );
    }
    // ...и читаем изображение карты высот
    png_read_image( png, image );


    // Нужно использовать область или брать изображение целиком?
    const int G = static_cast< int >( Grid );
    const float SG = sizeGrid();
    const typelib::coordInt_t gridAreaMin = (areaMin / SG).toInteger();
    const typelib::coordInt_t gridAreaMax = (areaMax / SG).toInteger();
    // @todo const bool useArea = common::empty(gridAreaMax - gridAreaMin);
    const bool useArea = false;

    // Подводим размер под размер области
    typelib::coordInt_t correctAreaMin( 0, 0, 0 );
    typelib::coordInt_t correctAreaMax( aAllImage - 1, bAllImage - 1, 0 );
    if ( useArea ) {
        if (correctAreaMin.x >= aAllImage) { correctAreaMin.x = aAllImage - 1; }
        if (correctAreaMin.y >= bAllImage) { correctAreaMin.y = bAllImage - 1; }
        if (correctAreaMax.x >= aAllImage) { correctAreaMax.x = aAllImage - 1; }
        if (correctAreaMax.y >= bAllImage) { correctAreaMax.y = bAllImage - 1; }
    }

    // размер области, которую надо затронуть в файле
    const int a = correctAreaMax.x - correctAreaMin.x + 1;
    const int b = correctAreaMax.y - correctAreaMin.y + 1;
    const int maxAB = std::max( a, b );
    assert( ( (a > 1) && (b > 1) ) && "Пустая область." );


    /* - Работаем в фикс. диапазоне. См. ниже.
    // Получаем коэффициенты для нормализации высот
    // Для правильных коэффициентов нужно просмотреть *всю карту*
    size_t vHMin = UINT_MAX;
    size_t vHMax = 0;
    for (int j = 0; j < b; ++j) {
        // Полученное изображение - перевёрнуто
        const png_byte* row = image[ b - j - 1 ];
        for (int i = 0; i < a; ++i) {
            // высота представлена чёрно-белой градацией: RGB всегда равны
            const png_byte* ptr = &( row[i] );
            const size_t v = static_cast< size_t >( ptr[0] );
            if (v < vHMin) {
                vHMin = v;
            }
            if (v > vHMax) {
                vHMax = v;
            }

        } // for (int i = 0; i < a; ++i)

    } // for (int j = 0; j < b; ++j)
    assert( (vHMax >= vHMin) && "Коэффициенты нормализации не собраны." );
    */

    size_t vHMax = 255;


    // Собираем запрошенную часть поверхности
    //const int sizeCube = static_cast< int >( bm.sizeCube() );
    const int maxCoordCube = static_cast< int >( bm.maxCoord().x );
    const float kv = 1.0f / static_cast< float >( vHMax );
    //const float krh = hMax / static_cast< float >( G );
    const int a0 = correctAreaMin.x;
    const int b0 = correctAreaMin.y;
    for (int j = b0; j < (b0 + b); ++j) {
        // Полученное изображение - перевёрнуто
        const png_byte* row = image[ b0 + b - j - 1 ];
        for (int i = a0; i < (a0 + a); ++i) {
            // высота представлена чёрно-белой градацией: RGB всегда равны
            const png_byte* ptr = &( row[i] );
            const char h = static_cast< char >( ptr[0] ) - 128;
            // Реальная высота в мире
            float realH = hMax * static_cast< float >( h ) * kv;
            if (realH < 0.0f) {
                realH = 0.0f;
            } else if (realH > hMax) {
                realH = hMax;
            }
            //std:: cout << h << " " << std::endl;
            const float height = realH;
            assert( (height <= hMax)
                && "Высота лежит за границами сетки." );

            // Формируем бит-карту
            const int gridZ = height / SG;
            const int gridX = (i - a0) * G / maxAB - maxCoordCube * a / maxAB;
            const int gridY = (j - b0) * G / maxAB - maxCoordCube * b / maxAB;
#ifdef _DEBUG
            if ( !bm.inside( gridX, gridY, gridZ ) ) {
                bool test = true;
            }
            assert( bm.inside( gridX, gridY, gridZ )
                && "Координаты лежат за пределами сетки." );
#endif
            bm.set( gridX, gridY, gridZ );

            // При необходимости, заполняем "низ"
            if ( fill ) {
                // @todo ...
            }

        } // for (size_t i = a0; i < (a0 + a); ++i)

    } // for (size_t j = b0; j < (b0 + b); ++j)

    // Убираем за собой
    png_destroy_read_struct( &png, nullptr, nullptr );
    fclose( fp );
    for (int j = 0; j < bAllImage; ++j) {
        free( image[j] );
    }
    free( image );


    return bm;
}

#endif









#if 0
// - Изображение надо привести к размеру сетки, усреднив значеия.
//   Воспользуемся для этого GraphicsMagick. Переписано. См. ниже.

template< size_t Grid >
typename siu::shape::ElevationMap< Grid >::bm_t siu::shape::ElevationMap< Grid >::operator()(
    const typelib::coord_t& areaMin,
    const typelib::coord_t& areaMax
) {
    // Получаем высоты и прямо при получении формируем бит-карту

    FILE* fp = fopen( source.c_str(), "rb" );
    // @todo fine Писать ошибки в лог-файл.
    assert( fp && "Не удалось открыть файл." );

    unsigned char sig[8];
    fread( sig, 1, 8, fp );
    const auto checkSig = png_check_sig( sig, 8 );
    assert( checkSig && "Источник не является файлом в формате PNG." );

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    assert( png && "Структура PNG не распознана." );

    png_infop info = png_create_info_struct( png );
    assert( info && "Информация о PNG не получена." );

    const auto jmpbufError = setjmp( png_jmpbuf( png) );
    assert( !jmpbufError && "Информация о PNG не получена." );

    png_init_io( png, fp );
    png_set_sig_bytes( png, 8 );

    // @info Если выбрасывает ошибку, вероятно библиотека lpng
    //       скомпилирована в другой среде. Сделать одинаковыми.
    png_read_info( png, info );

    // Выясняем размер файла образа
    const int aAllImage = png_get_image_width( png, info );
    const int bAllImage = png_get_image_height( png, info );
    assert( (aAllImage == bAllImage)
        && "Работать умеем только с равносторонней картой." );


    // Резервируем место под образ...
    auto image = (png_bytep*)malloc( sizeof( png_bytep ) * bAllImage );
    for (int j = 0; j < bAllImage; ++j) {
        image[j] = (png_byte*)malloc( png_get_rowbytes( png, info ) );
    }
    // ...и читаем изображение карты высот
    png_read_image( png, image );


    // @todo Позволить использовать область. Сейчас: изображение берётся целиком.
    assert( ( empty( areaMin ) && empty( areaMax ) ) && "Пустая область." );


    // Работаем в фикс. диапазоне высот
    const int vHMax = 255;


    const int G = static_cast< int >( Grid );
    const float SG = sizeGrid();
    const int maxCoordCube = static_cast< int >( bm.maxCoord().x );

    // Собираем поверхность: пробегаем по сетке XY
    for (int y = -maxCoordCube; y <= maxCoordCube; ++y) {
        // Находим координаты внутри изображения карты высот
        const int py = G * (y + maxCoordCube) / bAllImage;
        assert( py >= 0 );
        // Полученное изображение - перевёрнуто
        const png_byte* row = image[ bAllImage - py - 1 ];

        for (int x = -maxCoordCube; x <= maxCoordCube; ++x) {
            const int px = G * (x + maxCoordCube) / aAllImage;
            assert( px >= 0 );

            // Получаем значение высоты в этой ячейке
            // высота представлена чёрно-белой градацией: RGB всегда равны
            const png_byte* ptr = &( row[px] );
            const int h = static_cast< int >( ptr[0] );
            // Реальная высота в мире
            //const float realH = hMax * static_cast< float >( h ) / vHMax;

            const int z = G * h / vHMax - maxCoordCube - 1;

            // Формируем бит-карту
#ifdef _DEBUG
            if ( !bm.inside( x, y, z ) ) {
                bool test = true;
            }
            assert( bm.inside( x, y, z )
                && "Координаты лежат за пределами сетки." );
#endif
            bm.set( x, y, z );

            // При необходимости, заполняем "низ"
            if ( fill ) {
                // @todo ...
            }

        } // for (size_t i = a0; i < (a0 + a); ++i)

    } // for (size_t j = b0; j < (b0 + b); ++j)

    // Убираем за собой
    png_destroy_read_struct( &png, nullptr, nullptr );
    fclose( fp );
    for (int j = 0; j < bAllImage; ++j) {
        free( image[j] );
    }
    free( image );


    return bm;
}

#endif







template< size_t SX, size_t SY, size_t SZ >
typename ElevationMap< SX, SY, SZ >::bm_t ElevationMap< SX, SY, SZ >::operator()() {
    // Приводим изображение к размеру Grid: т.о. мы без лишних хлопот
    // получим хорошо усреднённую карту высот

    // @use http://graphicsmagick.org/Magick++/Image.html

    Magick::InitializeMagick( nullptr );

    // Читаем изображение карты, приводим картинку к размеру сетки с учётом
    // требуемого участка и масштаба, меняем палитру на чёрно-белую

    Wrapper wrapper;

    size_t imageSizeWidth = 0;
    size_t imageSizeHeight = 0;
    try {
        wrapper.image.read( source.c_str() );
        imageSizeWidth  = wrapper.image.size().width();
        imageSizeHeight = wrapper.image.size().height();

        if (wrapper.image.colorSpace() != Magick::GRAYColorspace) {
            // @source http://imagemagick.org/Magick++/Image.html
            wrapper.image.quantizeColorSpace( Magick::GRAYColorspace );
            wrapper.image.quantizeColors( 256 );
            wrapper.image.quantize();
        }

    } catch( const Magick::Exception& ex ) {
        const auto exWhat = ex.what();
        //std::cout << "Caught exception: " << ex.what() << std::endl;
        assert( false && "Не удалось прочитать карту высот." );
        return bm;
    }
    


    // До того, как изменить изображение, надо получить полную картину о карте
    // высот: когда возьмём часть изображения (см. shiftArea и sizeArea),
    // информация о высотах будет искажена
    int fullVHMin = INT_MAX;
    int fullVHMax = INT_MIN;
    for (size_t j = 0; j < imageSizeHeight; ++j) {
        for (size_t i = 0; i < imageSizeWidth; ++i) {
            // высота представлена чёрно-белой градацией: R == G == B
            wrapper.color = wrapper.image.pixelColor( i, j );
            const int v = static_cast< int >( wrapper.color.redQuantum() );
            if (v < fullVHMin) {
                fullVHMin = v;
            }
            if (v > fullVHMax) {
                fullVHMax = v;
            }

        } // for (int i

        // если полный диапазон, не имеет смысла прочёсывать изображение дальше
        if ( (fullVHMin == 0) && (fullVHMax == 255) ) {
            break;
        }

    } // for (int j

    assert( (fullVHMax >= fullVHMin)
        && "Коэффициенты нормализации для полной карты высот не собраны." );

    // равнина
    const bool flat = (fullVHMax == fullVHMin);
    const int fullVH = (flat ? 255 : (fullVHMax - fullVHMin)) + 1;


    // Теперь можем оставить только нужную часть изображения
    // @source http://imagemagick.org/script/command-line-processing.php?ImageMagick=3buive6jn1i4t9np2jitq25fa6#geometry
    const bool hasArea = !shiftArea.undefined();
    Magick::Geometry cropGeometry( 0, 0, 0, 0 );
    if ( hasArea ) {
        assert( ((shiftArea.x + sizeArea.x) < static_cast< int >( imageSizeWidth ))
            && "Область обрезки изображения по X за пределами границ изображения." );
        assert( ((shiftArea.y + sizeArea.y) < static_cast< int >( imageSizeHeight ))
            && "Область обрезки изображения по Y за пределами границ изображения." );
        cropGeometry =
            Magick::Geometry( shiftArea.x, shiftArea.y,  sizeArea.x, sizeArea.y );
    }

    // изображение (или его часть) всегда приводится к размеру сетки
    const Magick::Geometry zoomGeometry( SX, SY );
    
    try {
        if ( hasArea ) {
            wrapper.image.crop( cropGeometry );
            /* - @todo fine Убирать прозрачность. Код ниже - не помогает.
            wrapper.image.opacity( 0 );
            wrapper.image.syncPixels();
            */
#ifdef _DEBUG
            // @test
            const std::string file = "elevation-map-crop.png";
            wrapper.image.write( file );
#endif
        }
        wrapper.image.zoom( zoomGeometry );

#ifdef _DEBUG
        // @test
        const std::string file = "elevation-map-crop-zoom.png";
        wrapper.image.write( file );
#endif

    } catch( const Magick::Exception& ex ) {
        const auto exWhat = ex.what();
        assert( false && "Не удалось прочитать карту высот." );
        return bm;
    }

    // размер изображения (в общем случае) изменён
    imageSizeWidth  = wrapper.image.size().width();
    imageSizeHeight = wrapper.image.size().height();



    // Проходим по полученному изображению и формируем сетку высот

    // Получаем коэффициенты для нормализации высот
    int vHMin = INT_MAX;
    int vHMax = INT_MIN;
    for (size_t j = 0; j < imageSizeHeight; ++j) {
        for (size_t i = 0; i < imageSizeWidth; ++i) {
            // высота представлена чёрно-белой градацией: R == G == B
            wrapper.color = wrapper.image.pixelColor( i, j );
            const int v = static_cast< int >( wrapper.color.redQuantum() );
            if (v < vHMin) {
                vHMin = v;
            }
            if (v > vHMax) {
                vHMax = v;
            }

        } // for (int i

        // если получен полный диапазон, не имеет смысла дальше прочёсывать изображение
        if ( (vHMin == 0) && (vHMax == 255) ) {
            break;
        }

    } // for (int j

    assert( (vHMax >= vHMin)
        && "Коэффициенты нормализации для части карты высот не собраны." );

    const int vH = (flat ? 255 : (vHMax - vHMin)) + 1;

    // Высота области будет пропорционально отличаться от высоты, заданной для
    // целого изображения
    const float kh = static_cast< float >( vH ) / static_cast< float >( fullVH );
    const float khMax = kh * hMax;
    const float khMin = kh * hMin;

    // Масштаб картинки по осям, пкс / км
    const float scaleX = scaleXY;
    const float scaleY = scaleXY;
    /* - @todo Правильно?
    const float scaleZ = static_cast< float >( vH ) / (khMax - khMin);
    */
    const float scaleZ = (khMax - khMin) / static_cast< float >( SZ )
        * (static_cast< float >( SZ ) / static_cast< float >( SX ));
    /*
    const float scaleZ = static_cast< float >( vH ) / (khMax - khMin)
        * (static_cast< float >( SX ) / static_cast< float >( SZ ));
    */

    const typelib::coordInt_t S( SX, SY, SZ );
    const typelib::coord_t inOneG = sizeGrid();
    const typelib::coordInt_t maxCoord = bm_t::maxCoord();
    assert( ((maxCoord * 2 + 1) == S)
        && "Сетка карты высот не согласована с координатами." );

    // Коэффициент для корректировки согласно масштабу
    const float kX = scaleXY / scaleX;
    const float kY = scaleXY / scaleY;
    /*
    const float kZ =
        (static_cast< float >( vH ) / static_cast< float >( fullVH ))
      * scaleXY / scaleZ;
    */
    const float kZ = scaleXY / scaleZ;

    // Где, с учётом масштаба и высоты, у нас будет "пол" карты высот
    int floorZ = -static_cast< int >(
        static_cast< float >( maxCoord.z ) * kZ
    );
    /* - ? пол требует корректировки при частичном формировании карты высот
    if (floorZ < bm_t::minCoord().z) {
        floorZ = bm_t::minCoord().z;
    }
    */
    assert( typelib::between( floorZ, bm_t::minCoord().z, bm_t::maxCoord().z )
        && "Пол карты высот лежит вне Z-границ карты." );


    // Собираем поверхность: пробегаем по сетке XY
    for (int y = -maxCoord.y; y <= maxCoord.y; ++y) {
        const int py = static_cast< int >( (y + maxCoord.y) * kY );
        // Находим координаты внутри изображения карты высот
        // (изображение уже приведено к размеру сетки)
        for (int x = -maxCoord.x; x <= maxCoord.x; ++x) {
            const int px = static_cast< int >( (x + maxCoord.x) * kX );
            // Получаем значение высоты в этой ячейке;
            // высота представлена чёрно-белой градацией: RGB всегда равны
            wrapper.color = wrapper.image.pixelColor( px, py );
            const int h = static_cast< int >( wrapper.color.redQuantum() ) + 1;
            if (h == 1) {
                // 0значает "нет дна", пропасть
                continue;
            }

            // Реальная высота в мире
            const float realH = static_cast< float >( h ) * scaleZ;

            const float tz = (
                static_cast< float >( h ) / static_cast< float >( vHMax + 1 )
                * static_cast< float >( S.z )
                - static_cast< float >( maxCoord.z )
                - 1
                // высоту надо скорректировать по масштабу картинки
            ) * kZ;
            const int z = static_cast< int >( tz );

            // Формируем бит-карту
#ifdef _DEBUG
            if ( !bm.inside( x, y, z ) ) {
                bool test = true;
            }
            assert( bm.inside( x, y, z )
                && "Координаты лежат за пределами сетки." );
#endif
            bm.set( x, y, z );

            // По требованию заполняем "низ"
            if ( fill ) {
                for (int q = z; q > floorZ; --q) {
                    bm.set( x, y, q );
                }
            }

        } // for x

    } // for y

    return bm;
}











template< size_t SX, size_t SY, size_t SZ >
void ElevationMap< SX, SY, SZ >::sizeImage(
    std::pair< size_t, size_t >* rSizeImage,
    const std::string& source
) {

#if 0
// - Заменено на GraphicsMagick. См. ниже.
    assert( !source.empty()
        && "Не задан путь к файлу с картой высот." );

    FILE* fp = fopen( source.c_str(), "rb" );
    if ( !fp ) {
        // @todo fine Писать ошибки в лог-файл.
        //throw "File with heightmap is not open.";
        return std::make_pair( 0, 0 );
    }

    unsigned char sig[8];
    fread( sig, 1, 8, fp );
    if ( !png_check_sig( sig, 8 ) ) {
        //throw "File with heightmap is not in PNG format.";
        return std::make_pair( 0, 0 );
    }

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    if ( !png ) {
        //throw "PNG struct is not created.";
        return std::make_pair( 0, 0 );
    }

    png_infop info = png_create_info_struct( png );
    if ( !info ) {
        png_destroy_read_struct( &png, nullptr, nullptr );
        //throw "Info struct is not created.";
        return std::make_pair( 0, 0 );
    }

    if ( setjmp( png_jmpbuf( png) ) ) {
        //throw "Error during init_io.";
        return std::make_pair( 0, 0 );
    }

    png_init_io( png, fp );
    png_set_sig_bytes( png, 8 );

    // @info Если выбрасывает ошибку, вероятно библиотека lpng
    //       скомпилирована в другой среде. Сделать одинаковыми.
    png_read_info( png, info );

    // Выясняем размер файла образа
    const size_t a = png_get_image_width( png, info );
    const size_t b = png_get_image_height( png, info );

    return std::make_pair( a, b );
#endif

    Magick::InitializeMagick( nullptr );

    // Читаем изображение карты и приводим его к размеру сетки
    Wrapper wrapper;

    try {
        wrapper.image.read( source.c_str() );

    } catch( const Magick::Exception& ex ) {
        const auto exWhat = ex.what();
        //std::cout << "Caught exception: " << ex.what() << std::endl;
        //assert( false && "Не удалось прочитать карту высот." );
        *rSizeImage = std::make_pair( 0, 0 );
        return;
    }

    const auto geometry = wrapper.image.size();
    const size_t a = geometry.width();
    const size_t b = geometry.height();

    *rSizeImage = std::make_pair( a, b );
}






template< size_t SX, size_t SY, size_t SZ >
inline typename Shape< SX, SY, SZ >::Ptr  ElevationMap< SX, SY, SZ >::clone() const {
    return ElevationMap< SX, SY, SZ >::Ptr( new ElevationMap< SX, SY, SZ >( *this ) );
}



    } // shape

} // siu
