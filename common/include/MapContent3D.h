#pragma once

#include "type.h"
#include <memory>


namespace siu {
    namespace common {


/* - Нет, потому что шаблон.
class MapContent3D;
typedef std::shared_ptr< MapContent3D >  MapContent3DPtr;
*/


/**
* Карта содержания. 3D. Базовый класс.
*
* @template S* Кол-во ячеек по сторонам параллелипипеда.
*
*
* Термины:
*    - тринум - часть ячейки, разбитой на равные части 3-мя плоскостями (3^3 =
*      27 частей). Лат., tria (3) + planum (плоскость) = три[пла]нум.
*    - нонум - часть ячейки, разбитой на равные части 9-тью плоскостями (9^3 =
*      729 частей). Лат., novem (9) + planum (плоскость) = но[вопла]нум.
* @see Числа по латыни > http://www.liveinternet.ru/users/2752294/post114441737
*
*
* Принцип нумерациии частей 3D-ячейки (тринумов), разбитой 3-мя плоскостями.
* Смотрим по оси OY на куб 3x3x3. Ось Z направлена вверх.
* Снимаем средний (центральный) 2D слой - номеруем его согласно isc2D().
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
template< typename T, size_t SX, size_t SY, size_t SZ >
class MapContent3D {
public:

    inline MapContent3D() {
        static_assert( ((SX % 2) == 1), "Длина стороны по X должна быть нечётной." );
        static_assert( ((SY % 2) == 1), "Длина стороны по Y должна быть нечётной." );
        static_assert( ((SZ % 2) == 1), "Длина стороны по Z должна быть нечётной." );
    }



    inline virtual ~MapContent3D() {
    }




    /**
    * @return Размер карты содержания (3D, длины сторон).
    */
    static inline psizeInt_t size() {
        return psizeInt_t( static_cast< int >( SX ),  static_cast< int >( SY ),  static_cast< int >( SZ ) );
    }



    /**
    * @return Все стороны карты равны.
    */
    static inline bool cube() {
        return ( (SX == SY) && (SX == SZ) );
    }



    /**
    * @return Количество ячеек по каждой из сторон карты содержания.
    */
    static inline size_t sizeCube() {
        static_assert( (SX == SY) && (SX == SZ), "Метод применим только для кубических карт." );
        return SX;
    }



    /**
    * @return Объём карты. Он же - размер в 1D.
    */
    static inline size_t volume() {
        return (SX * SY * SZ);
    }



    /**
    * @return Минимальная и максимальная координаты на карте.
    */
    static inline coordInt_t minCoord() {
        return -maxCoord();
    }

    static inline coordInt_t maxCoord() {
        return ( (size() - 1) / 2 ).toCoordInt();
    }




    /**
    * @return Координата лежит на карте.
    *
    * @alias !outside()
    */
    static inline bool inside( const coordInt_t& c ) {
        //return inside( ic( c ) );
        return (c >= minCoord()) && (c <= maxCoord());
    }

    static inline bool inside( int x, int y, int z ) {
        //return inside( ic( x, y, z ) );
        return inside( coordInt_t( x, y, z ) );
    }

    static inline bool inside( size_t i ) {
        return (i < volume());
    }




    /**
    * @return Координата лежит за пределами карты.
    *
    * @alias !inside()
    */
    static inline bool outside( const coordInt_t& c ) {
        const coordInt_t mi = minCoord();
        const coordInt_t ma = maxCoord();
        return ( (c.x < mi.x) || (c.x > ma.x) )
            || ( (c.y < mi.y) || (c.y > ma.y) )
            || ( (c.z < mi.z) || (c.z > ma.z) )
        ;
    }

    static inline bool outside( int x, int y, int z ) {
        return outside( coordInt_t( x, y, z ) );
    }

    /* - Метод не точен в прикладных задачах.
    static inline bool outside( size_t i ) {
        return (i >= volume());
    }
    */



    /**
    * @return Первый не пустой элемент карты.
    *         (!) 1D-координата в формате хранения 'raw'.
    *
    * @see raw, next(), ic(), ci()
    */
    virtual size_t first() const = 0;



    /**
    * @return Следующий непустой элемент карты по 1D-координате или
    *         0, если след. непустого элемента на карте нет.
    *         (!) 1D-координата в формате хранения 'raw'.
    *
    * @see first(), ic(), ci()
    */
    virtual size_t next( size_t i ) const = 0;




    /**
    * @return Содежание с указанной стороны ячейки не пустое.
    *
    * Если соседняя ячейка выходит за пределы 3D-матрицы (ячейка лежит на
    * границе), возвращает 'false'.
    *
    * @param с Рассматриваемая ячейка.
    * @param k Номер соседней ячейки. Нумерация сторон - см. прим. к классу.
    */
    inline bool has( const coordInt_t& с, size_t k ) const {
        assert( inside( c ) && "Координата лежит за пределами карты." );
        const coordInt_t coordCell = с + isc( k );
        return inside( coordCell ) && test( coordCell );
    }



