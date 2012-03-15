#pragma once

// �������� ������ � WinDef.h
#define NOMINMAX

#include <iostream>
#include <limits>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>


/*
* ����������� ��� ����� ������� ����, ���������, ������, ������.
*/
namespace d {


/**
* �������� ��������� ����� � ��������� ������.
*/
const float PRECISION = 0.001f;




/**
* ������������� ��������.
* ������� CouchDB.
*/
typedef std::string uid_t;
typedef std::string rev_t;
typedef std::pair< uid_t, rev_t >  externKey_t;



/**
* ��������� ����������.
* ���� �������, ����� ���������� �������� ������ �������. ��
* ����� ����������� � �������� ����������� (��������, ������
* ������ ����� �������� � ������), ����� ��������� �� �������.
*/
typedef boost::tuple< float, float >         coord2d_t;
typedef boost::tuple< int, int >             coordInt2d_t;
typedef boost::tuple< float, float, float >  coord3d_t;
typedef boost::tuple< int, int, int >        coordInt3d_t;

/**
* �������� �� �����������.
* �� ����� ���. ����� boost::numeric::interval<>, �.�. �� �� �������� �
* ���������� ������.
*/
typedef boost::tuple< coord2d_t, coord2d_t >        interval2d_t;
typedef boost::tuple< coordInt2d_t, coordInt2d_t >  intervalInt2d_t;

/**
* �������. ������ ���������� - ���� � ������������ ������������.
*/
typedef std::pair< coord2d_t, coord2d_t >        box2d_t;
typedef std::pair< coordInt2d_t, coordInt2d_t >  boxInt2d_t;
typedef std::pair< coord3d_t, coord3d_t >        box3d_t;
typedef std::pair< coordInt3d_t, coordInt3d_t >  boxInt3d_t;


/**
* ������: ����� (��� X), ������ (��� Z).
*/
typedef boost::tuple< float, float >            size2d_t;
typedef boost::tuple< size_t, size_t >          sizeInt2d_t;
typedef boost::tuple< float, float, float >     size3d_t;
typedef boost::tuple< size_t, size_t, size_t >  sizeInt3d_t;

/**
* ������.
* ����� ��� �������� ���������� �������, �������� �����������.
*/
typedef boost::tuple< float, float >         vector2d_t;
typedef boost::tuple< float, float, float >  vector3d_t;

/**
* ������� �� ������������ ����.
*/
typedef boost::tuple< float, float >  scale2d_t;
typedef boost::tuple< float, float >  scale3d_t;





/**
* ��������� ��������������� ���� �������.
* (!) ����� ������������ �� ����� ��������, ��� ��������
* ������� ��������������.
*/
const coord2d_t ZERO2D( 0.0f, 0.0f );
const coordInt2d_t ZEROINT2D( 0, 0 );
const coord3d_t ZERO3D( 0.0f, 0.0f, 1.0f );
const coordInt3d_t ZEROINT3D( 0, 0, 0 );
const coord2d_t ONE2D ( 1.0f, 1.0f );
const coord2d_t UNDEFINED2D ( std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() );
const coordInt2d_t ONEINT2D ( 1, 1 );
const coordInt2d_t UNDEFINEDINT2D ( INT_MAX, INT_MAX );
const coord3d_t ONE3D ( 1.0f, 1.0f, 1.0f );
const coord3d_t UNDEFINED3D ( std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() );
const coordInt3d_t ONEINT3D ( 1, 1, 1 );
const coordInt3d_t UNDEFINEDINT3D ( INT_MAX, INT_MAX, INT_MAX );



} // namespace d











/* * 2D operations * */

inline d::coord2d_t operator-( const d::coord2d_t& a ) {
    return d::coord2d_t( -a.get<0>(), -a.get<1>() );
}


inline d::coord2d_t operator-( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return d::coord2d_t(
        a.get<0>() - b.get<0>(),
        a.get<1>() - b.get<1>()
    );
}


inline d::coord2d_t operator-=( d::coord2d_t& a, const d::coord2d_t& b ) {
    a.get<0>() -= b.get<0>();
    a.get<1>() -= b.get<1>();
    return a;
}


inline d::coord2d_t operator+( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return d::coord2d_t(
        a.get<0>() + b.get<0>(),
        a.get<1>() + b.get<1>()
    );
}


inline d::coord2d_t operator+=( d::coord2d_t& a, const d::coord2d_t& b ) {
    a.get<0>() += b.get<0>();
    a.get<1>() += b.get<1>();
    return a;
}


