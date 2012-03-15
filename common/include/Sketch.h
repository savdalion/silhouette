#pragma once

#include "configure.h"
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
* @see Canvas
*/
class Sketch {
public:
    /**
    * Содержимое эскиза - элемент, включённый в эскиз.
    */
    struct Content {
        std::shared_ptr< ElementSketch >  es;
        RelativeCoord c;
        // @todo rotation

        Content(
            const std::shared_ptr< ElementSketch >  es,
            const RelativeCoord&  c
        ) : es( es ), c( c ) {
            assert( es && "Элемент эскиза не задан." );
        }

        inline bool operator==( const Content& b ) const {
            return (*es == *b.es) && (c == b.c);
        }

    };



public:
    /**
    * @param rc Координаты эскиза в эскизе-родителе.
    * @param hCeil Верхний горизонт, охватываемый эскизом.
    * @param hFloor Нижний горизонт, охватываемый эскизом.
    */
    inline Sketch(
        const RelativeCoord&  rcParent,
        int hCeil, int hFloor
    ) : rcParent( rcParent ), hCeil( hCeil ), hFloor( hFloor ) {
        static_assert( ((K % 2) == 1),
            "Коэффициент эскиза должен быть нечётным." );
        assert( (hCeil >= hFloor)
            && "Верхний горизонт должен быть больше или равен нижнему." );
    }



    virtual inline ~Sketch() {
    }



    /**
    * @return Глубина эскиза.
    */
    inline size_t depth() const {
        return static_cast< size_t >( hCeil - hFloor );
    }



    /**
    * @return Размер эскиза в одном измерении, м.
    */
    inline double n() const {
        return std::pow( static_cast< double >( K ),  static_cast< int >( depth() ) );
    }



    /**
    * Минимальное допустимое значения координаты (включительно).
    */
    inline double min() const {
        return -max();
    }



    /**
    * Максимальное допустимое значения координаты (включительно).
    */
    inline double max() const {
        return n() / 2.0;
    }



    /**
    * @return Заданная координата лежит внутри эскиза.
    */
    inline bool inside( double x, double y, double z ) const {
        return (
            (x >= min()) && (x <= max())
         && (y >= min()) && (y <= max())
         && (z >= min()) && (z <= max())
        );
    }



    /**
    * @return Заданная координата лежит за границами эскиза.
    */
    inline bool outside( double x, double y, double z ) const {
        return (
            (z < min()) || (z > max())
         || (y < min()) || (y > max())
         || (x < min()) || (x > max())
        );
    }




    /**
    * Добавляет элемент к эскизу.
    * Размер элемента не может превышать размер эскиза. Допустимо, чтобы
    * элемент (но не его центр!) выходил за пределы эскиза.
    *//* - Заменено на operator<<().
    inline void place( const ElementSketch& e, const RelativeCoord& c ) {
        mContent.push_back( Content( e, c ) );
    }
    */

    inline Sketch& operator<<( const Content& yet ) {
        assert( (yet.es->psize().get<0>() <= n() )
             && (yet.es->psize().get<1>() <= n() )
             && (yet.es->psize().get<2>() <= n() )
             && "Размер элемента не может превышать размер эскиза." );

        mContent.push_back( yet );

        return *this;
    }




    /**
    * @return Содержимое эскиза.
    */
    inline const std::vector< Content >&  content() const {
        return mContent;
    }


    inline std::vector< Content >&  content() {
        return mContent;
    }




    inline bool operator==( const Sketch& b ) const {
        return (hCeil == b.hCeil)
            && (hFloor == b.hFloor)
            && (rcParent == b.rcParent)
            && (mContent == b.mContent);
    }





public:
    /**
    * Горизонты холста.
    */
    const int hCeil;
    const int hFloor;

    /**
    * Координаты относительно родительского эскиза.
    */
    const RelativeCoord rcParent;



protected:
    /**
    * Содержимое эскиза.
    * Представляет собой набор элементов эскиза заданного размера, формы
    * и положения.
    */
    std::vector< Content >  mContent;

};

}
