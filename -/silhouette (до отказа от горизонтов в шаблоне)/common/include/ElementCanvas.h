#pragma once


namespace siu {


/**
* ������� ������.
*
* @see Sketch
*/
struct ElementCanvas {

    inline ElementCanvas() {
    }



    inline ~ElementCanvas() {
    }





    /**
    * @return ���������� ������������� ��������.
    */
    virtual size_t cid() const = 0;

};


}
