#pragma once

// �������� ������ � WinDef.h
#define NOMINMAX

#pragma warning( push, 2 )
#include <assert.h>
#include <string>
#include <limits>
#pragma warning( pop )

#pragma warning( disable: 4514 )
#pragma warning( disable: 4201 )


/**
* ������ ���� ������.
*/


namespace siu {
    namespace common {


/**
* �������� ��������� ����� � ��������� ������.
*/
const float PRECISION = 0.001f;



/**
* @return true, ���� �������� ������.
*/
static inline bool empty( int v ) {
    return (v == 0);
}


static inline bool empty( float v ) {
    return (std::abs( v ) < PRECISION);
}


static inline bool empty( double v ) {
    return (std::abs( v ) < PRECISION);
}


static inline bool empty( bool v ) {
    return v;
}




/**
* @return true, ���� �������� �����.
*/
static inline bool equal( int a, int b ) {
    return (a == b);
}


static inline bool equal( float a, float b ) {
    return (std::abs( a - b ) < PRECISION);
}



struct coord_t;
struct coordInt_t;



/**
* ���������� � 3D.
*
* @todo optimize? ������������� � ����������� ��������� (union-������ + ������. ����������)?
*/
struct coord_t {
    float x;
    float y;
    float z;

    inline coord_t() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {
    }

    inline coord_t(
        float x, float y, float z
    ) : x( x ), y( y ), z( z )
    {
    }

    /**
    * @return �����.
    */
    inline bool center() const {
        return empty( x ) && empty ( y ) && empty( z );
    }

    inline coordInt_t toInteger() const;

    inline coord_t operator-() const {
        return coord_t( -x, -y, -z );
    }

    inline coord_t operator+( const coord_t& c ) const {
        return coord_t( x + c.x,  y + c.y,  z + c.z );
    }

    inline coord_t operator-( const coord_t& c ) const {
        return coord_t( x - c.x,  y - c.y,  z - c.z );
    }

    inline coord_t operator*( const coord_t& c ) const {
        return coord_t( x * c.x,  y * c.y,  z * c.z );
    }

    inline coord_t operator/( const coord_t& c ) const {
        return coord_t( x / c.x,  y / c.y,  z / c.z );
    }

    inline coord_t operator*( int k ) const {
        return coord_t(
            x * static_cast< float >( k ),
            y * static_cast< float >( k ),
            z * static_cast< float >( k )
        );
    }

    inline coord_t operator*( float k ) const {
        return coord_t( x * k,  y * k,  z * k );
    }

    inline coord_t operator/( int k ) const {
        return coord_t(
            x / static_cast< float >( k ),
            y / static_cast< float >( k ),
            z / static_cast< float >( k )
        );
    }

    inline coord_t operator/( float k ) const {
        return coord_t( x / k,  y / k,  z / k );
    }

    // @todo fine ��������� ��� real ������ �������. PRECISION.
    inline bool operator>( const coord_t& c ) const {
        return (x > c.x) && (y > c.y) && (z > c.z);
    }

    inline bool operator>=( const coord_t& c ) const {
        return (x >= c.x) && (y >= c.y) && (z >= c.z);
    }

    inline bool operator<( const coord_t& c ) const {
        return (x < c.x) && (y < c.y) && (z < c.z);
    }

    inline bool operator<=( const coord_t& c ) const {
        return (x <= c.x) && (y <= c.y) && (z <= c.z);
    }
};


struct coordInt_t {
    int x;
    int y;
    int z;

    inline coordInt_t() : x( 0 ), y( 0 ), z( 0 ) {
    }

    inline coordInt_t(
        int x, int y, int z
    ) : x( x ), y( y ), z( z )
    {
    }

    /**
    * @return �����.
    */
    inline bool center() const {
        return (x == 0) && (y == 0) && (z == 0);
    }

    inline coord_t toReal() const {
        return coord_t(
            static_cast< float >( x ),
            static_cast< float >( y ),
            static_cast< float >( z )
        );
    }

    inline coordInt_t operator-() const {
        return coordInt_t( -x, -y, -z );
    }

    inline coordInt_t operator+( const coordInt_t& c ) const {
        return coordInt_t( x + c.x,  y + c.y,  z + c.z );
    }

    inline coordInt_t operator-( const coordInt_t& c ) const {
        return coordInt_t( x - c.x,  y - c.y,  z - c.z );
    }

    inline coordInt_t operator*( int k ) const {
        return coordInt_t( x * k,  y * k,  z * k );
    }

    inline coord_t operator*( float k ) const {
        return coord_t(
            static_cast< float >( x ) * k
          , static_cast< float >( y ) * k
          , static_cast< float >( z ) * k
       );
    }

    inline coordInt_t operator/( int k ) const {
        return coordInt_t( x / k,  y / k,  z / k );
    }

    inline coord_t operator/( float k ) const {
        return coord_t(
            static_cast< float >( x ) / k
          , static_cast< float >( y ) / k
          , static_cast< float >( z ) / k
       );
    }

