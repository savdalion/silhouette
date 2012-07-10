#pragma once

#include <coord.h>
#include <mapcontent3d/BitMap.h>



namespace siu {
    namespace shape {


/**
* Функтор - для common::Shaper, базовый класс - для форм.
*
* @template SX Кол-во ячеек биткарты по оси X.
* @template SY Кол-во ячеек биткарты по оси Y.
* @template SZ Кол-во ячеек биткарты по оси Z.
*/
template< size_t SX, size_t SY, size_t SZ >
class Shape {
public:
    typedef typelib::BitMap< SX, SY, SZ >  bm_t;


public:
    inline Shape() : bm() {
        static_assert( (SX != 0), "Количество ячеек по оси X для биткарты необходимо указать." );
        static_assert( (SY != 0), "Количество ячеек по оси Y для биткарты необходимо указать." );
        static_assert( (SZ != 0), "Количество ячеек по оси Z для биткарты необходимо указать." );
    }


    virtual inline ~Shape() {
    }




    /**
    * Создаёт битовую карту.
    */
    virtual bm_t operator()(
        const typelib::coord_t& areaMin = typelib::coord_t::ZERO,
        const typelib::coord_t& areaMax = typelib::coord_t::ZERO
    ) = 0;




    /**
    * @return Максимальный размер формы.
    *
    * @see sizeGrid()
    */
    virtual typelib::coord_t sizeMax() const = 0;




    /**
    * @return Размер одной ячейки сетки. Размер выбирается т.о., чтобы фигура
    *         полностью помещалась в ячейке размером (SX, SY, SZ).
    */
    virtual inline typelib::coord_t sizeGrid() const {
        return sizeMax() / typelib::coord_t( SX, SY, SZ );
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
        return !outside( x, y, z, areaMin, areaMax );
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
