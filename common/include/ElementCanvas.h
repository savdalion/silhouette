#pragma once


namespace siu {


/**
* ������� ������.
*
* @see Sketch
*/
struct ElementCanvas {

    /**
    * ��� ����������� �������������� ��������.
    */
    typedef size_t cid_t;


    inline ElementCanvas() {
    }



    inline ~ElementCanvas() {
    }





    /**
    * @return ���������� ������������� ��������.
    */
    virtual cid_t cid() const = 0;

};


}
