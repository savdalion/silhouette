#pragma once

#include "RelativeCoord.h"
#include "ElementSketch.h"


namespace siu {


/**
* Эскиз.
* Схематический набросок мира или его части.
*
* @see V
*
* Координаты элементов в эскизе задаются целыми числами.
* Допустимые координаты лежат в диапазоне [ min(), max() ].
*
* @template HCeil Верхний горизонт, охватываемый эскизом.
* @template HFloor Нижний горизонт, охватываемый эскизом. Помогает определить
*           границы присутствия эскиза при добавлении эскиза на холст.
* @template K Коэффициент. Во сколько раз каждый вышестоящий горизонт больше
*           нижестоящего. Должен быть нечётным.
*
* @see Canvas
*/
template< int HCeil, int HFloor, size_t K = 3 >
class Sketch {
public:
    /**
    * Содержимое эскиза - элемент, включённый в эскиз.
    */
    struct Content {
        ElementSketch es;
        RelativeCoord c;
        // @todo rotation

        Content(
            const ElementSketch& es,
            const RelativeCoord& c
        ) : es( es ), c( c ) {
        }
    };



public:
    /**
    * @param Координаты эскиза в эскизе-родителе.
    */
    inline Sketch( const RelativeCoord& rc ) {
        static_assert( ((K % 2) == 1), "Коэффициент эскиза должен быть нечётным." );
    }



    virtual inline ~Sketch() {
    }



    /**
    * @return Размер эскиза в одном измерении, м.
    */
    static inline double n() {
        const int D = HCeil - HFloor;
        return std::pow( static_cast< double >( K ), D );
    }



    /**
    * Минимальное допустимое значения координаты (включительно).
    */
    static inline double min() {
        return -max();
    }



    /**
    * Максимальное допустимое значения координаты (включительно).
    */
    static inline double max() {
        return n() / 2.0;
    }



    /**
    * @return Заданная координата лежит внутри эскиза.
    */
    static inline bool inside( double x, double y, double z ) {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return Заданная координата лежит за границами эскиза.
    */
    static inline bool outside( double x, double y, double z ) {
        return (
            (z < min()) || (z > max())
         || (y < min()) || (y > max())
         || (x < min()) || (x > max())
        );
    }




    /**
    * Добавляет элемент к эскизу.
    *//* - Заменено на operator<<().
    inline void place( const ElementSketch& e, const RelativeCoord& c ) {
        mContent.push_back( Content( e, c ) );
    }
    */


    inline Sketch& operator<<( const Content& yet ) {
        mContent.push_back( yet );
        return *this;
    }




    /**
    * @return Содержимое эскиза.
    */
    inline std::vector< Content >&  content() const {
        return mContent;
    }


    inline std::vector< Content >&  content() {
        return mContent;
    }






protected:
    /**
    * Содержимое эскиза.
    * Представляет собой набор элементов эскиза заданного размера, формы
    * и положения.
    */
    std::vector< Content >  mContent;

};

}








/**
* Добавляет к эскизу элемент.
*//* - @todo Реализовать operator<< вместо place().
template< int HCeil, int HFloor, size_t K = 3 >
inline siu::Sketch&  operator<<( siu::Sketch< HCeil, HFloor, K >& s, const siu::Sketch::Content& yet ) {
    s.content().push_back( yet );
}
*/
