#pragma once

#include <BitMap.h>
#include <other-type.h>
#include <json-type.h>

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
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkProperty.h>


// @todo extend Создать контурный визуализатор > http://www.bu.edu/tech/research/training/tutorials/vtk/#CONTOUR


namespace siu {
    namespace io {


/**
* Визуализация средствами VTK в виде облака точек.
*
* @template sizeWindowT Размер окна визуализации, пкс. Окно - квадратное.
* @template sizePointT Размер точки, пкс.
* @template showCornerT Отмечать углы визуализируемого элемента.
* @template showAxesT Показывать оси декартовых координат.
* @template rgb Первые 4 байта задают цвет по умолчанию для визуализируемых
*           данных. Если 0, данные раскрашиваются в цвета диапазона
*           [ синий (минимум); красный (максимум) ].
*           Пример для красного цвета: 0xFF0000FF
*
* @source http://vtk.org
*/
class VTKVisual {
public:
    /**
    * Тип для задания опций визаулизатору.
    */
    typedef typelib::json::Variant  option_t;


public:
    /**
    * Открывает окно для визуализации.
    */
    VTKVisual( const io::VTKVisual::option_t& option );



    virtual ~VTKVisual();




    /**
    * Визуализирует холст. Если окно визуализации ещё не было создано, оно
    * создаётся. Иначе, холст добавляется к текущему окну.
    */
    template< size_t Grid >
    VTKVisual& operator<<( const BitMap< Grid >&  bm );



    /**
    * Ожидает закрытия окна визуализации.
    */
    void wait();





private:
    /**
    * Опции визуализатора.
    */
    typelib::json::Variant option;


    vtkSmartPointer< vtkRenderer >  renderer;
    vtkSmartPointer< vtkRenderWindow >  renderWindow;

    /**
    * Оси координат визуализированы.
    */
    bool hasAxes;

};


    } // io

} // siu








#include "VTKVisual.inl"
