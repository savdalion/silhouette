namespace siu {
    namespace shape {


template< size_t Grid >
template< typename IT >
inline ElevationMap< Grid >::ElevationMap(
    const std::string& source,
    IT scaleXY,
    IT hMin, IT hMax,
    bool fill
) :
    source( source ),
    scaleXY( static_cast< float >( scaleXY ) ),
    hMin( static_cast< float >( hMin ) ),
    hMax( static_cast< float >( hMax ) ),
    hReal( static_cast< float >( hMax - hMin ) ),
    fill( fill ),
    // ������ �����������: ���������� ��� ������ ������ size()
    sizeImage_( std::make_pair( 0, 0 ) )
{
    assert( !typelib::empty( scaleXY ) && "������� �� ������." );
    assert( (scaleXY > 0.0f)
        && "������� ������ ���� ������������� ���������." );
    assert( !source.empty()
        && "�� ������ �������� ��� ������������ ����� �����." );
    assert( (hMin <= hMax)
        && "����������� ������ �� ����� ��������� ������������." );
}




template< size_t Grid >
inline ElevationMap< Grid >::~ElevationMap() {
}






template< size_t Grid >
inline float ElevationMap< Grid >::sizeMax() const {
    if (sizeImage_.first == 0) {
        sizeImage( &sizeImage_, source );
        assert( (sizeImage_.first != 0)
            && "����������� � ������ ����� �� �������� ��� �� ����������." );
        assert( (sizeImage_.first == sizeImage_.second)
            && "�������� ����� ������ � �������������� ������." );
    }
    const float ti = std::max( sizeImage_.first, sizeImage_.second ) * scaleXY;
    return std::max( hReal, ti );
}











#if 0
// - ���������� ���� �� ��������� �� ����� � ����� ������ �� ��������,
//   ���������� � ������. ����������. ��. ����.

template< size_t Grid >
typename siu::shape::ElevationMap< Grid >::bm_t siu::shape::ElevationMap< Grid >::operator()(
    const typelib::coord_t& areaMin,
    const typelib::coord_t& areaMax
) {
    // �������� ������ � ����� ��� ��������� ��������� ���-�����

    FILE* fp = fopen( source.c_str(), "rb" );
    // @todo fine ������ ������ � ���-����.
    assert( fp && "�� ������� ������� ����." );

    unsigned char sig[8];
    fread( sig, 1, 8, fp );
    const auto checkSig = png_check_sig( sig, 8 );
    assert( checkSig && "�������� �� �������� ������ � ������� PNG." );

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    assert( png && "��������� PNG �� ����������." );

    png_infop info = png_create_info_struct( png );
    assert( info && "���������� � PNG �� ��������." );

    const auto jmpbufError = setjmp( png_jmpbuf( png) );
    assert( !jmpbufError && "���������� � PNG �� ��������." );

    png_init_io( png, fp );
    png_set_sig_bytes( png, 8 );

    // @info ���� ����������� ������, �������� ���������� lpng
    //       �������������� � ������ �����. ������� �����������.
    png_read_info( png, info );

    // �������� ������ ����� ������
    const int aAllImage = png_get_image_width( png, info );
    const int bAllImage = png_get_image_height( png, info );


    // ����������� ����� ��� �����...
    auto image = (png_bytep*)malloc( sizeof( png_bytep ) * bAllImage );
    for (int j = 0; j < bAllImage; ++j) {
        image[j] = (png_byte*)malloc( png_get_rowbytes( png, info ) );
    }
    // ...� ������ ����������� ����� �����
    png_read_image( png, image );


    // ����� ������������ ������� ��� ����� ����������� �������?
    const int G = static_cast< int >( Grid );
    const float SG = sizeGrid();
    const typelib::coordInt_t gridAreaMin = (areaMin / SG).toInteger();
    const typelib::coordInt_t gridAreaMax = (areaMax / SG).toInteger();
    // @todo const bool useArea = common::empty(gridAreaMax - gridAreaMin);
    const bool useArea = false;

    // �������� ������ ��� ������ �������
    typelib::coordInt_t correctAreaMin( 0, 0, 0 );
    typelib::coordInt_t correctAreaMax( aAllImage - 1, bAllImage - 1, 0 );
    if ( useArea ) {
        if (correctAreaMin.x >= aAllImage) { correctAreaMin.x = aAllImage - 1; }
        if (correctAreaMin.y >= bAllImage) { correctAreaMin.y = bAllImage - 1; }
        if (correctAreaMax.x >= aAllImage) { correctAreaMax.x = aAllImage - 1; }
        if (correctAreaMax.y >= bAllImage) { correctAreaMax.y = bAllImage - 1; }
    }

    // ������ �������, ������� ���� ��������� � �����
    const int a = correctAreaMax.x - correctAreaMin.x + 1;
    const int b = correctAreaMax.y - correctAreaMin.y + 1;
    const int maxAB = std::max( a, b );
    assert( ( (a > 1) && (b > 1) ) && "������ �������." );


    /* - �������� � ����. ���������. ��. ����.
    // �������� ������������ ��� ������������ �����
    // ��� ���������� ������������� ����� ����������� *��� �����*
    size_t vHMin = UINT_MAX;
    size_t vHMax = 0;
    for (int j = 0; j < b; ++j) {
        // ���������� ����������� - ����������
        const png_byte* row = image[ b - j - 1 ];
        for (int i = 0; i < a; ++i) {
            // ������ ������������ �����-����� ���������: RGB ������ �����
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
    assert( (vHMax >= vHMin) && "������������ ������������ �� �������." );
    */

    size_t vHMax = 255;


    // �������� ����������� ����� �����������
    //const int sizeCube = static_cast< int >( bm.sizeCube() );
    const int maxCoordCube = static_cast< int >( bm.maxCoord().x );
    const float kv = 1.0f / static_cast< float >( vHMax );
    //const float krh = hMax / static_cast< float >( G );
    const int a0 = correctAreaMin.x;
    const int b0 = correctAreaMin.y;
    for (int j = b0; j < (b0 + b); ++j) {
        // ���������� ����������� - ����������
        const png_byte* row = image[ b0 + b - j - 1 ];
        for (int i = a0; i < (a0 + a); ++i) {
            // ������ ������������ �����-����� ���������: RGB ������ �����
            const png_byte* ptr = &( row[i] );
            const char h = static_cast< char >( ptr[0] ) - 128;
            // �������� ������ � ����
            float realH = hMax * static_cast< float >( h ) * kv;
            if (realH < 0.0f) {
                realH = 0.0f;
            } else if (realH > hMax) {
                realH = hMax;
            }
            //std:: cout << h << " " << std::endl;
            const float height = realH;
            assert( (height <= hMax)
                && "������ ����� �� ��������� �����." );

            // ��������� ���-�����
            const int gridZ = height / SG;
            const int gridX = (i - a0) * G / maxAB - maxCoordCube * a / maxAB;
            const int gridY = (j - b0) * G / maxAB - maxCoordCube * b / maxAB;
#ifdef _DEBUG
            if ( !bm.inside( gridX, gridY, gridZ ) ) {
                bool test = true;
            }
            assert( bm.inside( gridX, gridY, gridZ )
                && "���������� ����� �� ��������� �����." );
#endif
            bm.set( gridX, gridY, gridZ );

            // ��� �������������, ��������� "���"
            if ( fill ) {
                // @todo ...
            }

        } // for (size_t i = a0; i < (a0 + a); ++i)

    } // for (size_t j = b0; j < (b0 + b); ++j)

    // ������� �� �����
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
// - ����������� ���� �������� � ������� �����, �������� �������.
//   ������������� ��� ����� GraphicsMagick. ����������. ��. ����.

template< size_t Grid >
typename siu::shape::ElevationMap< Grid >::bm_t siu::shape::ElevationMap< Grid >::operator()(
    const typelib::coord_t& areaMin,
    const typelib::coord_t& areaMax
) {
    // �������� ������ � ����� ��� ��������� ��������� ���-�����

    FILE* fp = fopen( source.c_str(), "rb" );
    // @todo fine ������ ������ � ���-����.
    assert( fp && "�� ������� ������� ����." );

    unsigned char sig[8];
    fread( sig, 1, 8, fp );
    const auto checkSig = png_check_sig( sig, 8 );
    assert( checkSig && "�������� �� �������� ������ � ������� PNG." );

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    assert( png && "��������� PNG �� ����������." );

    png_infop info = png_create_info_struct( png );
    assert( info && "���������� � PNG �� ��������." );

    const auto jmpbufError = setjmp( png_jmpbuf( png) );
    assert( !jmpbufError && "���������� � PNG �� ��������." );

    png_init_io( png, fp );
    png_set_sig_bytes( png, 8 );

    // @info ���� ����������� ������, �������� ���������� lpng
    //       �������������� � ������ �����. ������� �����������.
    png_read_info( png, info );

    // �������� ������ ����� ������
    const int aAllImage = png_get_image_width( png, info );
    const int bAllImage = png_get_image_height( png, info );
    assert( (aAllImage == bAllImage)
        && "�������� ����� ������ � �������������� ������." );


    // ����������� ����� ��� �����...
    auto image = (png_bytep*)malloc( sizeof( png_bytep ) * bAllImage );
    for (int j = 0; j < bAllImage; ++j) {
        image[j] = (png_byte*)malloc( png_get_rowbytes( png, info ) );
    }
    // ...� ������ ����������� ����� �����
    png_read_image( png, image );


    // @todo ��������� ������������ �������. ������: ����������� ������ �������.
    assert( ( empty( areaMin ) && empty( areaMax ) ) && "������ �������." );


    // �������� � ����. ��������� �����
    const int vHMax = 255;


    const int G = static_cast< int >( Grid );
    const float SG = sizeGrid();
    const int maxCoordCube = static_cast< int >( bm.maxCoord().x );

    // �������� �����������: ��������� �� ����� XY
    for (int y = -maxCoordCube; y <= maxCoordCube; ++y) {
        // ������� ���������� ������ ����������� ����� �����
        const int py = G * (y + maxCoordCube) / bAllImage;
        assert( py >= 0 );
        // ���������� ����������� - ����������
        const png_byte* row = image[ bAllImage - py - 1 ];

        for (int x = -maxCoordCube; x <= maxCoordCube; ++x) {
            const int px = G * (x + maxCoordCube) / aAllImage;
            assert( px >= 0 );

            // �������� �������� ������ � ���� ������
            // ������ ������������ �����-����� ���������: RGB ������ �����
            const png_byte* ptr = &( row[px] );
            const int h = static_cast< int >( ptr[0] );
            // �������� ������ � ����
            //const float realH = hMax * static_cast< float >( h ) / vHMax;

            const int z = G * h / vHMax - maxCoordCube - 1;

            // ��������� ���-�����
#ifdef _DEBUG
            if ( !bm.inside( x, y, z ) ) {
                bool test = true;
            }
            assert( bm.inside( x, y, z )
                && "���������� ����� �� ��������� �����." );
#endif
            bm.set( x, y, z );

            // ��� �������������, ��������� "���"
            if ( fill ) {
                // @todo ...
            }

        } // for (size_t i = a0; i < (a0 + a); ++i)

    } // for (size_t j = b0; j < (b0 + b); ++j)

    // ������� �� �����
    png_destroy_read_struct( &png, nullptr, nullptr );
    fclose( fp );
    for (int j = 0; j < bAllImage; ++j) {
        free( image[j] );
    }
    free( image );


    return bm;
}

#endif







template< size_t Grid >
typename siu::shape::ElevationMap< Grid >::bm_t siu::shape::ElevationMap< Grid >::operator()(
    const typelib::coord_t& areaMin,
    const typelib::coord_t& areaMax
) {

    // ����� ������� ������ - �������� ����������� � ������� Grid: �.�.
    // �� ��� ������ ������ ������� ������ ���������� ����� �����

    // @use http://graphicsmagick.org/Magick++/Image.html

    Magick::InitializeMagick( nullptr );

    // ������ ����������� �����, �������� ��� � ������� Grid �
    // ������ ������� �� �����-�����

    Wrapper wrapper;

    std::ostringstream ss;
    ss << Grid << "x" << Grid;
    try {
        wrapper.image.read( source.c_str() );
        wrapper.image.zoom( ss.str() );
        if (wrapper.image.colorSpace() != Magick::GRAYColorspace) {
            wrapper.image.colorSpace( Magick::GRAYColorspace );
        }

    } catch( const Magick::Exception& ex ) {
        const auto exWhat = ex.what();
        //std::cout << "Caught exception: " << ex.what() << std::endl;
        assert( false && "�� ������� ��������� ����� �����." );
        return bm;
    }
    

    // �������� �� ����������� ����������� � ��������� ����� �����

    // �������� ������������ ��� ������������ �����
    // ��� ���������� ������������� ����� ����������� *��� �����*
    int vHMin = INT_MAX;
    int vHMax = INT_MIN;
    for (size_t j = 0; j < wrapper.image.size().height(); ++j) {
        for (size_t i = 0; i < wrapper.image.size().width(); ++i) {
            // ������ ������������ �����-����� ���������: R == G == B
            wrapper.color = wrapper.image.pixelColor( i, j );
            /* - ������� ��������������� ����.
            assert( ( (color.redQuantum() == color.greenQuantum())
                   && (color.redQuantum() == color.blueQuantum()) )
                && "����������� �� � ��������� ������: �������� ��������� ��� ������������ ����� �����." );
            */
            const int v = static_cast< int >( wrapper.color.redQuantum() );
            if (v < vHMin) {
                vHMin = v;
            }
            if (v > vHMax) {
                vHMax = v;
            }

        } // for (int i

    } // for (int j
    assert( (vHMax >= vHMin) && "������������ ������������ �� �������." );
    //const int vHCenter = (vHMax - vHMin) / 2;
    // �������
    const bool flat = (vHMax == vHMin);
    const int vH = (flat ? 255 : (vHMax - vHMin)) + 1;
    //const int vHMedian = vH / 2;

    // ������� �������� �� ������, ��� / ��
    const float scaleZ =
        static_cast< float >( vH ) / static_cast< float >(hMax - hMin);

    const int G = static_cast< int >( Grid );
    const float inOneG = sizeGrid();
    const int maxCoordCube = static_cast< int >( bm.maxCoord().x );
    assert( ((maxCoordCube * 2 + 1) == Grid)
        && "����� �� ����������� � �������������." );
    //const float hMedian = (hMax - hMin) / 2.0f;  // == hReal / 2.0

    //const float scale = 1.0f / sizeMax();

    // ����������� ��� ������������� ������ �������� ��������
    const float kZ = scaleXY / scaleZ;

    // ���, � ������ �������� � ������, � ��� ����� "���" ����� �����
    const int floorZ = -static_cast< int >(
        static_cast< float >( maxCoordCube ) * kZ
    );


    // �������� �����������: ��������� �� ����� XY
    for (int y = -maxCoordCube; y <= maxCoordCube; ++y) {
        const int py = y + maxCoordCube;
        // ������� ���������� ������ ����������� ����� �����
        // (����������� ��� ��������� � ������� �����)
        for (int x = -maxCoordCube; x <= maxCoordCube; ++x) {
            const int px = x + maxCoordCube;
            // �������� �������� ������ � ���� ������;
            // ������ ������������ �����-����� ���������: RGB ������ �����
            wrapper.color = wrapper.image.pixelColor( px, py );
            const int h = static_cast< int >( wrapper.color.redQuantum() ) + 1;
            if (h == 1) {
                // 0������� "��� ���", ��������
                continue;
            }

            // �������� ������ � ����
            const float realH = static_cast< float >( h ) * scaleZ;

            const float tz = (
                static_cast< float >( h ) / static_cast< float >( vHMax + 1 )
                * static_cast< float >( G )
                - static_cast< float >( maxCoordCube )
                - 1
                // ������ ���� ��������������� �� �������� ��������
            ) * kZ;
            const int z = static_cast< int >( tz );

            // ��������� ���-�����
#ifdef _DEBUG
            if ( !bm.inside( x, y, z ) ) {
                bool test = true;
            }
            assert( bm.inside( x, y, z )
                && "���������� ����� �� ��������� �����." );
#endif
            bm.set( x, y, z );

            // �� ���������� ��������� "���"
            if ( fill ) {
                for (int q = z; q > floorZ; --q) {
                    bm.set( x, y, q );
                }
            }

        } // for x

    } // for y

    return bm;
}











template< size_t Grid >
void siu::shape::ElevationMap< Grid >::sizeImage(
    std::pair< size_t, size_t >* rSizeImage,
    const std::string& source
) {

#if 0
// - �������� �� GraphicsMagick. ��. ����.
    assert( !source.empty()
        && "�� ����� ���� � ����� � ������ �����." );

    FILE* fp = fopen( source.c_str(), "rb" );
    if ( !fp ) {
        // @todo fine ������ ������ � ���-����.
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

    // @info ���� ����������� ������, �������� ���������� lpng
    //       �������������� � ������ �����. ������� �����������.
    png_read_info( png, info );

    // �������� ������ ����� ������
    const size_t a = png_get_image_width( png, info );
    const size_t b = png_get_image_height( png, info );

    return std::make_pair( a, b );
#endif

    Magick::InitializeMagick( nullptr );

    // ������ ����������� ����� � �������� ��� � ������� Grid
    Wrapper wrapper;

    try {
        wrapper.image.read( source.c_str() );

    } catch( const Magick::Exception& ex ) {
        const auto exWhat = ex.what();
        //std::cout << "Caught exception: " << ex.what() << std::endl;
        //assert( false && "�� ������� ��������� ����� �����." );
        *rSizeImage = std::make_pair( 0, 0 );
        return;
    }

    const auto geometry = wrapper.image.size();
    const size_t a = geometry.width();
    const size_t b = geometry.height();

    *rSizeImage = std::make_pair( a, b );
}



    } // shape

} // siu
