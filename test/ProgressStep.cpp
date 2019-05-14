#define BOOST_TEST_DYN_LINK
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/base_object.hpp"
#include "boost/test/unit_test.hpp"

#include "ProgressStep.hpp"

using namespace ashley::progress_tracker;

class DummyProgressStep : public ProgressStep {
  public:
    DummyProgressStep( std::size_t capacity ) : ProgressStep( capacity ) {}

    template<typename Archive>
    void serialize( Archive& ar, const unsigned int version ) {
      ar & boost::serialization::base_object<ProgressStep>( *this );
    }
};

std::ostream& operator<<( std::ostream& os, const DummyProgressStep& rhs ) {
  os << rhs.getCapacity();
  return os;
}

BOOST_AUTO_TEST_SUITE( ProgressStepTest )
  //1) Parametr capacity konstruktoru musi byt kladny, jinak vyjimka
  //2) operator==() funguje
  //3) serialize funguje, tj. lze objekt zapsat do streamu, z nej
  //    nacist obsah druheho objektu a ziskat tak dva totozne objekty
  BOOST_AUTO_TEST_CASE( test_1a_create_ok_with_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( DummyProgressStep( 1 ) );
    BOOST_REQUIRE_NO_THROW( DummyProgressStep( 42 ) );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_not_ok_with_non_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( DummyProgressStep( 0 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_2a_equal_identical_objects ) {
    //arrange
    DummyProgressStep pS1{ 1 };
    //act
    //assert
    BOOST_TEST( pS1 == pS1 );
  }

  BOOST_AUTO_TEST_CASE( test_2b_equal_different_object_same_initializer ) {
    //arrange
    DummyProgressStep pS1{ 1 };
    DummyProgressStep pS2{ 1 };
    //act
    //assert
    BOOST_TEST( pS1 == pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_2c_equal_different_object_different_initializers ) {
    //arrange
    DummyProgressStep pS1{ 1 };
    DummyProgressStep pS2{ 23491 };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3_serialize_works ) {
    //arrange
    DummyProgressStep pSSource{ 12144 };
    DummyProgressStep pSTarget{ 1 };
    std::stringstream stream;
    //act
    boost::archive::text_oarchive oa{ stream };
    oa << pSSource;
    boost::archive::text_iarchive ia{ stream };
    ia >> pSTarget;
    //assert
    BOOST_TEST( pSSource == pSTarget );
  }
BOOST_AUTO_TEST_SUITE_END()
