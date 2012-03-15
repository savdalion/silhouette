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
    * �������, �� ������� ������� ����.
    */
    typedef std::string uidMatter_t;
    const uidMatter_t matter;


    /**
    * @param nick �������� ��� ��� ������������� �������� ������ ������.
    *        ����� �� �����������, �.�. �� ������� ������ ���� �����, ���
    *        ���������� �������. ����� ��������� � ������ ������ �� �����
    *        �������������� ��������.
    */
    inline SolidEC(
        const uidMatter_t& matter
    ) : matter( matter ) {
    }



    inline ~SolidEC() {
    }



    /**
    * @see ElementCanvas
    */
    inline virtual cid_t cid() const {
        boost::hash< uidMatter_t >  hash;
        return hash( matter );
    }

};


}
