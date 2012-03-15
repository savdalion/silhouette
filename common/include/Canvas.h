#pragma once

#include "configure.h"
#include <boost/assign/list_of.hpp>
#include <boost/unordered_set.hpp>
#include "Sketch.h"
#include "ElementCanvas.h"
#include "BitContent3D.h"
#include "SolidEC.h"


namespace siu {


/**
* Холст. Представлен 3D-кубом.
* Используется художником для рисования Картины.
* Холст заполняется элементами эскиза, причём элементы эскиза *преобразуются*
* в элементы холста.
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
class Canvas {
public:
    /**
    * Cодержимое холста - элементы, помещённые на холст.
    */
    struct Content {
        /* - @todo Добавить.
        // Эскизы и элементы эскизов, которые сформировали это содержимое.
        // Включены сюда, т.к. холст может изменять эскиз. Наличие связи
        // позволяет провести изменения и в родителях содержимого холста.
        std::vector< Sketch >  s;
        std::vector< std::shared_ptr< ElementSketch > >  es;
        */

        // Элемент холста - результат слияния элементов эскиза
        std::shared_ptr< ElementCanvas >  ec;

        // Карта, определяющая заполнение ячеек холста элементом холста
        std::shared_ptr< BitContent3D >  bm;

        Content(
            //const std::vector< Sketch >&  s,
            const std::vector< std::shared_ptr< ElementSketch > >&  es,
            const std::shared_ptr< ElementCanvas >  ec,
            const std::shared_ptr< BitContent3D >  bm
        ) : ec( ec ), bm( bm ) {
            /*
            assert( ( !s.empty() && !es.empty() )
                && "Содержимое холста должно формироваться на основании эскизов и их элементов." );
            */
            assert( ec );
        }

        /**
        * @alias ec.cid()
        */
        inline ElementCanvas::cid_t cid() const {
            return ec->cid();
        }

        inline bool operator==( const Content& b ) const {
            return ( cid() == b.cid() );
        }

        /* - Вынесена в namespace boost.
        inline size_t hash_value() const {
            return cid();
        }
        */
    };



    /**
    * Содержимое (топология) холста.
    * Представляет собой сгруппированные по cid() элементы холста с
    * зажжёнными битами-ячейками, где располагаются эти элементы.
    */
    typedef boost::unordered_set< Content >  topology_t;



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
        const RelativeCoord& c
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




    inline boost::unordered_set< Content > const&  content() const {
        return mContent;
    }



    /**
    * @return Реальный размер в метрах, который охватывает холст.
    */
    inline double realSize() const {
        return std::pow(
            static_cast< double >( K ),  static_cast< int >( hCeil )
        );
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
    * Заполняет в 'coord[3]' трёхмерную координату по одномерной.
    * Результат лежит в диапазоне [ min(); max() ].
    */
    inline void ci( int* coord, size_t i ) const {
        const size_t a = n();
        const size_t aa = a * a;
        const size_t z = i / aa;
        const size_t kz = z * aa;
        const size_t y = (i - kz) / a;
        const size_t x = i - y * a - kz;
        // @todo optimize Можно без вычитания max()?
        coord[0] = static_cast< int >( x ) - max();
        coord[1] = static_cast< int >( y ) - max();
        coord[2] = static_cast< int >( z ) - max();
    }




    /**
    * Добавляет эскиз к холсту.
    */
    inline Canvas& operator<<( const Sketch& sketch ) {
        // @todo optimize? Полагаем, что эскиз не содержит элементов,
        // размер которых превышает размер самого эскиза. Поэтому
        // можем исключить эскиз, если он или его часть не попадают
        // на холст.
        
        // Элементы эскиза преобразуются в элементы холста

        // Определяем коробку видимого холста (см. assert и todo в цикле ниже)
        const double realSizeC = realSize();
        const double boxCeilMinC[] = {
            c.x - realSizeC / 2.0,
            c.y - realSizeC / 2.0,
            c.z - realSizeC / 2.0
        };
        const double boxCeilMaxC[] = {
            c.x + realSizeC / 2.0,
            c.y + realSizeC / 2.0,
            c.z + realSizeC / 2.0
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
            place( sketch, ctr->es, coordES );
            
        } // for (auto ctr = sketch.content().cbegin(); ctr != sketch.content().cend(); ++ctr)

        return *this;
    }






protected:
    /**
    * Преобразует элемент эскиза в элемент, подходящий для включения в холст.
    * Добавляет полученный элемент на холст, вычислив положение элемента
    * внутри холста.
    */
    inline void place(
        const Sketch& sketch,
        const std::shared_ptr< ElementSketch >  esPlace,
        const RelativeCoord& cPlace
    ) {
        // Строим битовую форму фигуры, совмещая её образ с холстом
        const double rs = realSize();
        const size_t N = n();
        const BitContent3D b = esPlace->form(
            // реальный размер холста, м
            rs,
            // кол-во неделимых ячеек, на которые разбит холст
            N,
            // координата центра холста
            c,
            // координата, куда следует поместить элемент холста
            cPlace
        );

        // Формируем новое содержание
        //const std::vector< Sketch >  s = boost::assign::list_of( sketch );
        const std::vector< std::shared_ptr< ElementSketch > >  es =
            boost::assign::list_of( esPlace );
        // Элемент холста - результат слияния элементов эскиза
        const auto ec =
            std::shared_ptr< ElementCanvas >( new SolidEC( esPlace->matter ) );
        // Карта, определяющая заполнение ячеек холста элементом холста
        const auto bm =
            std::shared_ptr< BitContent3D >( new BitContent3D( b )  );
        const Content yet( /*s,*/ es, ec, bm );

        // Добавляем содержание
        auto ftr = mContent.find( yet );
        if (ftr == mContent.cend()) {
            // cid() ещё нет в топологии:
            // добавляем содержание целиком
            mContent.insert( yet );

        } else {
            // Уже есть запись с тем же cid():
            // сливаем совпадающие содержания

            /* - @todo Ошибка.
            // @test
            for (auto itr = yet.s.begin(); itr != yet.s.end(); ++itr) {
                auto s = *itr;
                (*ftr).s.push_back( s );
            }

            //ftr->s.insert( ftr->s.end(), yet.s.begin(), yet.s.end() );
            //ftr->es.insert( ftr->es.end(), yet.es.begin(), yet.es.end() );
            *ftr->bm |= *yet.bm;
            */

            *ftr->bm |= *yet.bm;
        }

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
    const RelativeCoord  c;


    /**
    * Топология холста.
    */
    topology_t mContent;

};



/*
inline bool operator==( const siu::Canvas::Content& a, const siu::Canvas::Content& b ) {
    return ( a.cid() == b.cid() );
}
*/

}





namespace boost {

inline size_t hash_value( const siu::Canvas::Content& a ) {
    return a.cid();
}

}
