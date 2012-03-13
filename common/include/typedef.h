#pragma once

#include <bm.h>


namespace siu {



/**
* UID слоя на холсте.
*/
typedef std::string  uidLayer_t;


/**
* Топология для обмена битовыми картами между объектами.
*/
typedef bm::bvector<>  topology_t;


/**
* Декларация слоя.
*/
typedef std::pair< uidLayer_t, topology_t >  layer_t;










// Методы ниже помогают построить объекты по шаблонам.


/**
* Возведение целого числа в степень двойки.
* Спасает при создании шаблонов, которые надо параметризировать, вычислив
* возведение в степень.
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
