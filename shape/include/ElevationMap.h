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
*  арта высот.
*
* @template FillT  арта высот будет заполнена.
*/
template< bool FillT >
class ElevationMap : public Shape {
public:
    /**
    * @see ќсобенности воспри€ти€ 'source' в коде draw().
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
            && "¬се размеры должны быть указаны." );
    }


    virtual inline ~ElevationMap() {
    }



    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && " ол-во €чеек холста должно быть нечЄтным." );

        const int NI = static_cast< int >( nCanvas );
        const size_t NP = nCanvas - 1;
        const int maxCanvas = (NI - 1) / 2;

        BitContent3D bc( nCanvas );

        // —оберЄм карту высот пробежав плоскостью XY по оси Z.
        // ¬оспринимаем из источника значени€ в диапазоне [0; 255]. ѕриводим
        // их к диапазону [-128; 127] и работаем с отрицательными значени€ми -
        // как с глубиной, с положительными - как с высотой. ÷ентр создаваемой
        // карты высот по оси Z лежит на уровне 0.
        // @todo –аботать с 16-битовыми PNG-источниками.

        for (int y = 0; y < NI; ++y) {
            const int yy = y + cy;
            if ( (yy < -maxCanvas) || (yy > maxCanvas) ) {
                // точка за границами холста
                continue;
            }
            for (int x = 0; x < NI; ++x) {
                const int xx = x + cx;
                if ( (xx < -maxCanvas) || (xx > maxCanvas) ) {
                    // точка за границами холста
                    continue;
                }
                // ƒл€ каждой точки заполн€ем ось Z от -128 до (h - 128).
                // ѕроходим по всему возможному диапазону значений высоты.
                const int h = static_cast< int >( pixelValue(
                    static_cast< size_t >( x ),
                    static_cast< size_t >( y )
                ) );

                // @todo optimize ’олостой цикл при FillT == false.
                const int hd = h - 128;
                for (int kz = -128; kz <= hd; ++kz) {
                    const float fz = static_cast< float >( kz * static_cast< int >( height ) ) / 256.0f * 2.0f;
                    const int z = static_cast< int >( (fz > 0.0f) ? std::ceil( fz ) : std::floor( fz ) );
                    const int zz = z + cz;
                    if ( (zz < -maxCanvas) || (zz > maxCanvas) ) {
                        // точка за границами холста
                        continue;
                    }
                    const size_t nxx = static_cast< size_t >( xx + maxCanvas );
                    const size_t nyy = static_cast< size_t >( yy + maxCanvas );
                    const size_t nzz = static_cast< size_t >( zz + maxCanvas );
                    if ( FillT ) {
                        // заполнено целиком
                        bc.set( nxx, nyy, nzz );
                    } else {
                        // только поверхность
                        if (
                          // крайние точки (дно и вершина)
                             ( (kz == -128) || (kz == hd) )
                          // точки на границах холста
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
        std::cout << "ќтмечено точек в карте высот: " << bc.count() << std::endl;

        return bc;
    }







protected:
    /**
    * «начение точки с координатами (px, py) в источнике 'source' с интерпол€цией
    * координаты на область размером 'sizeXY'.
    */
    inline unsigned char pixelValue( size_t px, size_t py ) const {

        // ѕри первом запросе читаем карту высот из 'source'
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
            assert( (dimension[0] == dimension[1]) && "”меем работать только с квадратной картой высот. @todo" );
            assert( ((dimension[0] * dimension[1]) > 0) && "»зображение карты высот не содержит данных." );

            // изображение увеличивать не будем, но большое - уменьшим
            if (dimension[0] > static_cast< int >( sizeXY ) ) {
                const float k = static_cast< float >( sizeXY ) / static_cast< float >( dimension[0] );

                /* - –аботает с €ркостью. «аменено. —м. ниже.
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


        // “очки за пределами изображени€ считаем пропаст€ми
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
    * ѕрочитанные из источника данные о высотах.
    */
    mutable vtkSmartPointer< vtkPNGReader >  imageReader;
    mutable vtkSmartPointer< vtkImageResample >  imageResizer;
    mutable vtkImageData* imageData;
    mutable int dimension[ 3 ];

};


    } // shape

} // siu
