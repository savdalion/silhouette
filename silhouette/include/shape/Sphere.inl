namespace siu {
    namespace shape {


template< size_t SX, size_t SY, size_t SZ >
template< typename T, typename S >
inline Sphere< SX, SY, SZ >::Sphere(
    T r,
    bool fill,
    S scale
) :
    Ellipsoid( r, r, r, fill, scale )
{
}




template< size_t SX, size_t SY, size_t SZ >
inline Sphere< SX, SY, SZ >::~Sphere() {
}





template< size_t SX, size_t SY, size_t SZ >
inline typename Sphere< SX, SY, SZ >::bm_t Sphere< SX, SY, SZ >::operator()() {
    // @todo optimize —феру можно строить быстрее.
    return static_cast< Ellipsoid >( *this )();
}





template< size_t SX, size_t SY, size_t SZ >
inline typename Shape< SX, SY, SZ >::Ptr  Sphere< SX, SY, SZ >::clone() const {
    return Sphere< SX, SY, SZ >::Ptr( new Sphere< SX, SY, SZ >( *this ) );
}



    } // shape

} // siu
