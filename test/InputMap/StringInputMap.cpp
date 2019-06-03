#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "InputMap/StringInputMap.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"

using namespace ashley::progress_tracker::input_map;

std::map<std::string, std::string> map_ = { { "int", "1" }, { "string", "two" }, { "size_t", "3" }, { "ptime", "2019-06-02 12:00:00" } };
//1) konstrukce
//-- konstruktor je nothrow, jezteberou referenci, toto je almost as good as NO_THROW test
StringInputMap input_map_{ map_ };

BOOST_AUTO_TEST_SUITE( StringInputMapTest )
  //2) getAs
  //  - throw when key doesn't exist
  //  - throw when conversion fails
  //  - success when possible
  BOOST_AUTO_TEST_CASE( test_2a_key_doesnt_exist ) {
    BOOST_CHECK_THROW( input_map_.getAs<std::string>( "three" ), std::out_of_range );
  }

  BOOST_AUTO_TEST_CASE( test_2b_conversion_fails ) {
    BOOST_CHECK_THROW( input_map_.getAs<int>( "string" ), std::invalid_argument );
  }

  BOOST_AUTO_TEST_CASE( test_2c_ok ) {
    BOOST_TEST( input_map_.getAs<int>( "int" ) == 1 );
    BOOST_TEST( input_map_.getAs<std::string>( "string" ) == "two" );
    BOOST_TEST( input_map_.getAs<std::size_t>( "size_t" ) == 3 );
    auto ptime = input_map_.getAs<boost::posix_time::ptime>( "ptime" );
    auto date = ptime.date();
    auto time = ptime.time_of_day();
    BOOST_TEST( date.year() == 2019 );
    BOOST_TEST( date.month() == 6 );
    BOOST_TEST( date.day() == 2 );
    BOOST_TEST( time.hours() == 12 );
    BOOST_TEST( time.minutes() == 0 );
    BOOST_TEST( time.seconds() == 0 );
  }
BOOST_AUTO_TEST_SUITE_END()
