#pragma once

#include "typedef.h"
#include <boost/unordered_set.hpp>
#include "Sketch.h"
#include "ElementCanvas.h"


namespace siu {


/**
* Холст. Представлен 3D-кубом.
* Используется художником для рисования Картины.
* Холст заполняется элементами эскиза, причём элементы эскиза *преобразуются*
* в элементы холста.
*
* @template K Коэффициент. Во сколько раз каждый вышестоящий горизонт больше
*           нижестоящего. Должен быть нечётным.
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
template< size_t K = 3 >
class Canvas {
public:
    /**
    * Cодержимое холста - элементы, помещённые на холст.
    */
    struct Content {
        // Эскизы и элементы эскизов, которые сформировали это содержимое.
        // Включены сюда, т.к. холст может изменять эскиз.
        std::vector< Sketch< K > >  s;
        std::vector< std::shared_ptr< ElementSketch > >  es;

        // Элемент холста - результат слияния элементов эскиза
        std::shared_ptr< ElementCanvas >  ec;

        // Карта, определяющая заполнение ячеек холста элементом холста
        // @todo ...

        Content(
            const std::vector< Sketch< K > >&  s,
            const std::vector< std::shared_ptr< ElementSketch > >&  es,
            const std::shared_ptr< ElementCanvas >  ec
        ) : s( s ), es( es ), ec( ec ) {
            assert( ( !s.empty() && !es.empty() )
                && "Содержимое холста должно формироваться на основании эскизов и их элементов." );
            assert( ec );
        }

        /**
        * @alias ec.cid()
        */
        inline ElementCanvas::cid_t cid() const {
            return ec->cid();
        }

        bool operator==( const Content& content ) const {
            return ( cid() == content.cid() );
        }