inline d::coord2d_t operator*( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return d::coord2d_t(
        a.get<0>() * b.get<0>(),
        a.get<1>() * b.get<1>()
    );
}


inline d::coord2d_t operator*=( d::coord2d_t& a, const d::coord2d_t& b ) {
    a.get<0>() *= b.get<0>();
    a.get<1>() *= b.get<1>();
    return a;
}


inline d::coord2d_t operator*( const d::coord2d_t& a, float k ) {
    return d::coord2d_t(
        a.get<0>() * k,
        a.get<1>() * k
    );
}


inline d::coord2d_t operator/( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return d::coord2d_t(
        a.get<0>() / b.get<0>(),
        a.get<1>() / b.get<1>()
    );
}


inline d::coord2d_t operator/=( d::coord2d_t& a, float k ) {
    a.get<0>() /= k;
    a.get<1>() /= k;
    return a;
}


inline d::coord2d_t operator/( const d::coord2d_t& a, float k ) {
    return d::coord2d_t(
        a.get<0>() / k,
        a.get<1>() / k
    );
}


// @see d::PRECISION
inline bool operator==( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return (std::abs(a.get<0>() - b.get<0>()) < d::PRECISION)
        && (std::abs(a.get<1>() - b.get<1>()) < d::PRECISION);
}

inline bool operator!=( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return !(a == b);
}


inline bool operator>( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return (a.get<0>() > b.get<0>()) && (a.get<1>() > b.get<1>());
}


inline bool operator<( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return (a.get<0>() < b.get<0>()) && (a.get<1>() < b.get<1>());
}


inline bool operator>=( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return (a.get<0>() >= b.get<0>()) && (a.get<1>() >= b.get<1>());
}


inline bool operator<=( const d::coord2d_t& a, const d::coord2d_t& b ) {
    return (a.get<0>() <= b.get<0>()) && (a.get<1>() <= b.get<1>());
}






inline d::coordInt2d_t operator-( const d::coordInt2d_t& a ) {
    return d::coordInt2d_t( -a.get<0>(), -a.get<1>() );
}


inline d::coordInt2d_t operator-( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return d::coordInt2d_t(
        a.get<0>() - b.get<0>(),
        a.get<1>() - b.get<1>()
    );
}


inline d::coordInt2d_t operator-=( d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    a.get<0>() -= b.get<0>();
    a.get<1>() -= b.get<1>();
    return a;
}


inline d::coordInt2d_t operator+( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return d::coordInt2d_t(
        a.get<0>() + b.get<0>(),
        a.get<1>() + b.get<1>()
    );
}


inline d::coordInt2d_t operator+=( d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    a.get<0>() += b.get<0>();
    a.get<1>() += b.get<1>();
    return a;
}


inline d::coordInt2d_t operator*( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return d::coordInt2d_t(
        a.get<0>() * b.get<0>(),
        a.get<1>() * b.get<1>()
    );
}


inline d::coordInt2d_t operator*=( d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    a.get<0>() *= b.get<0>();
    a.get<1>() *= b.get<1>();
    return a;
}


inline d::coordInt2d_t operator*( const d::coordInt2d_t& a, int k ) {
    return d::coordInt2d_t(
        a.get<0>() * k,
        a.get<1>() * k
    );
}


inline d::coordInt2d_t operator/( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return d::coordInt2d_t(
        (int)(a.get<0>() / b.get<0>()),
        (int)(a.get<1>() / b.get<1>())
    );
}


inline d::coordInt2d_t operator/=( d::coordInt2d_t& a, int k ) {
    a.get<0>() = (int)(a.get<0>() / k);
    a.get<1>() = (int)(a.get<1>() / k);
    return a;
}


inline d::coordInt2d_t operator/( const d::coordInt2d_t& a, int k ) {
    return d::coordInt2d_t(
        (int)(a.get<0>() / k),
        (int)(a.get<1>() / k)
    );
}


inline d::coordInt2d_t operator%( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return d::coordInt2d_t(
        a.get<0>() % b.get<0>(),
        a.get<1>() % b.get<1>()
    );
}


inline bool operator==( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return (a.get<0>() == b.get<0>())
        && (a.get<1>() == b.get<1>());
}

inline bool operator!=( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return !(a == b);
}


inline bool operator>( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return (a.get<0>() > b.get<0>()) && (a.get<1>() > b.get<1>());
}


