namespace siu {


template< size_t SX, size_t SY, size_t SZ >
inline ScaleBitMap< SX, SY, SZ >::ScaleBitMap(
) :
    mShape(),
    mScaleBitMap(),
    bm_(),
    // ����� �������� ������������ ����������, ����� ������ ����� draw()
    // ������ ������ ����� ��������
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
        // ������ ��� �� �������, ��������
        return bm_;
    }

    
    bm_.reset();

    // �-�������� ����� ���� ������������ ������� �����������
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
            && "�-�������� �� ����� ����������. ��� ������� ���������� ������� ������������ operator<<()." );
    }

    c_ = c;

    return bm_;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<(
    const typename shape::ElevationMap< OSX, OSY, OSZ >& source
) {
    // @todo ��������� ���������� ����� ����� � "������������" ���������.
    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "����� ����� ������ ���� ������� �� �� �� ���-�� �����, ��� �-��������." );

    mShape = shape_t( new shape::ElevationMap< OSX, OSY, OSZ >( source ) );

    return *this;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<(
    const typename ScaleBitMap< OSX, OSY, OSZ >& source
) {
    // @todo ��������� ���������� �-�������� � "������������" ���������.
    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "�������������� ��������-�������� ������ ���� ������� �� �� �� ���-�� �����, ��� ��� �-��������." );

    mScaleBitMap = scaleBitMap_t( new ScaleBitMap< OSX, OSY, OSZ >( source ) );

    return *this;
}







template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline typename ScaleBitMap< SX, SY, SZ >::bm_t  ScaleBitMap< SX, SY, SZ >::drawShape(
    const typename shape::ElevationMap< OSX, OSY, OSZ >::Ptr& elevationMap
) const {

    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "����� ����� ������ ���� ������� �� �� �� ���-�� �����, ��� �-��������." );

    // �����-�������� ������ �������� � �������� 1:1
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
        "�-��������-�������� ������ ���� ������� �� �� �� ���-�� �����, ��� ��� �-��������." );

    // ������� �������� ���������� ��������
    const auto sh = sbm->shape();
    assert( sh && "�-�������� ������ �������� ���������� � ���� �����." );

    // ������ ����� ��������

    /* - @todo ������. ������� ������� ������.
    // ��� ����� ��������� �������� � ������ ������� �������� � ������� �� ��
    // ���������� ������
    const auto shapeWithParent = sh->scale< OSX*SX, OSY*SY, OSZ*SZ >();
    const Shaper< OSX*SX, OSY*SY, OSZ*SZ >  shaper( shapeWithParent );

    // �������� ���� ������
    return shaper.draw( �, c );
    */


    // �������� ���� ������ � ������� ��������
    const Shaper< SX, SY, SZ >  shaper( sh );

    return shaper.draw( c, OSX, OSY, OSZ );
}




} // siu
