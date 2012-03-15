#pragma once

#include "configure.h"
#include "Element.h"


namespace siu {


/**
* ������� ������.
*
* @see Sketch
*/
struct ElementCanvas : public Element {

    /**
    * ��� ����������� �������������� ��������.
    */
    typedef size_t cid_t;


    inline ElementCanvas(
        const matter_t& matter
    ) : Element( matter ) {
    }



    virtual inline ~ElementCanvas() {
    }





    /**
    * @return ���������� ������������� �������� ������.
    */
    virtual cid_t cid() const = 0;

};


}
