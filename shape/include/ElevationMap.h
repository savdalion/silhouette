#pragma once

#include "Shape.h"
#include <string>
#include <vtkPNGReader.h>
#include <vtkPNGWriter.h>
#include <vtkImageData.h>
#include <vtkImageResample.h>


namespace siu {
    namespace shape {

/**
* ����� �����.
*
* @template FillT ����� ����� ����� ���������.
*/
template< bool FillT >
class ElevationMap : public Shape {
public:
    /**
    * @see ����������� ���������� 'source' � ���� draw().
    */
    inline ElevationMap(
        const std::string& source,
        size_t sizeXY, size_t height
    ) :
        source( source ),
        sizeXY( sizeXY ),
        height( height ),
        imageReader( nullptr ),
        imageData( nullptr )
    {
        assert( ( (sizeXY > 0) && (height > 0) )
            && "��� ������� ������ ���� �������." );
    }


    virtual inline ~ElevationMap() {
    }



    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && "���-�� ����� ������ ������ ���� ��������." );

        const int NI = static_cast< int >( nCanvas );
        const size_t NP = nCanvas - 1;
        const int maxCanvas = (NI - 1) / 2;

        BitContent3D bc( nCanvas );

        // ������ ����� ����� �������� ���������� XY �� ��� Z.
        // ������������ �� ��������� �������� � ��������� [0; 255]. ��������
        // �� � ��������� [-128; 127] � �������� � �������������� ���������� -
        // ��� � ��������, � �������������� - ��� � �������. ����� �����������
        // ����� ����� �� ��� Z ����� �� ������ 0.
        // @todo �������� � 16-�������� PNG-�����������.

        for (int y = 0; y < NI; ++y) {
            const int yy = y + cy;
            if ( (yy < -maxCanvas) || (yy > maxCanvas) ) {
                // ����� �� ��������� ������
                continue;
            }
            for (int x = 0; x < NI; ++x) {
                const int xx = x + cx;
                if ( (xx < -maxCanvas) || (xx > maxCanvas) ) {
                    // ����� �� ��������� ������
                    continue;
                }
                // ��� ������ ����� ��������� ��� Z �� -128 �� (h - 128).
                // �������� �� ����� ���������� ��������� �������� ������.
                const int h = static_cast< int >( pixelValue(
                    static_cast< size_t >( x ),
                    static_cast< size_t >( y )
                ) );

                // @todo optimize �������� ���� ��� FillT == false.
                const int hd = h - 128;
                for (int kz = -128; kz <= hd; ++kz) {
                    const float fz = static_cast< float >( kz * static_cast< int >( height ) ) / 256.0f * 2.0f;
                    const int z = static_cast< int >( (fz > 0.0f) ? std::ceil( fz ) : std::floor( fz ) );
                    const int zz = z + cz;
                    if ( (zz < -maxCanvas) || (zz > maxCanvas) ) {
                        // ����� �� ��������� ������
                        continue;
                    }
                    const size_t nxx = static_cast< size_t >( xx + maxCanvas );
                    const size_t nyy = static_cast< size_t >( yy + maxCanvas );
                    const size_t nzz = static_cast< size_t >( zz + maxCanvas );
                    if ( FillT ) {
                        // ��������� �������
                        bc.set( nxx, nyy, nzz );
                    } else {
                        // ������ �����������
                        if (
                          // ������� ����� (��� � �������)
                             ( (kz == -128) || (kz == hd) )
                          // ����� �� �������� ������
                          || (nxx == 0) || (nxx == NP)
                          || (nyy == 0) || (nyy == NP)
                          || (nzz == 0) || (nzz == NP)
                        ) {
                            bc.set( nxx, nyy, nzz );
                        }

                    } // if ( FillT )

                } // for (int kz = -127; kz < hd; ++kz)

            } // for (int x = 0; x < NI; ++x)

        } // for (int y = 0; y < NI; ++y)

        // @test
        std::cout << "�������� ����� � ����� �����: " << bc.count() << std::endl;

