#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/base_object.hpp"

#include "boost/date_time/posix_time/time_serialize.hpp"

#include "ProgressStep/TimePointProgressStep.hpp"

using namespace ashley::progress_tracker::progress_step;
using namespace boost::posix_time;//ptime
using namespace boost::gregorian;//date

namespace ashley::progress_tracker::progress_step {
  std::ostream& operator<<( std::ostream& os, const TimePointProgressStep& rhs ) {
    os << "(" << rhs.getCapacity() << ", " << rhs.getTimePoint() << ")";
    return os;
  }
}

BOOST_AUTO_TEST_SUITE( TimePointProgressStepTest )
  //1a) Konstruktor haze vyjimku pri nekladne hodnote capacity
  //1b) Konstruktor checkuje validitu predaneho time pointu
  //2a) Pri uspesne konstrukci vraci getCapacity() predanou hodnotu
  //2b) Pri uspesne konstrukci vraci getTimePoint() predanou hodnotu
  //3) funguje operator==()
  //4) funguje serialize
  BOOST_AUTO_TEST_CASE( test_1a_create_invalid_capacity_invalid_time_throws ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( TimePointProgressStep( 0, ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_invalid_capacity_valid_time_throws ) {
    //arrange
    ptime time{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_THROW( TimePointProgressStep( 0, time ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create_valid_capacity_invalid_time_throws ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( TimePointProgressStep( 1, ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1d_create_valid_capacity_valid_time_no_throw ) {
    //arrange
    ptime time{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    //act
    //assert
    BOOST_CHECK_NO_THROW( TimePointProgressStep( 1, time ) );
  }

  BOOST_AUTO_TEST_CASE( test_2_getters_return_correct_values ) {
    //arrange
    ptime time{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    TimePointProgressStep pS{ 43, time };
    //act
    //assert
    BOOST_TEST( pS.getCapacity() == 43 );
    BOOST_TEST( pS.getTimePoint() == time );
  }

  BOOST_AUTO_TEST_CASE( test_3a_equal_identical_objects ) {
    //arrange
    ptime time{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    TimePointProgressStep pS{ 810, time };
    //act
    //assert
    BOOST_TEST( pS == pS );
  }

  BOOST_AUTO_TEST_CASE( test_3b_equal_different_objects_same_initializers ) {
    //arrange
    ptime time{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    TimePointProgressStep pS1{ 810, time };
    TimePointProgressStep pS2{ 810, time };
    //act
    //assert
    BOOST_TEST( pS1 == pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3c_equal_different_capacity_same_time ) {
    //arrange
    ptime time{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    TimePointProgressStep pS1{ 810, time };
    TimePointProgressStep pS2{ 812, time };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3d_equal_same_capacity_different_time ) {
    //arrange
    ptime time1{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime time2{ date{ 2017, 1, 12 }, time_duration{ 3, 27, 42 } };
    TimePointProgressStep pS1{ 812, time1 };
    TimePointProgressStep pS2{ 812, time2 };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3e_equal_different_capacity_different_time ) {
    //arrange
    ptime time1{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime time2{ date{ 2017, 1, 12 }, time_duration{ 3, 27, 42 } };
    TimePointProgressStep pS1{ 810, time1 };
    TimePointProgressStep pS2{ 812, time2 };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_4_serialize_works ) {
    //arrange
    ptime time1{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime time2{ date{ 2016, 10, 21 }, time_duration{ 8, 30, 0 } };
    TimePointProgressStep pSSource{ 12144, time1 };
    TimePointProgressStep pSTarget{ 1, time2 };
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
