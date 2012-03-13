#pragma once

#include "typedef.h"
#include <boost/unordered_map.hpp>
#include "Sketch.h"


namespace siu {


/**
* Холст. Представлен 3D-кубом.
* холст заполняется элементами эскиза, причём 
* Используется художником для рисования Картины.
*
* @template HCeil Верхний горизонт, охватываемый холстом.
* @template HFloor Нижний горизонт, охватываемый холстом.
* @template K Коэффициент. Во сколько раз каждый вышестоящий горизонт больше
*           нижестоящего. Должен быть нечётным.
* @template HObservation Наблюдаемый горизонт.
* 
* Разница между верхним и нижним горизонтами определяет глубину холста.
* Координаты элементов на холсте задаются целыми числами.
* Допустимые координаты на холсте лежат в диапазоне diapason().
*
* Пример.
* Размер холста: [ f( HCeil ); f( HFloor ) ]
* [ ~ "пояс Койпера" (3^27 ~ 7.6 млрд. км);  ~ "орбита Меркурия" (3^22 ~ 31.4 млн. км) ]
* Наблюдение за элементами размером с "сегмент" (3^13 ~ 1600 км) и больше.
* Наблюдаемые элементы (HObservation), размером от "сегмента" до "орбиты
* меркурия" располагаются на холсте отдельно и без дополнительной обработки
* (растрирования), т.к. их размер меньше ячейки дна (HFloor) этого холста.
* Элементы, попав благодаря HObservation на холст, степень детализации
* которого меньше их размеров, визуализируются точками.
*
* @see Sketch
*/
template< int HCeil, int HFloor, int HObservation, size_t K = 3 >
class Canvas {
public:
    /**
    * Cодержимое холста - элементы, помещённые на холст.
    */
    struct Content {
        // Эскизы и элементы эскизов, которые сформировали это содержимое.
        // Включены сюда, т.к. холст может изменять эскиз.
        std::vector< Sketch >  s;
        std::vector< ElementSketch >  es;

        // Элемент холста - результат слияния элементов эскиза
        ElementCanvas  ec;

        // Карта, определяющая заполнение ячеек холста элементом холста
        // @todo ...

        BitContent(
            const std::vector< Sketch >&  s,
            const std::vector< ElementSketch >&  es,
            const ElementCanvas&  ec
        ) : es( es ), ec( ec ) {
            assert( ( !s.empty() && !es.empty() )
                && "Содержимое холста должно формироваться на основании эскизов и их элементов." );
        }

        /**
        * @alias ec.cid()
        */
        inline ElementCanvas::cid_t cid() const {
            return ec.cid();
        }

        bool operator==( const Content& c ) const {
            return ( cid() == c.cid() );
        }

        size_t hash_value( const Content& c ) const {
            return cid();
        }
    };



public:
    inline Canvas() {
        static_assert( ((K % 2) == 1),
            "Коэффициент холста должен быть нечётным." );
        static_assert( (depth() <= 19),
            "Глубина холста не может превышать 19 (защищаемся от переполнения разрядности при K == 3)." );
        // Если хотим создать динамическую систему, глубина холста не должна
        // превышать 5, т.к. на ПК 2012 г. обрабатывать 3^5^3 = 243^3 =
        // = 14 348 907 ячеек - довольно большая нагрузка.
        // При глубине = 6, кол-во ячеек = 3^6^3 = 729^3 = 387 420 489.
    }



    virtual inline ~Canvas() {
    }



    /**
    * @return Глубина холста.
    */
    static inline size_t depth() {
        return static_cast< size_t >( std::abs( HCeil - HFloor ) );
    }



    /**
    * @return Кол-во ячеек (делений) в одном измерении.
    */
    static inline size_t n() {
        return static_cast< size_t >( powN< K, depth() >().value );
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
    * @return Заданная координата лежит внутри холста.
    */
    static inline bool inside( int x, int y, int z ) {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return Заданная координата лежит за пределами холста.
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




    inline Canvas& operator<<( const Content&  yet ) {
        auto ftr = mContent.find( yet );
        if (ftr == mContent.cend()) {
            // cid() ещё нет в содержании
            mContent.insert( yet );
        } else {
            // Уже есть запись с тем же cid()
            // @todo ftr->bm. ...
        }
        return *this;
    }






protected:
    /**
    * Содержимое (топология) холста.
    * Представляет собой сгруппированные по cid() элементы холста с
    * зажжёнными битами-ячейками, где располагаются эти элементы.
    */
    boost::unordered_set< Content >  mContent;

};

}