        return bc;
    }







protected:
    /**
    * �������� ����� � ������������ (px, py) � ��������� 'source' � �������������
    * ���������� �� ������� �������� 'sizeXY'.
    */
    inline unsigned char pixelValue( size_t px, size_t py ) const {

        // ��� ������ ������� ������ ����� ����� �� 'source'
        if ( !imageData ) {
            imageReader = vtkSmartPointer< vtkPNGReader >::New();
            /*
            const char* fname =
                vtkTestUtilities::ExpandDataFileName( 0, nullptr, source.c_str() );
            reader->SetFileName( fname );
            delete[] fname;
            */
            imageReader->SetFileName( source.c_str() );
            imageReader->Update();
            imageData = vtkImageData::SafeDownCast( imageReader->GetOutput() );

            imageData->GetDimensions( dimension );
            assert( (dimension[0] == dimension[1]) && "����� �������� ������ � ���������� ������ �����. @todo" );
            assert( ((dimension[0] * dimension[1]) > 0) && "����������� ����� ����� �� �������� ������." );

            // ����������� ����������� �� �����, �� ������� - ��������
            if (dimension[0] > static_cast< int >( sizeXY ) ) {
                const float k = static_cast< float >( sizeXY ) / static_cast< float >( dimension[0] );

                /* - �������� � ��������. ��������. ��. ����.
                scaleFilter = vtkSmartPointer< vtkImageShiftScale >::New();
                scaleFilter->SetOutputScalarTypeToUnsignedChar();
#if VTK_MAJOR_VERSION <= 5
                scaleFilter->SetInputConnection( imageData->GetProducerPort() );
#else
                scaleFilter->SetInputData( imageData );
#endif
                scaleFilter->SetScale( k );
                scaleFilter->Update();

                imageData = scaleFilter->GetOutput();
                imageData->GetDimensions( dimension );
            */
                imageResizer = vtkSmartPointer< vtkImageResample >::New();
                imageResizer->SetInput( imageData );
                imageResizer->SetAxisMagnificationFactor( 0, k );
                imageResizer->SetAxisMagnificationFactor( 1, k );
#if VTK_MAJOR_VERSION <= 5
                imageResizer->SetInputConnection( imageData->GetProducerPort() );
#else
                imageResizer->SetInputData( imageData );
#endif
                imageResizer->Update();

                imageData = imageResizer->GetOutput();
                imageData->GetDimensions( dimension );

                /* @test
                auto imageWriter = vtkSmartPointer< vtkPNGWriter >::New();
                const std::string file = "V:/bin/debug-silhuette/pixelValue-ElevationMap-scale-test.png";
                imageWriter->SetFileName( file.c_str() );
                imageWriter->SetInput( imageData );
                imageWriter->Write();
                */

            } // if (dimension[0] > static_cast< int >( sizeXY ) )

#if VTK_MAJOR_VERSION <= 5
            imageData->SetNumberOfScalarComponents( 1 );
            imageData->SetScalarTypeToUnsignedChar();
            imageData->AllocateScalars();
#else
            imageData->AllocateScalars( VTK_UNSIGNED_CHAR, 1 );
#endif

        } // if ( !imageData )


        // ����� �� ��������� ����������� ������� ����������
        if ( (static_cast< int >( px ) >= dimension[0])
          || (static_cast< int >( py ) >= dimension[1])
        ) {
            return 0;
        }

        unsigned char* pixel = static_cast< unsigned char* >(
            imageData->GetScalarPointer( px, py, 0 )
        );

        // @test
        //std::cout << px << " " << py << " " << pixel[0] << std::endl;

        return pixel[0];
    }





protected:
    const std::string source;
    const size_t sizeXY;
    const size_t height;



private:
    /**
    * ����������� �� ��������� ������ � �������.
    */
    mutable vtkSmartPointer< vtkPNGReader >  imageReader;
    mutable vtkSmartPointer< vtkImageResample >  imageResizer;
    mutable vtkImageData* imageData;
    mutable int dimension[ 3 ];

};


    } // shape

} // siu
