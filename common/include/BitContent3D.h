#pragma once

// @see http://bmagic.sourceforge.net
#include <bm.h>

#include "default.h"


namespace siu {


/**
* Содержание 3D, хранимое в виде битового образа.
*/
struct BitContent3D {

    /**
    * Сырой (одномерный) набор бит.
    */
    typedef bm::bvector<>  raw_t;
    raw_t raw;

    /**
    * Размер содержания - куб со стороной N.
    */
    const size_t N;

    /**
    * Вспомогательные константы.
    */
    const size_t N2;
    const size_t N3;



    inline BitContent3D(
        size_t N
    ) :
        N( N ),  N2( N * N ),  N3( N * N * N )
    {
        assert( N != 0 );
        raw.resize( N3 );
        raw.reset();
    }



    inline BitContent3D(
        const raw_t& raw
    ) :
        N( static_cast< size_t >( std::pow( raw.size(), 1.0 / 3.0 ) + 0.01 ) ),
        N2( N * N ),
        N3( N * N * N ),
        raw( raw )
    {
        assert( N != 0 );
    }



    virtual inline ~BitContent3D() {
    }



    /**
    * @return Заданная целочисленная координата лежит внутри содержания.
    */
    inline bool inside( const d::coordInt3d_t& c ) const {
        return inside( c.get<0>(), c.get<1>(), c.get<2>() );
    }

    inline bool inside( int x, int y, int z ) const {
        return (
            (x >= 0) && (x < static_cast< int >( N ))
         && (y >= 0) && (y < static_cast< int >( N ))
         && (z >= 0) && (z < static_cast< int >( N ))
        );
    }



    /**
    * @return Заданная целочисленная координата лежит за пределами содержания.
    */
    inline bool outside( const d::coordInt3d_t& c ) const {
        return outside( c.get<0>(), c.get<1>(), c.get<2>() );
    }

    inline bool outside( int x, int y, int z ) const {
        return (
            (z < 0) || (z >= static_cast< int >( N ))
         || (y < 0) || (y >= static_cast< int >( N ))
         || (x < 0) || (x >= static_cast< int >( N ))
        );
    }




    /**
    * @return Бит с указанной стороны ячейки установлен.
    *
    * Если соседняя ячейка выходит за пределы 3D-матрицы (ячейка лежит на
    * границе), возвращает 'false'.
    *
    * @param n Рассматриваемая ячейка.
    * @param k Номер соседней ячейки. Нумерация сторон - см. helper::isc3D().
    */
    inline bool has( const d::coordInt3d_t& n, size_t k ) const {
        const d::coordInt3d_t coordCell = n + isc3D( k );
        return inside( coordCell ) && test( coordCell );
    }



    /**
    * @return Бит со всех 6-ти сторон ячейки установлен.
    *         Проверяются ячейки 1, 2, 3, 4, 9, 18 - см. helper::isc3D().
    *
    * @param n Рассматриваемая ячейка.
    */
    inline bool has6( const d::coordInt3d_t& n ) const {
        return
            // верхняя
            has( n, 1 )
            // правая
         && has( n, 2 )
            // нижняя
         && has( n, 3 )
            // левая
         && has( n, 4 )
            // ближняя
         && has( n, 9 )
            // дальняя
         && has( n, 18 );
    }




    inline BitContent3D& operator=( bool x ) {
        raw = x;
        return *this;
    }



    inline BitContent3D& operator=( const BitContent3D& b ) {
        raw = b.raw;
        return *this;
    }




    // bitset operations
    inline BitContent3D& operator&=(const BitContent3D& b) {
        raw &= b.raw;
        return *this;
    }


    inline BitContent3D& operator|=(const BitContent3D& b) {
        raw |= b.raw;
        return *this;
    }


    inline BitContent3D& operator^=(const BitContent3D& b) {
        raw ^= b.raw;
        return *this;
    }



    inline bool operator==( const BitContent3D& b ) {
        return (raw == b.raw);
    }



    inline bool operator!=( const BitContent3D& b ) {
        return (raw != b.raw);
    }



    // basic bit operations
    inline BitContent3D& set( const d::coordInt3d_t& n, bool val = true) {
        set( ic( n ),  val );
        return *this;
    }


    inline BitContent3D& set( int x, int y, int z, bool val = true) {
        set( ic( x, y, z ),  val );
        return *this;
    }


    inline BitContent3D& set( size_t i, bool val = true) {
        raw[ i ] = val;
        return *this;
    }


    inline BitContent3D& set() {
        raw.set();
        return *this;
    }


    inline BitContent3D& reset( const d::coordInt3d_t& n ) {
        reset( ic( n ) );
        return *this;
    }


    inline BitContent3D& reset( size_t i ) {
        raw[ i ] = false;
        return *this;
    }


