#pragma once

#include <bm.h>


namespace siu {



/**
* UID ���� �� ������.
*/
typedef std::string  uidLayer_t;


/**
* ��������� ��� ������ �������� ������� ����� ���������.
*/
typedef bm::bvector<>  topology_t;


/**
* ���������� ����.
*/
typedef std::pair< uidLayer_t, topology_t >  layer_t;










// ������ ���� �������� ��������� ������� �� ��������.


/**
* ���������� ������ ����� � ������� ������.
* ������� ��� �������� ��������, ������� ���� �����������������, ��������
* ���������� � �������.
*/
template <long num, size_t n, size_t y = 1>
struct pow2 {
    enum { value = pow2< num * num, (n >> 1), n & 1 ? num * y : y >::value };
};
template <long num, size_t y>
struct pow2< num, 0, y > {
    enum { value = y };
};


/**
* = B^N
*//*
template< int B, int N > 
struct powN { 
    enum { value = B * Pow< B, N-1 >::value };
}; 
template< int B > struct Pow< B, 0 > { 
    enum { value = 1 };
};
*/


}
