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

    // �-�������� ����� ���� ������������ ������� �����������
    assert( mShape
        && "�-�������� �� ����� ����������. ��� ������� ���������� ������� ������������ operator<<()." );

    const Shaper< SX, SY, SZ >  shaper( mShape );
    bm_ = shaper.draw();

    return bm_;
}





template< size_t SX, size_t SY, size_t SZ >
template< size_t OSX, size_t OSY, size_t OSZ >
inline ScaleBitMap< SX, SY, SZ >&  ScaleBitMap< SX, SY, SZ >::operator<<(
    const typename shape::Shape< OSX, OSY, OSZ >& source
) {
    // @todo ��������� ���������� ����� � "������������" ������.
    static_assert( ( (SX == OSX) && (SY == OSY) && (SZ == OSZ) ),
        "����� ����� ������ ���� ������� �� �� �� ���-�� �����, ��� �-��������." );

    mShape = source.clone();

    return *this;
}



} // siu
