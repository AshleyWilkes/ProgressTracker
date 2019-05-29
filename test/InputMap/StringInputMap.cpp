#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "InputMap/StringInputMap.hpp"

using namespace ashley::progress_tracker::input_map;

std::map<std::string, std::string> map_ = { { "one", "1" }, { "two", "2" }, { "three", "3" } };
//1) konstrukce
//-- konstruktor je nothrow, jezteberou referenci, toto je almost as good as NO_THROW test
StringInputMap input_map_{ map_ };

BOOST_AUTO_TEST_SUITE( StringInputMapTest )
  //2) getAs
  //  - throw when key doesn't exist
  //  - success when possible
  //Note: conversion is not tested, since only conversion to std::string is implemented,
  //no other conversion compiles
  BOOST_AUTO_TEST_CASE( test_2a_key_doesnt_exist ) {
    BOOST_CHECK_THROW( input_map_.getAs<std::string>( "four" ), std::out_of_range );
  }

  BOOST_AUTO_TEST_CASE( test_2b_ok ) {
    BOOST_TEST( input_map_.getAs<std::string>( "one" ) == "1" );
    BOOST_TEST( input_map_.getAs<std::string>( "two" ) == "2" );
    BOOST_TEST( input_map_.getAs<std::string>( "three" ) == "3" );
  }
BOOST_AUTO_TEST_SUITE_END()