inline bool operator<( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return (a.get<0>() < b.get<0>()) && (a.get<1>() < b.get<1>());
}


inline bool operator>=( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return (a.get<0>() >= b.get<0>()) && (a.get<1>() >= b.get<1>());
}


inline bool operator<=( const d::coordInt2d_t& a, const d::coordInt2d_t& b ) {
    return (a.get<0>() <= b.get<0>()) && (a.get<1>() <= b.get<1>());
}






/* * 3D operations * */

// d::coord3d_t

inline d::coord3d_t operator-( const d::coord3d_t& a ) {
    return d::coord3d_t( -a.get<0>(), -a.get<1>(), -a.get<2>() );
}


inline d::coord3d_t operator-( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return d::coord3d_t(
        a.get<0>() - b.get<0>(),
        a.get<1>() - b.get<1>(),
        a.get<2>() - b.get<2>()
    );
}


inline d::coord3d_t operator-=( d::coord3d_t& a, const d::coord3d_t& b ) {
    a.get<0>() -= b.get<0>();
    a.get<1>() -= b.get<1>();
    a.get<2>() -= b.get<2>();
    return a;
}


inline d::coord3d_t operator+( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return d::coord3d_t(
        a.get<0>() + b.get<0>(),
        a.get<1>() + b.get<1>(),
        a.get<2>() + b.get<2>()
    );
}


inline d::coord3d_t operator+=( d::coord3d_t& a, const d::coord3d_t& b ) {
    a.get<0>() += b.get<0>();
    a.get<1>() += b.get<1>();
    a.get<2>() += b.get<2>();
    return a;
}


inline d::coord3d_t operator*( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return d::coord3d_t(
        a.get<0>() * b.get<0>(),
        a.get<1>() * b.get<1>(),
        a.get<2>() * b.get<2>()
    );
}


inline d::coord3d_t operator*=( d::coord3d_t& a, const d::coord3d_t& b ) {
    a.get<0>() *= b.get<0>();
    a.get<1>() *= b.get<1>();
    a.get<2>() *= b.get<2>();
    return a;
}


inline d::coord3d_t operator*( const d::coord3d_t& a, float k ) {
    return d::coord3d_t(
        a.get<0>() * k,
        a.get<1>() * k,
        a.get<2>() * k
    );
}


inline d::coord3d_t operator/( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return d::coord3d_t(
        a.get<0>() / b.get<0>(),
        a.get<1>() / b.get<1>(),
        a.get<2>() / b.get<2>()
    );
}


inline d::coord3d_t operator/=( d::coord3d_t& a, float k ) {
    a.get<0>() /= k;
    a.get<1>() /= k;
    a.get<2>() /= k;
    return a;
}


inline d::coord3d_t operator/( const d::coord3d_t& a, float k ) {
    return d::coord3d_t(
        a.get<0>() / k,
        a.get<1>() / k,
        a.get<2>() / k
    );
}


// @see d::PRECISION
inline bool operator==( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return (std::abs(a.get<0>() - b.get<0>()) < d::PRECISION)
        && (std::abs(a.get<1>() - b.get<1>()) < d::PRECISION)
        && (std::abs(a.get<2>() - b.get<2>()) < d::PRECISION);
}

inline bool operator!=( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return !(a == b);
}


inline bool operator>( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return (a.get<0>() > b.get<0>())
        && (a.get<1>() > b.get<1>())
        && (a.get<2>() > b.get<2>());
}


inline bool operator<( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return (a.get<0>() < b.get<0>())
        && (a.get<1>() < b.get<1>())
        && (a.get<2>() < b.get<2>());
}


inline bool operator>=( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return (a.get<0>() >= b.get<0>())
        && (a.get<1>() >= b.get<1>())
        && (a.get<2>() >= b.get<2>());
}


inline bool operator<=( const d::coord3d_t& a, const d::coord3d_t& b ) {
    return (a.get<0>() <= b.get<0>())
        && (a.get<1>() <= b.get<1>())
        && (a.get<2>() <= b.get<2>());
}





// d::coordInt3d_t

inline d::coordInt3d_t operator-( const d::coordInt3d_t& a ) {
    return d::coordInt3d_t( -a.get<0>(), -a.get<1>(), -a.get<2>() );
}


