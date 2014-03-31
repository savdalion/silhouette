#pragma once

#include "typedef.h"


namespace siu {

    
/**
* Координаты, представленные в виде целых чисел.
* Введены для позициционирования элемента на холсте.
*
* @template K Коэффициент. Во сколько раз вышестоящий горизонт больше
*           нижестоящего. Должен быть нечётным.
*
* @see IntegerRelativeCoord
*/
template< size_t K = 3 >
class IntegerRelativeCoord {
public:
    inline IntegerRelativeCoord(
        int horizontCeil, int horizontFloor,
        int x, int y, int z
    ) :
        mHorizontCeil( horizontCeil ), mHorizontFloor( horizontFloor ),
        mX( x ), mY( y ), mZ( z )
    {
        static_assert( ((K % 2) == 1), "Коэффициент должен быть нечётным." );
        assert( ((horizontCeil - horizontFloor) <= 19)
            && "Глубина не может превышать 19 (защищаемся от переполнения разрядности)." );
        assert( inside() && "Координата вне допустимого диапазона." );
    }



    inline ~IntegerRelativeCoord() {
    }



    inline int horizontCeil() const {
        return mHorizontCeil;
    }



    inline void horizontCeil( int hc ) {
        mHorizontCeil = hc;
    }



    inline int horizontFloor() const {
        return mHorizontFloor;
    }



    inline void horizontFloor( int hf ) {
        mHorizontFloor = hf;
    }



    /**
    * @return Глубина координаты.
    */
    inline size_t depth() const {
        return (mHorizontCeil - mHorizontFloor);
    }



    static inline size_t k() {
        return K;
    }







    inline int x() const {
        return mX;
    }



    inline void x( int cx ) {
        mX = cx;
    }



    inline int y() const {
        return mY;
    }



    inline void y( int cy ) {
        mY = cy;
    }



    inline int z() const {
        return mZ;
    }



    inline void z( int cz ) {
        mZ = cz;
    }



    /**
    * @return Кол-во ячеек (делений) в одном измерении.
    */
    inline size_t n() const {
        return static_cast< size_t >(
            (mHorizontCeil > 0) ? std::pow( (float)K, mHorizontCeil )
          : (mHorizontCeil == 0) ? 1
          : std::pow( (float)K, -mHorizontCeil )
        );
    }



    /**
    * Быстрое - по сравнению с чистым n() - вычисление.
    *
    * @see n() без шаблона
    *//* - Используем V<>::n().
    template< size_t D >
    static inline size_t n() {
        return static_cast< size_t >( powN< K, D >().value );
    }
    */



    /**
    * @return Минимальное допустимое значения координаты (включительно).
    */
    inline int min() const {
        return -max();
    }



    /**
    * Максимальное допустимое значения координаты (включительно).
    */
    inline int max() const {
        // n() - всегда нечётное благодаря 'K', см. конструктор
        return (n() - 1) / 2;
    }



    /**
    * @return Заданная координата - внутри объёма.
    */
    inline bool inside() const {
        return (
            (x() >= min()) && (x() <= max())
         && (y() >= min()) && (y() <= max())
         && (z() >= min()) && (z() <= max())
        );
    }



    /**
    * @return Заданная координата - за пределами эскиза.
    */
    inline bool outside() const {
        return (
            (z() < min()) || (z() > max())
         || (y() < min()) || (y() > max())
         || (x() < min()) || (x() > max())
        );
    }




    /**
    * @return Одномерная координата.
    *         Лежит в диапазоне [ 0; n()*n()*n() ).
    */
    inline size_t ic() {
        const int a = static_cast< int >( n() );
        const int i = (
            (x() + max())
          + (y() + max()) * a
          + (z() + max()) * a * a
        );
        assert( ( (i >= 0) && ( i < (a*a*a) ) )
            && "Одномерная координата вне допустимого диапазона." );
        return static_cast< size_t >( i );
    }


    /**
    * Заполняет 'coord[3]' трёхмерной координатой по заданной одномерной.
    * Результат лежит в диапазоне [ min(); max() ].
    */
    inline void ci( int* coord, size_t i ) {
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





protected:
    /**
    * Горизонт: потолок (небо) и пол (дно).
    */
    int mHorizontCeil;
    int mHorizontFloor;

    /**
    * Целочисленная 3D-координата.
    */
    int mX;
    int mY;
    int mZ;
};


}
