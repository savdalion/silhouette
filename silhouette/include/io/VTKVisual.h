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
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

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
* @template rgb ������ 4 ����� ������ ���� �� ��������� ��� ���������������
*           ������. ���� 0, ������ �������������� � ����� ���������
*           [ ����� (�������); ������� (��������) ].
*           ������ ��� �������� �����: 0xFF0000FF
*
* @source http://vtk.org
*/
template<
    size_t sizeWindowT,
    size_t sizePointT = 1,
    bool showCornerT = true,
    bool showAxesT = true,
    unsigned int rgba = 0
>
class VTKVisual {
public:
    /**
    * ��������� ���� ��� ������������.
    */
    VTKVisual();



    virtual ~VTKVisual();




    /**
    * ������������� �����. ���� ���� ������������ ��� �� ���� �������, ���
    * ��������. �����, ����� ����������� � �������� ����.
    */
    template< size_t Grid >
    VTKVisual& operator<<( const BitMap< Grid >&  bm );



    /**
    * ������� �������� ���� ������������.
    */
    void wait();





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








#include "VTKVisual.inl"
