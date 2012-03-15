#pragma once

#include <string>


namespace siu {


/**
* ������� - ������ ��� ���������� ������ � ������.
*/
struct Element {

    /**
    * �������, �� ������� ������� �������.
    */
    typedef std::string matter_t;
    const matter_t matter;


    /**
    * @param nick �������� ��� ��� ������������� ��������.
    */
    inline Element(
        const matter_t& matter
    ) : matter( matter ) {
    }



    virtual inline ~Element() {
    }



};


}
