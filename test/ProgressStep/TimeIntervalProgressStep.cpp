#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/base_object.hpp"

#include "boost/date_time/posix_time/time_serialize.hpp"

#include "ProgressStep/TimeIntervalProgressStep.hpp"

using namespace ashley::progress_tracker::progress_step;
using namespace boost::posix_time;//ptime
using namespace boost::gregorian;//date

namespace ashley::progress_tracker::progress_step {
  std::ostream& operator<<( std::ostream& os, const TimeIntervalProgressStep& rhs ) {
    os << "(capacity: " << rhs.getCapacity() << ", duration:[" << rhs.getBeginTimePoint() << ", " << rhs.getEndTimePoint() << "])";
    return os;
  }
}

BOOST_AUTO_TEST_SUITE( TimeIntervalProgressStepTest )
  //1a) Konstruktor haze vyjimku pri nekladne hodnote capacity
  //1b) Konstruktor checkuje validitu predanych time pointu
  //1c) Knostruktor checkuje, ze end intervalu neni driv nez begin intervalu
  //2a) Pri uspesne konstrukci vraci getCapacity() predanou hodnotu
  //2b) Pri uspesne konstrukci vraci getBeginTimePoint() predanou hodnotu
  //2c) Pri uspesne konstrukci vraci getEndTimePoint() predanou hodnotu
  //3) funguje operator==()
  //4) funguje serialize
  BOOST_AUTO_TEST_CASE( test_1a_create_inv_inv_inv_throws ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalProgressStep( 0, ptime(), ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_inv_valid_valid_throws ) {
    //arrange
    ptime begin{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    ptime end{ date{ 2019, 5, 2 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalProgressStep( 0, begin, end ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create_valid_inv_valid_throws ) {
    //arrange
    ptime time{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalProgressStep( 1, time, ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1d_create_valid_valid_inv_throws ) {
    //arrange
    ptime time{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalProgressStep( 1, ptime(), time ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1e_create_valid_valid_valid_interval_invalid_throws ) {
    //arrange
    ptime begin{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    ptime end{ date{ 2019, 5, 2 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalProgressStep( 1, end, begin ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1f_create_valid_valid_valid_interval_ok_no_throw ) {
    //arrange
    ptime begin{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    ptime end{ date{ 2019, 5, 2 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_NO_THROW( TimeIntervalProgressStep( 1, begin, end ) );
  }

  BOOST_AUTO_TEST_CASE( test_1g_create_valid_valid_valid_interval_0seconds_no_throw ) {
    //arrange
    ptime time{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_NO_THROW( TimeIntervalProgressStep( 1, time, time ) );
  }

  BOOST_AUTO_TEST_CASE( test_2_getters_return_correct_values ) {
    //arrange
    ptime begin{ date{ 2017, 7, 26 }, time_duration{ 8, 0, 13 } };
    ptime end{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pS{ 72, begin, end };
    //act
    //assert
    BOOST_TEST( pS.getCapacity() == 72 );
    BOOST_TEST( pS.getBeginTimePoint() == begin );
    BOOST_TEST( pS.getEndTimePoint() == end );
    BOOST_TEST( pS.getDuration() == end - begin );
  }

  BOOST_AUTO_TEST_CASE( test_3a_equal_identical_objects ) {
    //arrange
    ptime begin{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime end{ date{ 2018, 10, 22 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pS{ 810, begin, end };
    //act
    //assert
    BOOST_TEST( pS == pS );
  }

  BOOST_AUTO_TEST_CASE( test_3b_equal_different_objects_same_initializers ) {
    //arrange
    ptime begin{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime end{ date{ 2018, 10, 22 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pS1{ 810, begin, end };
    TimeIntervalProgressStep pS2{ 810, begin, end };
    //act
    //assert
    BOOST_TEST( pS1 == pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3c_equal_different_capacity ) {
    //arrange
    ptime begin{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime end{ date{ 2018, 10, 22 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pS1{ 810, begin, end };
    TimeIntervalProgressStep pS2{ 10, begin, end };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3d_equal_different_begin ) {
    //arrange
    ptime begin1{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime begin2{ date{ 2018, 9, 21 }, time_duration{ 6, 30, 0 } };
    ptime end{ date{ 2018, 10, 22 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pS1{ 810, begin1, end };
    TimeIntervalProgressStep pS2{ 810, begin2, end };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_3e_equal_different_end ) {
    //arrange
    ptime begin{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    ptime end1{ date{ 2018, 10, 22 }, time_duration{ 6, 30, 0 } };
    ptime end2{ date{ 2018, 11, 23 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pS1{ 810, begin, end1 };
    TimeIntervalProgressStep pS2{ 810, begin, end2 };
    //act
    //assert
    BOOST_TEST( pS1 != pS2 );
  }

  BOOST_AUTO_TEST_CASE( test_4_serialize_works ) {
    //arrange
    ptime begin{ date{ 2016, 10, 21 }, time_duration{ 8, 30, 0 } };
    ptime end{ date{ 2018, 10, 21 }, time_duration{ 6, 30, 0 } };
    TimeIntervalProgressStep pSSource{ 12144, begin, end };
    TimeIntervalProgressStep pSTarget{ 1, begin, end };
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