inline d::coordInt3d_t operator-( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return d::coordInt3d_t(
        a.get<0>() - b.get<0>(),
        a.get<1>() - b.get<1>(),
        a.get<2>() - b.get<2>()
    );
}


inline d::coordInt3d_t operator-=( d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    a.get<0>() -= b.get<0>();
    a.get<1>() -= b.get<1>();
    a.get<2>() -= b.get<2>();
    return a;
}


inline d::coordInt3d_t operator+( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return d::coordInt3d_t(
        a.get<0>() + b.get<0>(),
        a.get<1>() + b.get<1>(),
        a.get<2>() + b.get<2>()
    );
}


inline d::coordInt3d_t operator+=( d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    a.get<0>() += b.get<0>();
    a.get<1>() += b.get<1>();
    a.get<2>() += b.get<2>();
    return a;
}


inline d::coordInt3d_t operator*( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return d::coordInt3d_t(
        a.get<0>() * b.get<0>(),
        a.get<1>() * b.get<1>(),
        a.get<2>() * b.get<2>()
    );
}


inline d::coordInt3d_t operator*=( d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    a.get<0>() *= b.get<0>();
    a.get<1>() *= b.get<1>();
    a.get<2>() *= b.get<2>();
    return a;
}


inline d::coordInt3d_t operator*( const d::coordInt3d_t& a, int k ) {
    return d::coordInt3d_t(
        a.get<0>() * k,
        a.get<1>() * k,
        a.get<2>() * k
    );
}


inline d::coordInt3d_t operator/( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return d::coordInt3d_t(
        a.get<0>() / b.get<0>(),
        a.get<1>() / b.get<1>(),
        a.get<2>() / b.get<2>()
    );
}


inline d::coordInt3d_t operator/=( d::coordInt3d_t& a, int k ) {
    a.get<0>() /= k;
    a.get<1>() /= k;
    a.get<2>() /= k;
    return a;
}


inline d::coordInt3d_t operator/( const d::coordInt3d_t& a, int k ) {
    return d::coordInt3d_t(
        a.get<0>() / k,
        a.get<1>() / k,
        a.get<2>() / k
    );
}


inline d::coordInt3d_t operator%( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return d::coordInt3d_t(
        a.get<0>() % b.get<0>(),
        a.get<1>() % b.get<1>(),
        a.get<2>() % b.get<2>()
    );
}


inline d::coordInt3d_t operator%( const d::coordInt3d_t& a, int k ) {
    return d::coordInt3d_t(
        a.get<0>() % k,
        a.get<1>() % k,
        a.get<2>() % k
    );
}



inline bool operator==( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() == b.get<0>())
        && (a.get<1>() == b.get<1>())
        && (a.get<2>() == b.get<2>());
}

inline bool operator!=( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() != b.get<0>())
        || (a.get<1>() != b.get<1>())
        || (a.get<2>() != b.get<2>());
}


inline bool operator>( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() > b.get<0>()) && (a.get<1>() > b.get<1>()) && (a.get<2>() > b.get<2>());
}


inline bool operator<( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() < b.get<0>()) && (a.get<1>() < b.get<1>()) && (a.get<2>() < b.get<2>());
}


inline bool operator>=( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() >= b.get<0>()) && (a.get<1>() >= b.get<1>()) && (a.get<2>() >= b.get<2>());
}


inline bool operator<=( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() <= b.get<0>()) && (a.get<1>() <= b.get<1>()) && (a.get<2>() <= b.get<2>());
}





/* - ���. ��������� ���������� ����� � operator() ��� coord*_t...
namespace boost {

// ��� ������ ��������� boost::unordered_set � ��.
inline size_t hash_value( const d::coordInt3d_t& a ) {
    size_t seed = 0;
    boost::hash_combine( seed, a.get<0>() );
    boost::hash_combine( seed, a.get<1>() );
    boost::hash_combine( seed, a.get<2>() );
    return seed;
}


inline bool operator()( const d::coordInt3d_t& a, const d::coordInt3d_t& b ) {
    return (a.get<0>() == b.get<0>())
        && (a.get<1>() == b.get<1>())
        && (a.get<2>() == b.get<2>());
}


}
*/







// d::sizeInt3d_t

inline d::sizeInt3d_t operator-( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return d::sizeInt3d_t(
        a.get<0>() - b.get<0>(),
        a.get<1>() - b.get<1>(),
        a.get<2>() - b.get<2>()
    );
}


