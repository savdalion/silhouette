#pragma once

#include "configure.h"
#include "RelativeCoord.h"
#include "BitContent3D.h"
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
    * ������ ������� ������ ����� �������� ���� ����� � ���� ������� �����.
    *
    * @param realSizeCanvas �������� ������ ������, �.
    * @param nCanvas ���-�� ��������� �����, �� ������� ������ �����.
    * @param coordCanvas ���������� ������ ������.
    * @param coordElementSketch ����������, ���� ������� ��������� ������� ������.
    */
    virtual BitContent3D form(
        double realSizeCanvas,
        size_t nCanvas,
        const RelativeCoord& coordCanvas,
        const RelativeCoord& coordElementSketch
    ) const = 0;



    virtual bool operator==( const ElementSketch& b ) const {
        return (nick == b.nick);
    }



    /**
    * ������� ��� ��� ��������� � �������� ������.
    */
    const std::string nick;

};


}