    inline BitContent3D& reset() {
        raw.reset();
        return *this;
    }


    inline BitContent3D& flip( const d::coordInt3d_t& n ) {
        flip( ic( n ) );
        return *this;
    }


    inline BitContent3D& flip( size_t i ) {
        raw.flip( i );
        return *this;
    }


    inline BitContent3D& flip() {
        raw.flip();
        return *this;
    }



    
    inline bool test( const d::coordInt3d_t& n ) const {
        return test( ic( n ) );
    }

    inline bool test( size_t x, size_t y, size_t z ) const {
        return test( ic( x, y, z ) );
    }

    // Работает быстрее test( const d::coordInt3d_t& )
    inline bool test( size_t i ) const {
        return raw.test( i );
    }





    inline bool any() const {
        return raw.any();
    }


    inline bool none() const {
        return raw.none();
    }


    inline BitContent3D operator~() const {
        BitContent3D b( *this );
        b.raw = ~raw;
        return b;
    }


    inline size_t count() const {
        return raw.count();
    }



    inline bool empty() const {
        return (raw.count() == 0);
    }




    /**
    * @return 3D-координата, переведёная в 1D.
    */
    inline size_t ic( const d::coordInt3d_t& c ) const {
        return ic(
            static_cast< size_t >( c.get<0>() ),
            static_cast< size_t >( c.get<1>() ),
            static_cast< size_t >( c.get<2>() )
        );
    }

    inline size_t ic( size_t x, size_t y, size_t z ) const {
        assert( x < N );
        assert( y < N );
        assert( z < N );
        return (x + y * N + z * N2);
    }



    /**
    * Заполняет в 'coord[3]' трёхмерную координату по одномерной.
    * Результат лежит в диапазоне [ 0; N ).
    */
    inline void ci( size_t* coord, size_t i ) const {
        // @todo Брать координаты по готовой таблице.
        const size_t z = i / N2;
        assert( z < N );
        const size_t kz = z * N2;
        const size_t y = (i - kz) / N;
        assert( y < N );
        const size_t x = i - y * N - kz;
        assert( x < N );
        coord[0] = x;
        coord[1] = y;
        coord[2] = z;
    }



    /**
    * @return Вектор смещения в сторону указанной ячейки.
    *         Если превышены границы мира, возвращает (0; 0).
    *
    * Для 3D-пространства.
    * Принцип нумерациии ячеек. Смотрим по оси OY на куб 3x3x3. Ось Z направлена
    * вверх. Снимаем средний (центральный) 2D слой - номеруем его согласно isc2D().
    * Снимаем ближний - номеруем также. Снимаем дальний слой - аналогично.
    *
      Нумерация, вертикальный разрез:

        дальний 2D слой
        26   19   23
        22   18   20
        25   21   24

        центральный 2D слой
        8   1   5
        4   0   2
        7   3   6

        ближний 2D слой
        17   10   14
        13    9   11
        16   12   15
    *
    */
    static inline d::coordInt3d_t isc3D( size_t cell ) {
        const static d::coordInt3d_t a[27] = {
            // центральный 2D слой: 0-8
            d::coordInt3d_t(  0,  0,  0 ),
            d::coordInt3d_t(  0,  0, +1 ),
            d::coordInt3d_t( +1,  0,  0 ),
            d::coordInt3d_t(  0,  0, -1 ),
            d::coordInt3d_t( -1,  0,  0 ),
            d::coordInt3d_t( +1,  0, +1 ),
            d::coordInt3d_t( +1,  0, -1 ),
            d::coordInt3d_t( -1,  0, -1 ),
            d::coordInt3d_t( -1,  0, +1 ),
            // ближний 2D слой: 9-17
            d::coordInt3d_t(  0, -1,  0 ),
            d::coordInt3d_t(  0, -1, +1 ),
            d::coordInt3d_t( +1, -1,  0 ),
            d::coordInt3d_t(  0, -1, -1 ),
            d::coordInt3d_t( -1, -1,  0 ),
            d::coordInt3d_t( +1, -1, +1 ),
            d::coordInt3d_t( +1, -1, -1 ),
            d::coordInt3d_t( -1, -1, -1 ),
            d::coordInt3d_t( -1, -1, +1 ),
            // дальний 2D слой: 18-26
            d::coordInt3d_t(  0, +1,  0 ),
            d::coordInt3d_t(  0, +1, +1 ),
            d::coordInt3d_t( +1, +1,  0 ),
            d::coordInt3d_t(  0, +1, -1 ),
            d::coordInt3d_t( -1, +1,  0 ),
            d::coordInt3d_t( +1, +1, +1 ),
            d::coordInt3d_t( +1, +1, -1 ),
            d::coordInt3d_t( -1, +1, -1 ),
            d::coordInt3d_t( -1, +1, +1 )
        };

        return a[ cell ];
    }


};


}
