#pragma once

#include "configure.h"
#include "RelativeCoord.h"
#include "BitContent3D.h"
#include <boost/tuple/tuple.hpp>
#include "Element.h"


namespace siu {


/**
* ������� ������.
*
* ����� ��������� ���������� ��������� � �����, ���. ������� ���������
* ����������� � ������.
*
* @see Sketch
*/
struct ElementSketch : public Element {

    /**
    * ���������� ������ �������� ������ �� ���� XYZ.
    */
    typedef boost::tuple< double, double, double >  psize_t;



    /**
    * @param nick �������� ��� ��� ������������� �������� ������ ������.
    *        ����� �� �����������, �.�. �� ������� ������ ���� �����, ���
    *        ���������� �������. ����� ��������� � ������ ������ �� �����
    *        �������������� ��������.
    */
    explicit inline ElementSketch(
        const std::string& nick,
        const matter_t& matter
    ) :
        Element( matter ), nick( nick )
    {
    }



    virtual inline ~ElementSketch() {
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
        return (nick == b.nick)
            && (matter == b.matter)
            && ( std::abs( psize().get<0>() - b.psize().get<0>() ) < d::PRECISION)
            && ( std::abs( psize().get<1>() - b.psize().get<1>() ) < d::PRECISION)
            && ( std::abs( psize().get<2>() - b.psize().get<2>() ) < d::PRECISION);
    }



    /**
    * ������� ��� ��� ��������� � �������� ������.
    */
    const std::string nick;

};


}