inline d::sizeInt3d_t operator-=( d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    a.get<0>() -= b.get<0>();
    a.get<1>() -= b.get<1>();
    a.get<2>() -= b.get<2>();
    return a;
}


inline d::sizeInt3d_t operator+( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return d::sizeInt3d_t(
        a.get<0>() + b.get<0>(),
        a.get<1>() + b.get<1>(),
        a.get<2>() + b.get<2>()
    );
}


inline d::sizeInt3d_t operator+=( d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    a.get<0>() += b.get<0>();
    a.get<1>() += b.get<1>();
    a.get<2>() += b.get<2>();
    return a;
}


inline d::sizeInt3d_t operator*( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return d::sizeInt3d_t(
        a.get<0>() * b.get<0>(),
        a.get<1>() * b.get<1>(),
        a.get<2>() * b.get<2>()
    );
}


inline d::sizeInt3d_t operator*=( d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    a.get<0>() *= b.get<0>();
    a.get<1>() *= b.get<1>();
    a.get<2>() *= b.get<2>();
    return a;
}


inline d::sizeInt3d_t operator*( const d::sizeInt3d_t& a, int k ) {
    return d::sizeInt3d_t(
        a.get<0>() * k,
        a.get<1>() * k,
        a.get<2>() * k
    );
}


inline d::sizeInt3d_t operator/( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return d::sizeInt3d_t(
        a.get<0>() / b.get<0>(),
        a.get<1>() / b.get<1>(),
        a.get<2>() / b.get<2>()
    );
}


inline d::sizeInt3d_t operator/=( d::sizeInt3d_t& a, int k ) {
    a.get<0>() /= k;
    a.get<1>() /= k;
    a.get<2>() /= k;
    return a;
}


inline d::sizeInt3d_t operator/( const d::sizeInt3d_t& a, int k ) {
    return d::sizeInt3d_t(
        a.get<0>() / k,
        a.get<1>() / k,
        a.get<2>() / k
    );
}



inline bool operator==( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return (a.get<0>() == b.get<0>())
        && (a.get<1>() == b.get<1>())
        && (a.get<2>() == b.get<2>());
}

inline bool operator!=( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return !(a == b);
}


inline bool operator>( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return (a.get<0>() > b.get<0>()) && (a.get<1>() > b.get<1>()) && (a.get<2>() > b.get<2>());
}


inline bool operator<( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return (a.get<0>() < b.get<0>()) && (a.get<1>() < b.get<1>()) && (a.get<2>() < b.get<2>());
}


inline bool operator>=( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return (a.get<0>() >= b.get<0>()) && (a.get<1>() >= b.get<1>()) && (a.get<2>() >= b.get<2>());
}


inline bool operator<=( const d::sizeInt3d_t& a, const d::sizeInt3d_t& b ) {
    return (a.get<0>() <= b.get<0>()) && (a.get<1>() <= b.get<1>()) && (a.get<2>() <= b.get<2>());
}







/**
* ����� �������� ��� �������.
*/
inline d::box2d_t operator+( const d::box2d_t& box, const d::coord2d_t& c ) {
    const auto a = box.first  + c;
    const auto b = box.second + c;
    return d::box2d_t( a, b );
}

inline d::box2d_t operator-( const d::box2d_t& box, const d::coord2d_t& c ) {
    const auto a = box.first  - c;
    const auto b = box.second - c;
    return d::box2d_t( a, b );
}








