#pragma once

#include "ElementCanvas.h"
#include <string>
#include <boost/functional/hash.hpp>


namespace siu {


/**
* ������� ������ � ���� �������� ����� ���������� �������.
*/
struct SolidEC : public ElementCanvas {
    /**
    * @param nick �������� ��� ��� ������������� �������� ������ ������.
    *        ����� �� �����������, �.�. �� ������� ������ ���� �����, ���
    *        ���������� �������. ����� ��������� � ������ ������ �� �����
    *        �������������� ��������.
    */
    inline SolidEC() {
    }



    inline ~SolidEC() {
    }



    /**
    * @see ElementCanvas
    */
    inline virtual size_t cid() const {
        boost::hash< uidMatter_t >  hash;
        return hash( matter );
    }



    /**
    * �������, �� ������� ������� ����.
    */
    typedef std::string uidMatter_t;
    uidMatter_t matter;

};


}
