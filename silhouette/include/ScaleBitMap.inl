namespace siu {


template< size_t SX, size_t SY, size_t SZ >
inline ScaleBitMap< SX, SY, SZ >::ScaleBitMap(
) :
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
inline typename ScaleBitMap< SX, SY, SZ >::bm_t const&  ScaleBitMap< SX, SY, SZ >::draw( int x, int y, int z ) const {
    return draw( typelib::coordInt_t( x, y, z ) );
}




template< size_t SX, size_t SY, size_t SZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t const&  ScaleBitMap< SX, SY, SZ >::draw( const typelib::coordInt_t& c ) const {

    if (c == c_) {
        // лишний раз не считаем, накладно
        return bm_;
    }

    
    bm_.reset();

    //const Shaper< SX >  shaper( &elm );
    //bm_ = shaper.draw();

    c_ = c;

    return bm_;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<( const typename shape::ElevationMap< OSX, OSY, OSZ >& source ) {

    // @todo ...

    return *this;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<( const typename ScaleBitMap< OSX, OSY, OSZ >& source ) {

    // @todo ...

    return *this;
}



} // siu
