namespace siu {


template< size_t SX, size_t SY, size_t SZ >
inline ScaleBitMap< SX, SY, SZ >::ScaleBitMap(
) :
    mShape(),
    bm_()
{
}




template< size_t SX, size_t SY, size_t SZ >
inline ScaleBitMap< SX, SY, SZ >::~ScaleBitMap() {
}





template< size_t SX, size_t SY, size_t SZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t const&  ScaleBitMap< SX, SY, SZ >::draw() const {

    bm_.reset();

    // ћ-биткарта может быть инициирована разными источниками
    assert( mShape
        && "ћ-биткарта не имеет источников. ƒл€ задани€ источников следует использовать operator<<()." );

    const Shaper< SX, SY, SZ >  shaper( mShape );
    bm_ = shaper.draw();

    return bm_;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<(
    const typename shape::Shape< OSX, OSY, OSZ >& source
) {
    // @todo ѕозволить передавать формы с "отличающейс€" сеткой.
    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        " арта высот должна быть разбита на то же кол-во €чеек, что м-биткарта." );

    mShape = source.clone();

    return *this;
}



} // siu