    /**
    * @return Содержание со всех 6-ти сторон ячейки - не пустое.
    *         Проверяются ячейки 1, 2, 3, 4, 9, 18 - см. прим. к классу.
    *
    * @param c Рассматриваемая ячейка.
    */
    inline bool has6( const coordInt_t& c ) const {
        return
            // верхняя
            has( c, 1 )
            // правая
         && has( c, 2 )
            // нижняя
         && has( c, 3 )
            // левая
         && has( c, 4 )
            // ближняя
         && has( c, 9 )
            // дальняя
         && has( c, 18 );
    }





    /**
    * @param i Одномерная координата.
    *
    * @see ic()
    */
    virtual void set( size_t i, const T value ) = 0;


    virtual void set( const T value ) = 0;


    inline void set( const coordInt_t& c, const T value ) {
        set( ic( c ),  value );
    }


    inline bool set( int x, int y, int z, const T value ) {
        return set( ic( x, y, z ),  value );
    }



    /**
    * @param i Одномерная координата.
    *
    * @see ic()
    */
    void reset( size_t i ) {
        set( i, T() );
    }


    inline void reset( const coordInt_t& c ) {
        reset( ic( c ) );
    }


    inline bool reset( int x, int y, int z ) {
        return reset( ic( x, y, z ) );
    }


    inline void reset() {
        set( T() );
    }




    
    /**
    * @return Содержимое указанной ячейки - не пустое.
    */
    virtual bool test( size_t i ) const = 0;

    virtual bool test( const coordInt_t& c ) const = 0;

    virtual bool test( int x, int y, int z ) const = 0;




    /**
    * @return Количество непустых данных.
    */
    virtual size_t count() const = 0;



    /**
    * @return Карта не содержит данных (все ячейки - пустые).
    */
    inline bool empty() const {
        return (count() == 0);
    }




    /**
    * @return 3D-координата, переведёная в 1D.
    */
    static inline size_t ic( const coordInt_t& c ) {
        return ic( c.x, c.y, c.z );
    }

    static inline size_t ic( int x, int y, int z ) {
        /* - Проверка мешает оптимизации.
             Пример: IzoOgre3DVisual::operator<<( Picture )
        assert( inside( x, y, z ) && "Координата лежит за пределами карты." );
        */
        const size_t i = (
            static_cast< size_t >(x + maxCoord().x)
          + static_cast< size_t >(y + maxCoord().y) * SX
          + static_cast< size_t >(z + maxCoord().z) * SX * SY
        );
        return i;
    }



    /**
    * @return Трёхмерная координата по одномерной.
    *         Результат лежит в диапазоне [ minCoord(); maxCoord() ].
    */
    static inline coordInt_t ci( size_t i ) {
        assert( inside( i ) && "Координата 'i' лежит за пределами карты." );
        // @todo optimize Брать координаты из готовой таблицы.
        const size_t z = i / (SX * SY);
        const size_t kz = z * SX * SY;
        const size_t y = (i - kz) / SX;
        const size_t x = i - y * SX - kz;

        const coordInt_t c(
            static_cast< int >( x ) - maxCoord().x,
            static_cast< int >( y ) - maxCoord().y,
            static_cast< int >( z ) - maxCoord().z
        );
        assert( inside( c ) && "Вычисленная 3D координата лежит за пределами карты." );

        return c;
    }



    /**
    * @return Вектор смещения в сторону указанной ячейки.
    *         Если превышены границы мира, возвращает (0; 0).
    *
    * @see Нумерацию в прим. к классу.
    */
    static inline coordInt_t isc( size_t cell ) {
        const static coordInt_t a[27] = {
            // центральный 2D слой: 0-8
            coordInt_t(  0,  0,  0 ),
            coordInt_t(  0,  0, +1 ),
            coordInt_t( +1,  0,  0 ),
            coordInt_t(  0,  0, -1 ),
            coordInt_t( -1,  0,  0 ),
            coordInt_t( +1,  0, +1 ),
            coordInt_t( +1,  0, -1 ),
            coordInt_t( -1,  0, -1 ),
            coordInt_t( -1,  0, +1 ),
            // ближний 2D слой: 9-17
            coordInt_t(  0, -1,  0 ),
            coordInt_t(  0, -1, +1 ),
            coordInt_t( +1, -1,  0 ),
            coordInt_t(  0, -1, -1 ),
            coordInt_t( -1, -1,  0 ),
            coordInt_t( +1, -1, +1 ),
            coordInt_t( +1, -1, -1 ),
            coordInt_t( -1, -1, -1 ),
            coordInt_t( -1, -1, +1 ),
            // дальний 2D слой: 18-26
            coordInt_t(  0, +1,  0 ),
            coordInt_t(  0, +1, +1 ),
            coordInt_t( +1, +1,  0 ),
            coordInt_t(  0, +1, -1 ),
            coordInt_t( -1, +1,  0 ),
            coordInt_t( +1, +1, +1 ),
            coordInt_t( +1, +1, -1 ),
            coordInt_t( -1, +1, -1 ),
            coordInt_t( -1, +1, +1 )
        };

        return a[ cell ];
    }


};



    } // common

} // siu