namespace d {



// (!) ������ ���� �������� �������� � ����������������� ����� ������ �
// ����� �������������� � �������� ������� ������� ��� ������ �����.
// ��������: coord2d_t, size2d_t.




/**
* @return ������.
*/
inline d::coordInt3d_t abs( const d::coordInt3d_t& � ) {
    return d::coordInt3d_t(
        std::abs( �.get<0>() ),
        std::abs( �.get<1>() ),
        std::abs( �.get<2>() )
    );
}





/**
* @return �������� �������.
*/
inline bool empty( float v ) {
    return (std::abs( v ) < PRECISION);
}

inline bool empty( int v ) {
    return (v == 0);
}

inline bool empty( const coord2d_t& c ) {
    return empty( c.get<0>() )
        && empty( c.get<1>() );
}

inline bool empty( const coordInt2d_t& c ) {
    return empty( c.get<0>() )
        && empty( c.get<1>() );
}

inline bool empty( const coord3d_t& c ) {
    return empty( c.get<0>() )
        && empty( c.get<1>() )
        && empty( c.get<2>() );
}

inline bool empty( const coordInt3d_t& c ) {
    return empty( c.get<0>() )
        && empty( c.get<1>() )
        && empty( c.get<2>() );
}


/**
* ������� ��������� ������, ���� ���� �� ���� � ������� - ������.
*/
inline bool empty( const box2d_t& b ) {
    //return (b.first == b.second);
    return empty( b.first.get<0>() - b.second.get<0>() )
        || empty( b.first.get<1>() - b.second.get<1>() );
}

inline bool empty( const boxInt2d_t& b ) {
    return empty( b.first.get<0>() - b.second.get<0>() )
        || empty( b.first.get<1>() - b.second.get<1>() );
}

inline bool empty( const box3d_t& b ) {
    return empty( b.first.get<0>() - b.second.get<0>() )
        || empty( b.first.get<1>() - b.second.get<1>() )
        || empty( b.first.get<2>() - b.second.get<2>() );
}

inline bool empty( const boxInt3d_t& b ) {
    return empty( b.first.get<0>() - b.second.get<0>() )
        || empty( b.first.get<1>() - b.second.get<1>() )
        || empty( b.first.get<2>() - b.second.get<2>() );
}


inline bool emptyAny( const coord2d_t& c ) {
    return empty( c.get<0>() )
        || empty( c.get<1>() );
}

inline bool emptyAny( const coordInt2d_t& c ) {
    return empty( c.get<0>() )
        || empty( c.get<1>() );
}

inline bool emptyAny( const coord3d_t& c ) {
    return empty( c.get<0>() )
        || empty( c.get<1>() )
        || empty( c.get<2>() );
}

inline bool emptyAny( const coordInt3d_t& c ) {
    return empty( c.get<0>() )
        || empty( c.get<1>() )
        || empty( c.get<2>() );
}




/**
* @return ������ ���������.
*/
inline bool correct( const box2d_t& b ) {
    return (b.second.get<0>() >= b.first.get<0>())
        && (b.second.get<1>() >= b.first.get<1>());
}

inline bool correct( const boxInt2d_t& b ) {
    return (b.second.get<0>() >= b.first.get<0>())
        && (b.second.get<1>() >= b.first.get<1>());
}






/**
* @return ���������� ����� ����� �������.
*/
inline float calcDistance( const coord2d_t& a, const coord2d_t& b ) {
    const auto d = b - a;
    return std::sqrt(
        d.get<0>() * d.get<0>()
      + d.get<1>() * d.get<1>()
    );
}


inline float calcDistance( const coord3d_t& a, const coord3d_t& b ) {
    const auto d = b - a;
    return std::sqrt(
        d.get<0>() * d.get<0>()
      + d.get<1>() * d.get<1>()
      + d.get<2>() * d.get<2>()
    );
}




/**
* @return ����� � ��������� ������������ ����������� �������.
*         ����������� ���� ������� - ���������, ������������ - ���.
*
* @alias !outsideBox(), ���������
*/
inline bool insideBox( const coordInt3d_t& c, const boxInt3d_t& box ) {
    //return (c >= box.first) && (c < box.second);
    return ( (c.get<0>() >= box.first.get<0>()) && (c.get<1>() >= box.first.get<1>()) && (c.get<2>() >= box.first.get<2>()) )
        && ( (c.get<0>() < box.second.get<0>()) && (c.get<1>() < box.second.get<1>()) && (c.get<2>() < box.second.get<2>()) );
}



/**
* @return ����� � ��������� ������������ ����� �� ��������� �������.
*         ����������� ���� ������� - ����� ������, ������������ - ���.
*
* @alias !insideBox(), ���������
*/
inline bool outsideBox( const coordInt3d_t& c, const boxInt3d_t& box ) {
    //return (c < box.first) && (c >= box.second);
    return ( (c.get<0>() < box.first.get<0>()) || (c.get<1>() < box.first.get<1>()) || (c.get<2>() < box.first.get<2>()) )
        || ( (c.get<0>() >= box.second.get<0>()) || (c.get<1>() >= box.second.get<1>()) || (c.get<2>() >= box.second.get<2>()) );
}




/**
* @return ������ �������.
*/
inline size2d_t calcSize( const box2d_t& b ) {
    //return (size2d_t)(b.second - b.first);
    return size2d_t(
        b.second.get<0>() - b.first.get<0>(),
        b.second.get<1>() - b.first.get<1>()
    );
}


inline sizeInt2d_t calcSize( const boxInt2d_t& b ) {
    //return (sizeInt2d_t)(b.second - b.first);
    return sizeInt2d_t(
        (size_t)(b.second.get<0>() - b.first.get<0>()),
        (size_t)(b.second.get<1>() - b.first.get<1>())
    );
}


inline sizeInt3d_t calcSize( const boxInt3d_t& b ) {
    //return (sizeInt3d_t)(b.second - b.first);
    return sizeInt3d_t(
        (size_t)(b.second.get<0>() - b.first.get<0>()),
        (size_t)(b.second.get<1>() - b.first.get<1>()),
        (size_t)(b.second.get<2>() - b.first.get<2>())
    );
}





/**
* @return ������������ ����� ��� numeric_limits< double >::infinity().
*/
inline double numberCast( const std::string& s ) {
    try {
        return boost::lexical_cast< double >( s );
    }
    catch ( boost::bad_lexical_cast& ) {
        return std::numeric_limits< double >::infinity();
    }
}







/**
* ���������� �������� boost::any.
*
* @return true1, ���� ���� �����. true2, ���� �������� �����.
*
* (!) ����� ���������� ���� ( false, true ), �.�. ����� ������ �����
* ������������ ��� �����.
*
* @param precision �������� ��� ��������� �������� � ��������� ������.
*
* @see strongEquals()
*/
inline std::pair< bool /* ���� ��������� */,  bool /* ���������� ��������� */ >
lazyEquals( const boost::any& a, const boost::any& b, double precision = 1e-8 ) {
    using namespace std;
    using namespace boost;

    // ������� ������ �� ���������
    const type_info& at = a.type();
    const type_info& bt = b.type();
    std::pair< bool, bool >  r = std::make_pair(
        (at == bt),
        false
    );
    
    // double
    if (at == typeid( double )) {
        const auto ca = any_cast< double >( a );
        double cb = numeric_limits< double >::infinity();
        if (bt == typeid( double )) {
            cb = any_cast< double >( b );
        } else if (bt == typeid( float )) {
            cb = (double)any_cast< float >( b );
        } else if (bt == typeid( int )) {
            cb = (double)any_cast< int >( b );
        } else if (bt == typeid( size_t )) {
            cb = (double)any_cast< size_t >( b );
        } else if (bt == typeid( string )) {
            cb = numberCast( any_cast< string >( b ) );
            // ������� infinity(), ���� �� �����
        }
        if (cb != numeric_limits< double >::infinity()) {
            r.second = (precision > std::abs( ca - cb ));
        }

    // float
    } else if (at == typeid( float )) {
        const auto ca = any_cast< float >( a );
        float cb = numeric_limits< float >::infinity();
        if (bt == typeid( double )) {
            cb = (float)any_cast< double >( b );
        } else if (bt == typeid( float )) {
            cb = any_cast< float >( b );
        } else if (bt == typeid( int )) {
            cb = (float)any_cast< int >( b );
        } else if (bt == typeid( size_t )) {
            cb = (float)any_cast< size_t >( b );
        } else if (bt == typeid( string )) {
            cb = (float)numberCast( any_cast< string >( b ) );
            // ������� infinity(), ���� �� �����
        }
        if (cb != numeric_limits< float >::infinity()) {
            r.second = (precision > std::abs( ca - cb ));
        }

    // int
    } else if (at == typeid( int )) {
        const auto ca = any_cast< int >( a );
        if (bt == typeid( double )) {
            r.second = (ca == (int)any_cast< double >( b ));
        } else if (bt == typeid( float )) {
            r.second = (ca == any_cast< float >( b ));
        } else if (bt == typeid( int )) {
            r.second = (ca == any_cast< int >( b ));
        } else if (bt == typeid( size_t )) {
            r.second = (ca == (int)any_cast< size_t >( b ));
        } else if (bt == typeid( string )) {
            const double temp = numberCast( any_cast< string >( b ) );
            // ������� infinity(), ���� �� �����
            if (temp != numeric_limits< double >::infinity()) {
                r.second = (ca == (int)temp);
            }
        }

    // size_t
    } else if (at == typeid( size_t )) {
        const auto ca = any_cast< size_t >( a );
        if (bt == typeid( double )) {
            r.second = (ca == (size_t)any_cast< double >( b ));
        } else if (bt == typeid( float )) {
            r.second = (ca == (size_t)any_cast< float >( b ));
        } else if (bt == typeid( int )) {
            r.second = (ca == (size_t)any_cast< int >( b ));
        } else if (bt == typeid( size_t )) {
            r.second = (ca == any_cast< size_t >( b ));
        } else if (bt == typeid( string )) {
            const double temp = numberCast( any_cast< string >( b ) );
            // ������� infinity(), ���� �� �����
            if (temp != numeric_limits< double >::infinity()) {
                r.second = (ca == (size_t)temp);
            }
        }

    // string
    } else if (at == typeid( string )) {
        const string ca = any_cast< string >( a );
        if (bt == typeid( string )) {
            r.second = (ca == any_cast< string >( b ));
        }
        // �����, ����� ������������ �������? ������ ����� ���������� �
        // ����� � ���������� ����� �������� �� ����
        const double nca = numberCast( ca );
        if (nca != numeric_limits< double >::infinity()) {
            if (bt == typeid( double )) {
                const auto ncb = any_cast< double >( b );
                r.second = (precision > std::abs( nca - ncb ));
            } else if (bt == typeid( float )) {
                const auto ncb = any_cast< float >( b );
                r.second = (precision > std::abs( (float)nca - ncb ));
            } else if (bt == typeid( int )) {
                const auto ncb = any_cast< int >( b );
                r.second = ((int)nca == ncb);
            } else if (bt == typeid( size_t )) {
                const auto ncb = any_cast< size_t >( b );
                r.second = ((size_t)nca == ncb);
            }
            // string ���������� ����
        }

    } // if-else ...

    return r;
}






/**
* ���� �������� � std::map.
*
* @return �������� �� ��������� �������� ��� �� ����� ������.
*
* @source http://stackoverflow.com/questions/535317/checking-value-exist-in-a-stdmap-c
*
* @see hasInMap()
*/
template< class Map, class Val >
inline typename Map::const_iterator searchInMap( const Map& searchMap, const Val& searchVal ) {
    for (auto itr = searchMap.cbegin(); itr != searchMap.cend(); ++itr) {
        if (itr->second == searchVal) {
            return itr;
        }
    }
    return searchMap.cend();
}




/**
* ���� �������� � std::map.
*
* @return true, ���� �������� �������.
*
* @see searchInMap()
*/
template< class Map, typename Val >
inline bool hasInMap( const Map& searchMap, const Val& searchVal ) {
    for (auto itr = searchMap.cbegin(); itr != searchMap.cend(); ++itr) {
        if (itr->second == searchVal) {
            return true;
        }
    }
    return false;
}





/**
* @return ���������� ���������� �������� � ������, ������������ ����������.
*
* @source http://www.bitsbyta.com/2011/03/how-to-counting-distinct-elements-in.html
*/
template< typename T >
inline std::vector< std::pair< T, size_t /* count distinct */ > >  countDistinct( T source[], size_t length ) {
    std::vector< std::pair< T, size_t > >  r;
    for (size_t i = 0; i < length; ++i) {
        int count = 0;
        int flag = 1;
        for (size_t j = 0; j < i; ++j) {
            if (source[i] == source[j]) {
                flag++;
            }
        }
        if (flag == 1) {
            for (size_t k = 0; k < length ; ++k) {
                if (source[i] == source[k]) {
                    count++;
                }
            }
            //std::cout << arr[i] << " - " << count << std::endl;
            r.push_back( std::make_pair( source[i], count ) );
        }

    } // for (size_t i = 0; i < length; ++i)

    return r;
}





} // namespace d








/**
* ����� � �����.
*/
inline ::std::ostream& operator<<( ::std::ostream &out, const d::coord3d_t& c ) {
    out << "(" << c.get<0>() << ", " << c.get<1>() << ", " << c.get<2>() << ")";
    return out;
}


inline ::std::ostream& operator<<( ::std::ostream &out, const d::coordInt3d_t& c ) {
    out << "(" << c.get<0>() << ", " << c.get<1>() << ", " << c.get<2>() << ")";
    return out;
}


inline ::std::ostream& operator<<( ::std::ostream &out, const d::sizeInt3d_t& s ) {
    out << "(" << s.get<0>() << ", " << s.get<1>() << ", " << s.get<2>() << ")";
    return out;
}


inline ::std::ostream& operator<<( ::std::ostream &out, const d::boxInt3d_t& b ) {
    out << "( " << b.first << " : " << b.second << " )";
    return out;
}
