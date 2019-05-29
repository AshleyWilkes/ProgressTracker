#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "InputMap.hpp"

using namespace ashley::progress_tracker::input_map;

//globalni promenne jsou zlo; tady to teda skoro vubec nevadi,
//protoze jsou tyto promenne videt jen v tomto testovacim souboru,
//ale i tak by bylo lepsi zvolit jine reseni
std::string str = "str";
int ii = 123;

class DummyInputValue {
  public:
    DummyInputValue( int value ) : value_{ value } {}

    template<typename T>
    T& as();
  private:
    int value_;
};

template<>
std::string&
DummyInputValue::as<std::string>() {
  if ( value_ == 1 ) {
    return str;
  }
  throw std::domain_error{ "" };
}

template<>
int&
DummyInputValue::as<int>() {
  if ( value_ == 2 ) {
    return ii;
  }
  throw std::domain_error{ "" };
}

class DummyInputMap : public InputMap<int, DummyInputValue> {
  public:
    DummyInputMap() : map_{} { 
      map_.insert({ 1, DummyInputValue( 1 ) });
      map_.insert({ 2, DummyInputValue( 2 ) });
    }
  private:
    DummyInputValue& get( const int& key ) override { return map_.at( key ); }
    std::map<int, DummyInputValue> map_;
};

DummyInputMap map;

BOOST_AUTO_TEST_SUITE( InputMap )
  //1) getAs
  //  - success when possible, check for int, double, std::string
  //  - throw when key doesn't exist
  //  - throw when conversion is not available
  BOOST_AUTO_TEST_CASE( test_1a_key_doesnt_exist ) {
    BOOST_CHECK_THROW( map.getAs<int>( 3 ), std::out_of_range );
  }

  BOOST_AUTO_TEST_CASE( test_1b_conversion_not_possible ) {
    BOOST_CHECK_THROW( map.getAs<int>( 1 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1c_ok ) {
    BOOST_TEST( map.getAs<std::string>( 1 ) == str );
    BOOST_TEST( map.getAs<int>( 2 ) == ii );
  }
BOOST_AUTO_TEST_SUITE_END()
