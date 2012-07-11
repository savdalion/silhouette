namespace siu {


template< size_t SX, size_t SY, size_t SZ >
inline ScaleBitMap< SX, SY, SZ >::ScaleBitMap(
) :
    mShape(),
    mScaleBitMap(),
    bm_(),
    // задаём заведомо неправильные координаты, чтобы первый вызов draw()
    // всегда считал новую биткарту
    c_( bm_t::undefinedCoord() )
{
}




template< size_t SX, size_t SY, size_t SZ >
inline ScaleBitMap< SX, SY, SZ >::~ScaleBitMap() {
}




template< size_t SX, size_t SY, size_t SZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t const&  ScaleBitMap< SX, SY, SZ >::draw() const {
    return draw( typelib::coordInt_t::ZERO() );
}




template< size_t SX, size_t SY, size_t SZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t const&  ScaleBitMap< SX, SY, SZ >::draw(
    int x, int y, int z
) const {
    return draw( typelib::coordInt_t( x, y, z ) );
}




template< size_t SX, size_t SY, size_t SZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t const&  ScaleBitMap< SX, SY, SZ >::draw(
    const typelib::coordInt_t& c
) const {

    if (c == c_) {
        // лишний раз не считаем, накладно
        return bm_;
    }

    
    bm_.reset();

    // М-биткарта может быть инициирована разными источниками
    if ( mShape ) {
        const auto elevationMap =
            std::static_pointer_cast< shape::ElevationMap< SX, SY, SZ > >( mShape );
        bm_ = drawShape< SX, SY, SZ >( elevationMap );

    } else if ( mScaleBitMap ) {
        const auto sbm =
            std::static_pointer_cast< ScaleBitMap< SX, SY, SZ > >( mScaleBitMap );
        bm_ = drawParent< SX, SY, SZ >( sbm, c );

    } else {
        assert( false
            && "М-биткарта не имеет источников. Для задания источников следует использовать operator<<()." );
    }

    c_ = c;

    return bm_;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<(
    const typename shape::ElevationMap< OSX, OSY, OSZ >& source
) {
    // @todo Позволить передавать карту высот с "отличающейся" разметкой.
    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "Карта высот должна быть разбита на то же кол-во ячеек, что м-биткарта." );

    mShape = shape_t( new shape::ElevationMap< OSX, OSY, OSZ >( source ) );

    return *this;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<(
    const typename ScaleBitMap< OSX, OSY, OSZ >& source
) {
    // @todo Позволить передавать м-биткарту с "отличающейся" разметкой.
    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "Масштабируемая биткарта-источник должна быть разбита на то же кол-во ячеек, что эта м-биткарта." );

    mScaleBitMap = scaleBitMap_t( new ScaleBitMap< OSX, OSY, OSZ >( source ) );

    return *this;
}







template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t  ScaleBitMap< SX, SY, SZ >::drawShape(
    const typename shape::ElevationMap< OSX, OSY, OSZ >::Ptr& elevationMap
) const {

    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "Карта высот должна быть разбита на то же кол-во ячеек, что м-биткарта." );

    // форма-источник всегда рисуется в масштабе 1:1
    const Shaper< SX, SY, SZ >  shaper( elevationMap );

    return shaper.draw();
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t  ScaleBitMap< SX, SY, SZ >::drawParent(
    const typename ScaleBitMap< OSX, OSY, OSZ >::Ptr& sbm,
    const typelib::coordInt_t& c
) const {

    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "М-биткарта-источник должна быть разбита на то же кол-во ячеек, что эта м-биткарта." );

    // смотрим источник полученной биткарты
    const auto sh = sbm->shape();
    assert( sh && "М-биткарта должна обладать источником в виде формы." );

    // рисуем часть биткарты

    /* - @todo Нельзя. Слишком большой размер.
    // для этого сфорируем биткарту с учётом размера родителя и выделим из неё
    // заказанную ячейку
    const auto shapeWithParent = sh->scale< OSX*SX, OSY*SY, OSZ*SZ >();
    const Shaper< OSX*SX, OSY*SY, OSZ*SZ >  shaper( shapeWithParent );

    // рисуется одна ячейка
    return shaper.draw( с, c );
    */


    // рисуется одна ячейка в большем масштабе
    const Shaper< SX, SY, SZ >  shaper( sh );

    return shaper.draw( c, OSX, OSY, OSZ );
}




} // siu
