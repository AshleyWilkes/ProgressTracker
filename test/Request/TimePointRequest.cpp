#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "Request/TimePointRequest.hpp"

using namespace ashley::progress_tracker::progress_step;
using namespace ashley::progress_tracker::request;
using namespace boost::posix_time;//ptime
using namespace boost::gregorian;//date

BOOST_AUTO_TEST_SUITE( TimePointRequestTest )
  //1) Konstruktor checkuje validitu capacity x timePoint parametru (vyjimka when either is invalid)
  //2) useCapacity:
  //  chci otestovat, ze createProgressStep vytvari stepy s predanou hodnotou capacity a svym timePointem
  BOOST_AUTO_TEST_CASE( test_1a_create_invalid_capacity_invalid_time_throws ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( TimePointRequest( 0, ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_invalid_capacity_valid_time_throws ) {
    //arrange
    ptime time{ date{ 2019, 5, 1 }, time_duration{ 12, 0, 0} };
    //act
    //assert
    BOOST_CHECK_THROW( TimePointRequest( 0, time ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create_valid_capacity_invalid_time_throws ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( TimePointRequest( 1, ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1d_create_valid_capacity_valid_time_no_throw ) {
    //arrange
    ptime time{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    //act
    //assert
    BOOST_CHECK_NO_THROW( TimePointRequest( 1, time ) );
  }

  BOOST_AUTO_TEST_CASE( test_2a_use_capacity_no_throw ) {
    //arrange
    ptime time{ date{ 2018, 2, 23 }, time_duration{ 2, 14, 0 } };
    TimePointRequest req{ 5, time };
    //act
    //assert
    BOOST_CHECK_NO_THROW( req.useCapacity( 3 ) );
  }

  BOOST_AUTO_TEST_CASE( test_2b_use_capacity_returns_ps_with_proper_time_point ) {
    //arrange
    ptime time{ date{ 2018, 9, 25 }, time_duration{ 2, 15, 0 } };
    TimePointRequest req{ 5, time };
    //act
    std::unique_ptr<TimePointProgressStep> pS_ptr = req.useCapacity( 3 );
    //assert
    BOOST_CHECK( pS_ptr->getTimePoint() == time );
  }
BOOST_AUTO_TEST_SUITE_END()
