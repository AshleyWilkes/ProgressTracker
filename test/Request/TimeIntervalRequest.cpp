#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "Request/TimeIntervalRequest.hpp"

using namespace ashley::progress_tracker::progress_step;
using namespace ashley::progress_tracker::request;
using namespace boost::posix_time;//ptime
using namespace boost::gregorian;//date

BOOST_AUTO_TEST_SUITE( TimeIntervalRequestTest )
  //1) Konstruktor checkuje validitu parametru 
  //2) useCapacity:
  //  chci otestovat, ze createProgressStep vytvari stepy s predanou hodnotou capacity a spravnymi timePointy
  BOOST_AUTO_TEST_CASE( test_1a_create_invalid_capacity_throws ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    ptime end{ date{ 2019, 10, 21 }, time_duration{ 15, 37, 0 } };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalRequest( 0, begin, end ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_invalid_begin_throws ) {
    //arrange
    ptime end{ date{ 2019, 10, 21 }, time_duration{ 15, 37, 0 } };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalRequest( 1, ptime(), end ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create_invalid_end_throws ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalRequest( 2, begin, ptime() ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1d_create_invalid_interval_throws ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    ptime end{ date{ 2017, 10, 21 }, time_duration{ 15, 37, 0 } };
    //act
    //assert
    BOOST_CHECK_THROW( TimeIntervalRequest( 5, begin, end ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1e_valid_params_no_throw ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    ptime end{ date{ 2019, 10, 21 }, time_duration{ 15, 37, 0 } };
    //act
    //assert
    BOOST_CHECK_NO_THROW( TimeIntervalRequest( 5, begin, end ) );
  }

  BOOST_AUTO_TEST_CASE( test_2a_use_capacity_no_throw ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    ptime end{ date{ 2019, 10, 21 }, time_duration{ 15, 37, 0 } };
    TimeIntervalRequest req{ 5, begin, end };
    //act
    //assert
    BOOST_CHECK_NO_THROW( req.useCapacity( 3 ) );
  }

  BOOST_AUTO_TEST_CASE( test_2b_use_capacity_returns_ps_with_matching_begins_and_ends ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    ptime end{ date{ 2019, 1, 1 }, time_duration{ 16, 30, 0 } };
    TimeIntervalRequest req{ 7, begin, end };
    //act
    //assert
    std::unique_ptr<TimeIntervalProgressStep> pS_ptr = req.useCapacity( 1 );
    BOOST_CHECK( pS_ptr->getBeginTimePoint() == begin );
    ptime expected_begin = pS_ptr->getEndTimePoint();
    for ( int i = 0; i < 5; ++i ) {
      pS_ptr = req.useCapacity( 1 );
      BOOST_CHECK( pS_ptr->getBeginTimePoint() == expected_begin );
      expected_begin = pS_ptr->getEndTimePoint();
    }
    pS_ptr = req.useCapacity( 1 );
    BOOST_CHECK( pS_ptr->getBeginTimePoint() == expected_begin );
    BOOST_CHECK( pS_ptr->getEndTimePoint() == end );
  }

  BOOST_AUTO_TEST_CASE( test_2c_use_capacity_returns_ps_with_matching_lengths ) {
    //arrange
    ptime begin{ date{ 2018, 12, 31 }, time_duration{ 16, 30, 0 } };
    ptime end{ date{ 2019, 1, 1 }, time_duration{ 16, 30, 0 } };
    TimeIntervalRequest req{ 6, begin, end };
    //act
    //assert
    for ( int i = 0; i < 6; ++i ) {
      auto pS_ptr = req.useCapacity( 1 );
      BOOST_CHECK( pS_ptr->getDuration() == hours( 4 ) );
    }
  }
BOOST_AUTO_TEST_SUITE_END()
