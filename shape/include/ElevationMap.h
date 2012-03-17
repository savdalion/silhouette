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
* Карта высот.
*
* @template FillT Карта высот будет заполнена.
*/
template< bool FillT >
class ElevationMap : public Shape {
public:
    /**
    * @see Особенности восприятия 'source' в коде draw().
    */
    inline ElevationMap(
        const std::string& source,
        size_t sizeXY, size_t height
    ) :
        source( source ),
        sizeXY( sizeXY ),
        height( height ),
        imageScale( 1.0f ),
        imageReader( nullptr ),
        imageResizer( nullptr ),
        imageData( nullptr )
    {
        assert( ( (sizeXY > 0) && (height > 0) )
            && "Все размеры должны быть указаны." );

        // Читаем карту высот из 'source'
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
        assert( (dimension[0] == dimension[1]) && "Умеем работать только с квадратной картой высот. @todo" );
        assert( ((dimension[0] * dimension[1]) > 0) && "Изображение карты высот не содержит данных." );

        // изображение увеличивать не будем, но большое - уменьшим до 'sizeXY'
        imageScale = static_cast< float >( sizeXY ) / static_cast< float >( dimension[0] );
        if (dimension[0] > static_cast< int >( sizeXY ) ) {
            imageResizer = vtkSmartPointer< vtkImageResample >::New();
            imageResizer->SetInput( imageData );
            imageResizer->SetAxisMagnificationFactor( 0, imageScale );
            imageResizer->SetAxisMagnificationFactor( 1, imageScale );
#if VTK_MAJOR_VERSION <= 5
            imageResizer->SetInputConnection( imageData->GetProducerPort() );
#else
            imageResizer->SetInputData( imageData );
#endif
            imageResizer->Update();

            imageData = imageResizer->GetOutput();
            imageData->GetDimensions( dimension );

            // изображение уменьшено, масштаб теперь 1:1
            imageScale = 1.0f;

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

    }




    virtual inline ~ElevationMap() {
    }




    inline BitContent3D draw( size_t nCanvas, int cx, int cy, int cz ) const {
        assert( nCanvas > 0 );
        assert( ((nCanvas % 2) == 1) && "Кол-во ячеек холста должно быть нечётным." );

        const int NI = static_cast< int >( nCanvas );
        const size_t NB = nCanvas - 1;
        const int maxCanvas = (NI - 1) / 2;

        const int halfSizeXY = static_cast< int >( sizeXY ) / 2;
        //const int halfSizeHeight = static_cast< int >( height ) / 2;

        BitContent3D bc( nCanvas );

        // Соберём карту высот пробежав плоскостью XY по оси Z.
        // Воспринимаем из источника значения в диапазоне [0; 255]. Приводим
        // их к диапазону [-128; 127] и работаем с отрицательными значениями -
        // как с глубиной, с положительными - как с высотой. Центр создаваемой
        // карты высот по оси Z лежит на уровне 0.
        // @todo Работать с 16-битовыми PNG-источниками.

        for (int y = (cy - maxCanvas); y <= (cy + maxCanvas); ++y) {
            if ( (y < -halfSizeXY) || (y > halfSizeXY) ) {
                // карта меньше холста
                continue;
            }

            // интерполируем координаты XY на имеющуюся у нас карту высот
            const float fy = static_cast< float >( y + cy );
            const int yy = static_cast< int >( (fy > 0) ? std::ceil( fy ) : std::floor( fy ) );
            //assert( ( (yy >= -maxCanvas) && (yy <= maxCanvas) )
            //    && "Контроль границ Y должен был обеспечить 'imageResizer'." );
            if ( (yy < -maxCanvas) || (yy > maxCanvas) ) {
                // координата вне холста
                continue;
            }

            for (int x = (cx - maxCanvas); x <= (cy + maxCanvas); ++x) {
                if ( (x < -halfSizeXY) || (x > halfSizeXY) ) {
                    // карта меньше холста
                    continue;
                }

                const float fx = static_cast< float >( x + cx );
                const int xx = static_cast< int >( (fx > 0) ? std::ceil( fx ) : std::floor( fx ) );
                //assert( ( (xx >= -maxCanvas) && (xx <= maxCanvas) )
                //    && "Контроль границ X должен был обеспечить 'imageResizer'." );
                if ( (xx < -maxCanvas) || (xx > maxCanvas) ) {
                    // координата вне холста
                    continue;
                }

                // Для каждой точки заполняем ось Z от -128 до (h - 128).
                // Проходим по всему возможному диапазону значений высоты.
                // Значения могут быть отрицательными, т.к. источником может
                // быть карта размером меньше, чем размер холста 'nCanvas'.
                // @todo Проверить поведение при малых изображениях.
                const float fpx = static_cast< float >( xx + halfSizeXY ) / imageScale;
                const int px = static_cast< int >( (fpx > 0) ? std::ceil( fpx ) : std::floor( fpx ) );
                const float fpy = static_cast< float >( yy + halfSizeXY ) / imageScale;
                const int py = static_cast< int >( (fpy > 0) ? std::ceil( fpy ) : std::floor( fpy ) );
                const int h = static_cast< int >( pixelValue( px, py ) );

                // @todo optimize Холостой цикл при FillT == false.
                const int hd = h - 128;
                for (int kz = -128; kz <= hd; ++kz) {
                    const float fz = static_cast< float >( kz * static_cast< int >( height ) ) / 256.0f;
                    const int z = static_cast< int >( (fz > 0.0f) ? std::ceil( fz ) : std::floor( fz ) );
                    const int zz = z + cz;
                    if ( (zz < -maxCanvas) || (zz > maxCanvas) ) {
                        // координата вне холста
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
                          || (nxx == 0) || (nxx == NB)
                          || (nyy == 0) || (nyy == NB)
                          || (nzz == 0) || (nzz == NB)
                        ) {
                            bc.set( nxx, nyy, nzz );
                        }

                    } // if ( FillT )

                } // for (int kz = ...

            } // for (int x = ...

        } // for (int y = ...


        // @test
        std::cout << "Отмечено точек в карте высот: " << bc.count() << std::endl;

        return bc;
    }







protected:
    /**
    * Значение точки с координатами (px, py) в источнике 'source' с интерполяцией
    * координаты на область размером 'sizeXY'.
    */
    inline unsigned char pixelValue( int px, int py ) const {

        // Изображение подготовлено в конструкторе

        // Точки за пределами изображения считаем пропастями
        if (
            (px < 0) || (px >= dimension[0])
         || (py < 0) || (py >= dimension[1])
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
    * Прочитанные из источника данные о высотах.
    */
    vtkSmartPointer< vtkPNGReader >  imageReader;
    float imageScale;
    vtkSmartPointer< vtkImageResample >  imageResizer;
    vtkImageData* imageData;
    int dimension[ 3 ];

};


    } // shape

} // siu