    inline bool operator>( const coordInt_t& c ) const {
        return (x > c.x) && (y > c.y) && (z > c.z);
    }

    inline bool operator>=( const coordInt_t& c ) const {
        return (x >= c.x) && (y >= c.y) && (z >= c.z);
    }

    inline bool operator<( const coordInt_t& c ) const {
        return (x < c.x) && (y < c.y) && (z < c.z);
    }

    inline bool operator<=( const coordInt_t& c ) const {
        return (x <= c.x) && (y <= c.y) && (z <= c.z);
    }

};





coordInt_t coord_t::toInteger() const {
    return coordInt_t(
        static_cast< int >( x + ((x < 0.0f) ? -0.001f : 0.001f) ),
        static_cast< int >( y + ((y < 0.0f) ? -0.001f : 0.001f) ),
        static_cast< int >( z + ((z < 0.0f) ? -0.001f : 0.001f) )
    );
}





/**
* ���������� � 3D ������������ ��������� ������ / ������.
*/
struct relCoordInt_t {
    const std::string n;
    const coordInt_t c;

    inline relCoordInt_t() : n( "" ),  c() {
    }

    inline relCoordInt_t(
        const std::string& relativeName,
        const coordInt_t& relativeCoord
    ) : n( relativeName ),  c( relativeCoord )
    {
        assert( ( n.empty() && c.center() )
            && "���������� ���������� (������ 'relativeName') � ���� ������� ����� ���� ������������ ������ ��� coordInt_t(0; 0; 0)." );
    }

    inline relCoordInt_t(
        const std::string& relativeName,
        int x, int y, int z
    ) : n( relativeName ),  c( x, y, z )
    {
        assert( ( n.empty() && c.center() )
            && "���������� ���������� (������ 'relativeName') � ���� ������� ����� ���� ������������ ������ ��� (0; 0; 0)." );
    }

    /*
    // �������� ������� �� ������� "���������������"
    // ������ ����� ���������� (0; 0; 0)
    inline relCoordInt_t(
    ) : n( "" ),  c( 0, 0, 0 )
    {
    }
    */

    /**
    * @return ������� ���������� ��������� ��������.
    */
    inline bool root() const {
        assert( ( n.empty() && c.center() )
            && "���������� ���������� (������ 'n') � ���� ������� ����� ���� ������������ ������ ��� (0; 0; 0)." );
        return n.empty();
    }

    inline relCoordInt_t& operator=( const relCoordInt_t& b ) {
        const_cast< std::string& >( n ) = b.n;
        const_cast< coordInt_t& >( c ) = b.c;
    }

};





/**
* ������������� ��������.
* ���������� ������������ � ������ ������ ��������.
*
* ����. �� Name ID.
*/
typedef std::string  nid_t;




/**
* ����.
* ����������� � ��������� ISO 639-1.
*/
typedef std::string  lang_t;




/**
* ������, �������������� � ���� ������ � ������� JSON.
*/
typedef std::string  jsonString_t;



    
/**
* ���������� ������� ��������.
* � ��� ��������� ������ ��� �������� ������� �������� ���
* ����������� ����. ������ ���� ����������, ����� �������
* (�� ����� ��������) �� �����.
* �������� �������� �� ��������� ������������ ��������������.
*/
#if 0
// - ���������� ��� ������ ������. ��. ����.
struct psize_t {
    /**
    * ������� ��� ���������� ��������.
    */
    typedef struct {
    } nonexistent_t;

    /**
    * ������� ������� �� X, Y � Z.
    */
    typedef struct {
        float sx;
        float sy;
        float sz;
    } box_t;

    /**
    * ������ ����.
    */
    typedef struct {
        float s;
    } cube_t;

    /**
    * ������� X, Y � Z ��� ����������.
    */
    typedef struct {
        float rx;
        float ry;
        float rz;
    } ellipsoid_t;

    /**
    * ������ ��� �����.
    */
    typedef struct {
        float r;
    } sphere_t;

    // @todo extend ������� ��� ��������� ���������, ��������, ������ � ��. �����.

    /**
    * ������ ������.
    */
    union allSizeFigure_t {
        box_t box;
        cube_t cube;
        ellipsoid_t ellipsoid;
        sphere_t sphere;

        inline allSizeFigure_t() {
            box.sx = box.sy = box.sz = std::numeric_limits< float >::infinity();
        }

        inline allSizeFigure_t( float a ) {
            box.sx = box.sy = box.sz = a;
            // cube.s = a;
            // sphere.r = a;
        }

        inline allSizeFigure_t( float a ) {
            box.sx = box.sy = box.sz = a;
        }
    };

    allSizeFigure_t figure;


    inline psize_t(
    ) : figure()
    {
    }

}; // struct psize_t
#endif

struct psize_t {
    /**
    * ��� ������� ��������� ������ � �������.
    * ���, �������, �����, ��������� - ��� ��� ���� �����
    * ���������� 3 ��������.
    *
    * @todo extend ������� ��� ��������� ���������, ��������, ������ � ��. �����.
    */
    struct { float sx, sy, sz; };

