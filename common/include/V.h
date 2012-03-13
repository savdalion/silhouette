#pragma once

#include "typedef.h"


namespace siu {


/**
* Общий класс для упрощения построения шаблона холста, эскиза.
* 
* @template H Горизонт. Какой макс. размер пространства охватывает эскиз.
* @template D Глубина. Детализация пространства.
* @template K Коэффициент. Во сколько раз вышестоящий горизонт больше
*           нижестоящего. Должен быть нечётным.
*
* H, D, K определяют "ёмкость".
*
* @see IntegerRelativeCoord
*
* V - сокр. от Volume, Visual
*/
template< int H, size_t D, size_t K = 3 >
class V {
public:
    inline V() {
        static_assert( ((K % 2) == 1), "Коэффициент должен быть нечётным." );
        assert( (D <= 19) && "Глубина не может превышать 19 (защищаемся от переполнения разрядности)." );
        // @todo fine stability Большое 'K' может вызвать переполнение и при меньших 'D'.
    }



    inline ~V() {
    }



    /**
    * @return Кол-во ячеек (делений) в одном измерении.
    */
    static inline size_t n() {
        return static_cast< size_t >( powN< K, D >().value );
    }



    /**
    * Минимальное допустимое значения координаты (включительно).
    */
    static inline int min() {
        return -max();
    }



    /**
    * Максимальное допустимое значения координаты (включительно).
    */
    static inline int max() {
        return (static_cast< int >( n() ) - 1) / 2;
    }



    /**
    * @return Заданная координата лежит внутри объёма.
    */
    static inline bool inside( int x, int y, int z ) {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return Заданная координата лежит за границами объёма.
    */
    static inline bool outside( int x, int y, int z ) {
        return (
            (z < min()) || (z > max())
         || (y < min()) || (y > max())
         || (x < min()) || (x > max())
        );
    }




    /**
    * @return Одномерная координата по трёхмерной.
    *         Лежит в диапазоне [ 0; n()*n()*n() ).
    */
    static inline size_t ic( int x, int y, int z ) {
        const int a = static_cast< int >( n() );
        const int i = (
            (x + max())
          + (y + max()) * a
          + (z + max()) * a * a
        );
        assert( ( (i >= 0) && ( i < (a*a*a) ) )
            && "Одномерная координата вне допустимого диапазона." );
        return static_cast< size_t >( i );
    }


    /**
    * Заполняет в 'coord' трёхмерную координату по одномерной.
    * Результат лежит в диапазоне [ min(); max() ].
    */
    static inline void ci( int* coord, size_t i ) {
        const size_t a = n();
        const size_t aa = a * a;
        const size_t z = i / aa;
        const size_t kz = i - z * aa;
        const size_t y = kz / a;
        const size_t x = i - y * a - kz;
        coord[0] = (int)x - max();
        coord[1] = (int)y - max();
        coord[2] = (int)z - max();
    }

};


}
