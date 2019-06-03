#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "InputMap/CliInputMap.hpp"

//toto je funckni zpusob, jak oddelit od sebe kednotlive testy
//!!TODO: implementovat i v ostatnich testovacich souborech
namespace {

using namespace ashley::progress_tracker::input_map;
using namespace boost::program_options;

std::string input_string = "one=1\ntwo=2\nthree=3";
std::stringstream input_stream{ input_string };

options_description input_options{ "InputOptions" };

variables_map vm;

CliInputMap input_map_{ vm };

//test suite fixture
class F { 
  public:
    //setup
    F() { 
      input_options.add_options()
        ( "one", value<int>(), "One" )
        ( "two", value<std::string>(), "Two" )
        ( "three", value<double>(), "Three" );
      store( parse_config_file( input_stream, input_options ), vm );
      //1) konstrukce
      //-- konstruktor je nothrow, jezteberou referenci, toto je almost as good as NO_THROW test
      input_map_ = CliInputMap( vm );
    }
};

BOOST_AUTO_TEST_SUITE( CliInputMapTest, * boost::unit_test::fixture<F>() )
  //2) getAs
  //  - throw when key doesn't exist
  //  - throw when conversion fails
  //  - success when possible
  BOOST_AUTO_TEST_CASE( test_2a_key_doesnt_exist ) {
    BOOST_CHECK_THROW( input_map_.getAs<std::string>( "four" ), std::out_of_range );
  }

  BOOST_AUTO_TEST_CASE( test_2b_conversion_fails ) {
    BOOST_CHECK_THROW( input_map_.getAs<std::string>( "one" ), std::invalid_argument );
    BOOST_CHECK_THROW( input_map_.getAs<int>( "two" ), std::invalid_argument );
    BOOST_CHECK_THROW( input_map_.getAs<std::string>( "three" ), std::invalid_argument );
  }

  BOOST_AUTO_TEST_CASE( test_2c_ok ) {
    BOOST_TEST( input_map_.getAs<int>( "one" ) == 1 );
    BOOST_TEST( input_map_.getAs<std::string>( "two" ) == "2" );
    BOOST_TEST( input_map_.getAs<double>( "three" ) == 3.0 );
  }
BOOST_AUTO_TEST_SUITE_END()

}