    explicit inline psize_t( float a = 0.0f, float b = 0.0f, float c = 0.0f ) {
        sx = a;
        sy = b;
        sz = c;
    }

    inline bool anyEmpty() const {
        return (
            ::siu::common::empty( sx )
         || ::siu::common::empty( sy )
         || ::siu::common::empty( sz )
        );
    }

    inline bool empty() const {
        return (
            ::siu::common::empty( sx )
         && ::siu::common::empty( sy )
         && ::siu::common::empty( sz )
        );
    }

    /**
    * @return ������ ��� �������� (������� ����) �� ������ � ������.
    */
    inline bool cube() const {
        return (
            !anyEmpty()
         && equal(sx, sy) && equal(sx, sz)
        );
    }    

    inline psize_t operator+( const psize_t& ps ) const {
        return psize_t( sx + ps.sx,  sy + ps.sy,  sz + ps.sz );
    }

    inline psize_t operator+( int k ) const {
        const float t = static_cast< float >( k );
        return psize_t( sx + t,  sy + t,  sz + t );
    }

    inline psize_t operator-( const psize_t& ps ) const {
        return psize_t( sx - ps.sx,  sy - ps.sy,  sz - ps.sz );
    }

    inline psize_t operator-( int k ) const {
        const float t = static_cast< float >( k );
        return psize_t( sx - t,  sy - t,  sz - t );
    }

    inline psize_t operator*( int k ) const {
        const float t = static_cast< float >( k );
        return psize_t( sx * t,  sy * t,  sz * t );
    }

    inline psize_t operator*( float k ) const {
        return psize_t( sx * k, sy * k, sz * k );
    }

    inline psize_t operator/( int k ) const {
        const float t = static_cast< float >( k );
        return psize_t( sx / t, sy / t, sz / t );
    }

    inline psize_t operator/( float k ) const {
        return psize_t( sx / k, sy / k, sz / k );
    }

    inline coord_t toCoord() const {
        return coord_t( sx, sy, sz );
    }

    static const psize_t ZERO;
};





struct psizeInt_t {
    // ������ ��� int, ����� �� �������������� � ����������� �����
    // ��� ������ ��������� � �������������� ������������
    struct { int sx, sy, sz; };

    explicit inline psizeInt_t( int a = 0, int b = 0, int c = 0 ) {
        sx = a;
        sy = b;
        sz = c;
    }

    explicit inline psizeInt_t( const psize_t& ps ) {
        sx = static_cast< int >( ps.sx );
        sy = static_cast< int >( ps.sy );
        sz = static_cast< int >( ps.sz );
    }

    inline bool anyEmpty() const {
        return ( (sx == 0) || (sy == 0) || (sz == 0) );
    }

    inline bool empty() const {
        return ( (sx == 0) && (sy == 0) && (sz == 0) );
    }

    /**
    * @return ������ ��� ������� �� ������ � ������.
    */
    inline bool cube() const {
        return (
            !anyEmpty()
         && equal(sx, sy) && equal(sx, sz)
        );
    }    

    inline psizeInt_t operator+( const psizeInt_t& ps ) const {
        return psizeInt_t( sx + ps.sx,  sy + ps.sy,  sz + ps.sz );
    }

    inline psizeInt_t operator+( int k ) const {
        return psizeInt_t( sx + k,  sy + k,  sz + k );
    }

    inline psizeInt_t operator-( const psizeInt_t& ps ) const {
        return psizeInt_t( sx - ps.sx,  sy - ps.sy,  sz - ps.sz );
    }

    inline psizeInt_t operator-( int k ) const {
        return psizeInt_t( sx - k,  sy - k,  sz - k );
    }

    inline psizeInt_t operator*( int k ) const {
        return psizeInt_t( sx * k,  sy * k,  sz * k );
    }

    inline psize_t operator*( float k ) const {
        return psize_t(
            static_cast< float >( sx ) * k
          , static_cast< float >( sy ) * k
          , static_cast< float >( sz ) * k
        );
    }

    inline psizeInt_t operator/( int k ) const {
        return psizeInt_t(
            sx / k
          , sy / k
          , sz / k
        );
    }

    inline psize_t operator/( float k ) const {
        return psize_t(
            static_cast< float >( sx ) / k
          , static_cast< float >( sy ) / k
          , static_cast< float >( sz ) / k
        );
    }

    inline coordInt_t toCoordInt() const {
        return coordInt_t( sx, sy, sz );
    }

    static const psizeInt_t ZERO;
};



    } // common

} // siu






namespace std {

std::ostream& operator<<( std::ostream&, const siu::common::coord_t& );

std::ostream& operator<<( std::ostream&, const siu::common::coordInt_t& );


std::ostream& operator<<( std::ostream&, const siu::common::relCoordInt_t& );


std::ostream& operator<<( std::ostream&, const siu::common::psize_t& );

std::ostream& operator<<( std::ostream&, const siu::common::psizeInt_t& );

} // std



#pragma warning( default: 4514 )
#pragma warning( default: 4201 )