        size_t hash_value() const {
            return cid();
        }
    };



public:
    /*
    * @param hCeil Верхний горизонт, охватываемый холстом.
    * @param hFloor Нижний горизонт, охватываемый холстом.
    * @param HObservation Наблюдаемый горизонт.
    * @param c Координата холста относительно родительского горизонта.
    *        Родительский горизонт указан внутри 'c'.
    */
    inline Canvas(
        int hCeil, int hFloor, int hObservation,
        const RelativeCoord< K >& c
    ) : hCeil( hCeil ), hFloor( hFloor ), hObservation( hObservation ), c( c ) {
        static_assert( ((K % 2) == 1),
            "Коэффициент холста должен быть нечётным." );
        assert( (hCeil >= hFloor)
            && "Верхний горизонт должен быть больше или равен нижнему." );
        assert( (hObservation <= hFloor)
            && "Горизонт наблюдения выше нижнего горизонта." );
        assert( ((hCeil - hFloor) <= 19)
            && "Глубина холста не может превышать 19 (защищаемся от переполнения разрядности при K == 3)." );
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
    inline size_t depth() const {
        return static_cast< size_t >( hCeil - hFloor );
    }



    /**
    * @return Кол-во ячеек (целочисленных делений) в одном измерении.
    */
    inline size_t n() const {
        return static_cast< size_t >(
            std::pow( static_cast< double >( K ),  static_cast< int >( depth() ) )
        );
    }



    /**
    * Минимальное допустимое значение целочисленной координаты (включительно).
    */
    inline int min() const {
        return -max();
    }



    /**
    * Максимальное допустимое значение целочисленной координаты (включительно).
    */
    inline int max() const {
        return (static_cast< int >( n() ) - 1) / 2;
    }



    /**
    * @return Заданная целочисленная координата лежит внутри холста.
    */
    inline bool inside( int x, int y, int z ) const {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return Заданная целочисленная координата лежит за пределами холста.
    */
    inline bool outside( int x, int y, int z ) const {
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
    inline size_t ic( int x, int y, int z ) const {
        const int a = static_cast< int >( n() );
        // @todo optimize Можно без прибавления max()?
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
    inline void ci( int* coord, size_t i ) const {
        const size_t a = n();
        const size_t aa = a * a;
        const size_t z = i / aa;
        const size_t kz = i - z * aa;
        const size_t y = kz / a;
        const size_t x = i - y * a - kz;
        // @todo optimize Можно без вычитания max()?
        coord[0] = (int)x - max();
        coord[1] = (int)y - max();
        coord[2] = (int)z - max();
    }




    /**
    * Добавляет эскиз к холсту.
    */
    inline Canvas& operator<<( const Sketch< K >&  sketch ) {
        // @todo optimize? Полагаем, что эскиз не содержит элементов,
        // размер которых превышает размер самого эскиза. Поэтому
        // можем исключить эскиз, если он или его часть не попадают
        // на холст.
        
        // Элементы эскиза преобразуются в элементы холста

        // Определяем коробку видимого холста (см. assert и todo в цикле ниже)
        const double sizeCeilC = std::pow(
            static_cast< double >( K ),  static_cast< int >( hCeil )
        );
        const double boxCeilMinC[] = {
            c.x - sizeCeilC / 2.0,
            c.y - sizeCeilC / 2.0,
            c.z - sizeCeilC / 2.0
        };
        const double boxCeilMaxC[] = {
            c.x + sizeCeilC / 2.0,
            c.y + sizeCeilC / 2.0,
            c.z + sizeCeilC / 2.0
        };

        // Определяем коробку для отсечения не наблюдаемых в холсте
        // (малых по размеру) уровней
        // @todo ...

        for (auto ctr = sketch.content().cbegin(); ctr != sketch.content().cend(); ++ctr) {
            // Попадает ли элемент / часть элемента эскиза на холст? Для этого
            // сравним координаты и размер элемента эскиза с позицией и
            // размером холста.
            const auto& coordES = ctr->c;
            const auto& sizeES = ctr->es->psize();
            const double boxMinES[] = {
                coordES.x - sizeES.get<0>() / 2.0,
                coordES.y - sizeES.get<1>() / 2.0,
                coordES.z - sizeES.get<2>() / 2.0
            };
            const double boxMaxES[] = {
                coordES.x + sizeES.get<0>() / 2.0,
                coordES.y + sizeES.get<1>() / 2.0,
                coordES.z + sizeES.get<2>() / 2.0
            };

            // @todo Позволить работать с разными горизонтами.
            assert( (coordES.hCeil == c.hCeil) && "Горизонты холста и эскиза не совпадают. @todo" );

            // если горизонты совпадают, сравнение будет простым
            const auto intersectBox =
                [ &boxCeilMinC, &boxCeilMaxC, &boxMinES, &boxMaxES ] () -> bool {
                    return (boxCeilMinC[0] <= boxMaxES[0])
                        && (boxCeilMaxC[0] >= boxMinES[0])
                        && (boxCeilMinC[1] <= boxMaxES[1])
                        && (boxCeilMaxC[1] >= boxMinES[1])
                        && (boxCeilMinC[2] <= boxMaxES[2])
                        && (boxCeilMaxC[2] >= boxMinES[2]);
                };
            if ( !intersectBox() ) {
                // элемент эскиза на холст точно не попадает
                continue;
            }

            // элемент или его часть, возможно, попадут на холст
            // Добавляем элемент на холст
            place( ctr->es, coordES );
            
        } // for (auto ctr = sketch.content().cbegin(); ctr != sketch.content().cend(); ++ctr)


        // Добавляем эскиз
        // Content&  yet;
        // @todo ...

        /*
        // Добавляем содержание
        auto ftr = mContent.find( yet );
        if (ftr == mContent.cend()) {
            // cid() ещё нет в содержании
            mContent.insert( yet );
        } else {
            // Уже есть запись с тем же cid()
            // @todo ftr->bm. ...
        }
        */

        return *this;
    }






protected:
    /**
    * Преобразует элемент эскиза в элемент, подходящий для включения в холст.
    * Добавляет полученный элемент на холст, вычислив положение элемента
    * внутри холста.
    */
    inline void place(
        const std::shared_ptr< ElementSketch >  esPlace,
        const RelativeCoord< K >&  cPlace
    ) {
        // Координаты центра элемента внутри холста
        //@todo ...

        // @test
        const auto& t1 = typeid( PhysicsSolidES ).name();
        const auto& t2 = typeid( *esPlace ).name();

        if (typeid( EllipsoidES ) == typeid( *esPlace )) {
            auto ellipsoidES = static_cast< EllipsoidES* >( esPlace.get() );
            // @todo ...

        } else {
            // @todo ...
        }

        // @todo ...

    }






protected:
    /**
    * Горизонты холста.
    */
    const int hCeil;
    const int hFloor;
    const int hObservation;

    /**
    * Координата холста.
    */
    const RelativeCoord< K >  c;


    /**
    * Содержимое (топология) холста.
    * Представляет собой сгруппированные по cid() элементы холста с
    * зажжёнными битами-ячейками, где располагаются эти элементы.
    */
    boost::unordered_set< Content >  mContent;

};

}
