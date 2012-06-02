#pragma once

#include "MapContent3D.h"

// @see http://bmagic.sourceforge.net
#include <bm.h>



namespace siu {
    namespace common {

template< size_t Grid >  class BitMapContent3D;

    }
}



namespace std {

template< size_t Grid >
std::ostream& operator<<( std::ostream&, const siu::common::BitMapContent3D< Grid >& );

} // std




namespace siu {
    namespace common {


/**
* Содержание 3D, хранимое в виде битового образа.
* См. соглашение о '81' в configure.h.
*/
template< size_t Grid >
class BitMapContent3D : public MapContent3D< bool, Grid, Grid, Grid > {
public:
    /**
    * Сырой (одномерный) набор бит.
    */
    typedef bm::bvector<>  raw_t;
    raw_t raw;



    explicit inline BitMapContent3D( bool value = false ) {
        //raw.resize( side() );
        value ? raw.set() : raw.reset();
    }



    inline BitMapContent3D(
        const raw_t& raw
    ) :
        raw( raw )
    {
    }


        
    inline virtual ~BitMapContent3D() {
    }




    /**
    * @return Первый зажжённый бит карты. Может быть '0'.
    */
    virtual inline size_t first() const {
        return raw.get_first();
    }



    /**
    * @return Следующий зажжённый бит карты или 0, если больше битов нет.
    */
    virtual inline size_t next( size_t i ) const {
        return raw.get_next( i );
    }




    inline BitMapContent3D& operator=( bool x ) {
        raw = x;
        return *this;
    }



    inline BitMapContent3D& operator=( const BitMapContent3D& b ) {
        raw = b.raw;
        return *this;
    }




    // bitset operations
    inline BitMapContent3D& operator&=(const BitMapContent3D& b) {
        raw &= b.raw;
        return *this;
    }


    inline BitMapContent3D& operator|=(const BitMapContent3D& b) {
        raw |= b.raw;
        return *this;
    }


    inline BitMapContent3D& operator^=(const BitMapContent3D& b) {
        raw ^= b.raw;
        return *this;
    }



    inline bool operator==( const BitMapContent3D& b ) const {
        return (raw == b.raw);
    }



    inline bool operator!=( const BitMapContent3D& b ) const {
        return (raw != b.raw);
    }



    // basic bit operations
    inline void set() {
        raw.set();
    }


    virtual inline void set( size_t i, const bool value ) {
        raw[ i ] = value;
    }


    virtual inline void set( const bool value ) {
        raw.set( value );
    }


    inline void set( const coordInt_t& c ) {
        set( ic( c ), true );
    }


    inline void set( int x, int y, int z ) {
        set( ic( x, y, z ),  true );
    }




    inline void flip( const coordInt_t& c ) {
        flip( ic( c ) );
    }


    inline void flip( size_t i ) {
        raw.flip( i );
    }


    inline void flip() {
        raw.flip();
    }



    
    virtual inline bool test( size_t i ) const {
        return raw.test( i );
    }


    virtual inline bool test( const coordInt_t& c ) const {
        return test( ic( c ) );
    }

    virtual inline bool test( int x, int y, int z ) const {
        return test( ic( x, y, z ) );
    }




    inline bool any() const {
        return raw.any();
    }


    inline bool none() const {
        return raw.none();
    }


    inline BitMapContent3D operator~() const {
        BitMapContent3D b( *this );
        b.raw = ~raw;
        return b;
    }


    inline size_t count() const {
        return raw.count();
    }




    template< size_t Grid >
    friend std::ostream& std::operator<<( std::ostream&, const siu::common::BitMapContent3D< Grid >& );

};




    } // common

} // siu








template< size_t Grid >
inline std::ostream& std::operator<<(
    std::ostream& out,
    const siu::common::BitMapContent3D< Grid >& b
) {
    out << "Bit count: " << b.count() << std::endl;

    return out;
}
