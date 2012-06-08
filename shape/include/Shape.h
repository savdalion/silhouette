#pragma once

#include <coord-type.h>
#include <BitMap.h>



namespace siu {
    namespace shape {


/**
* Функтор - для common::Shaper, базовый класс - для форм.
*
* @template Grid Кол-во ячеек битовой карты.
*/
template< size_t Grid >
class Shape {
public:
    typedef common::BitMap< Grid >  bm_t;


public:
    inline Shape() : bm() {
        static_assert( (Grid != 0), "Количество ячеек для битовой карты необходимо указать." );
    }


    virtual inline ~Shape() {
    }




    /**
    * Создаёт битовую карту.
    *
    * @param np 
    */
    virtual bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t::ZERO,
        const typelib::coord_t& areaMax = typelib::coord_t::ZERO
    ) = 0;




    /**
    * @return Максимальный размер формы по одной из коорд. осей.
    *
    * @see sizeGrid()
    */
    virtual float sizeMax() const = 0;




    /**
    * @return Размер одной ячейки сетки. Размер выбирается т.о., чтобы фигура
    *         полностью помещалась в кубе со стороной Grid.
    */
    virtual inline float sizeGrid() const {
        return sizeMax() / static_cast< float >( Grid );
    }



    /**
    * @return Заданная кордината лежит в указанных границах сетки фигуры.
    *
    * @see outside()
    */
    inline bool inside(
        int x, int y, int z,
        const typelib::coord_t& areaMin,
        const typelib::coord_t& areaMax
    ) {
        return !outside( x, y, z, araeMin, areaMax );
    }





    /**
    * @return Заданная кордината лежит за границами сетки фигуры.
    */
    inline bool outside(
        // координаты ячейки
        int x, int y, int z,
        // контрольная область (попадание = в пределах границы)
        const typelib::coord_t& areaMin,
        const typelib::coord_t& areaMax
    ) {
        assert( !empty(areaMin - areaMax)
            && "Контрольная область не может быть пустой." );
        assert( (areaMin < areaMax)
            && "Контрольная область задаётся меньшей и большей координатами коробки. Именно в таком порядке." );

        const float sizeG = sizeGrid();
        const float wx = static_cast< float >( x ) * sizeG;
        const float wy = static_cast< float >( y ) * sizeG;
        const float wz = static_cast< float >( z ) * sizeG;
        return ( (wx < areaMin.x) || (wx > areaMax.x) )
            || ( (wy < areaMin.y) || (wy > areaMax.y) )
            || ( (wz < areaMin.z) || (wz > areaMax.z) )
        ;
    }





protected:
    /**
    * Сформированная битовая карта.
    */
    bm_t bm;

};


    } // shape

} // siu
