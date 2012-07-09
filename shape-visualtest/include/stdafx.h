#pragma once

// ����� ����������� ����� ������� � ����������� ��������� ������������ ��������, ��� �������
#pragma warning( disable : 4503 )

// ���������� ��������� �������, ������� ���� �������� ��� ���������� � deprecated
// @see http://lists.boost.org/Archives/boost/2006/08/109338.php 
#pragma warning( disable : 4996 )


// (!) ��������� �������������� ������� �������� ������ ���� ���. �����
//     ���������� � ��� ��� �������������� �����������.

// ����������� ������� �������������� "pcl::search::Search<PointT>::radiusSearch",
// ���������� ������ ����������� �� ��������� ���������������, ����� ���������
// ���������� ������ ��������������� const ��� volatile
//#pragma warning( disable : 4373 )

// �������������� double � int, �������� ������ ������
#pragma warning( disable : 4244 )



// �������� ������� ������ � WinDef.h
#define NOMINMAX

// ���������� �������������� ���������
#define _USE_MATH_DEFINES


#include <assert.h>
#include <algorithm>
#include <bitset>
#include <climits>
#include <cstdio>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <typeinfo>
#include <vector>
#include <windows.h>

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

#include <coord.h>
