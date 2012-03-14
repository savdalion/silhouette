#pragma once

#include <boost/tuple/tuple.hpp>


namespace siu {


/**
* ������� ������.
*
* ����� ��������� ���������� ��������� � �����, ���. ������� ���������
* ����������� � ������.
*
* @see Sketch
*/
struct ElementSketch {

    /**
    * ���������� ������ �������� �� ���� XYZ.
    */
    typedef boost::tuple< double, double, double >  psize_t;



    /**
    * @param nick �������� ��� ��� ������������� �������� ������ ������.
    *        ����� �� �����������, �.�. �� ������� ������ ���� �����, ���
    *        ���������� �������. ����� ��������� � ������ ������ �� �����
    *        �������������� ��������.
    */
    explicit inline ElementSketch(
        const std::string& nick
    ) :
        nick( nick )
    {
    }



    inline ~ElementSketch() {
    }



    /**
    * @return ���������� ������ �������� �� ���� X, Y, Z. ��������� ���
    *         �����������, �������� �� ������� ������ �� �����.
    */
    virtual psize_t psize() const = 0;




    /**
    * ������� ��� ��� ��������� � �������� ������.
    */
    const std::string nick;

};


}
