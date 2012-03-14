#pragma once


namespace siu {


/**
* ����������.
* ������� ��� ������������������ �������� � ������, ����������������
* ������ ������������ ��������� � �.�..
*
* @see IntegerRelativeCoord
*/
template< size_t K = 3 >
struct RelativeCoord {

    /**
    * ��������: �������.
    * ���������� ����������� ������������ ������ ����� ���������.
    */
    const int hCeil;

    /**
    * 3D-���������� � ��������� 'double'.
    */
    const double x;
    const double y;
    const double z;



    explicit inline RelativeCoord(
        // ���������� �������� ���������: ���������� ������� ����� 'double'.
        // �.�. ������� ������ �� �������� � ������������� ����������, �
        // ������� �� IntegerRelativeCoord.
        int hCeil,
        double x = 0.0, double y = 0.0, double z = 0.0
    ) :
        hCeil( hCeil ),
        x( x ), y( y ), z( z )
    {
    }

};


}
