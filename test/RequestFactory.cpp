#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "RequestFactory.hpp"

using namespace ashley::progress_tracker::request_factory;

std::string notAFactoryId = "JDWQH@*#)!#Not a request factory*#&!@#";

class DummyRequest{};

BOOST_AUTO_TEST_SUITE( RequestFactoryTest )
  //1) getKnownFactoryIds() nevyhodi vyjimku
  //2) Pro kazdou hodnotu z Range metoda createRequestFactory vrati non-null tovarnu
  //
  //Testovat createRequest() zde nelze, protoze funkce teto metody zavisi na vstupni
  //mape inputu a zde nelze vytvorit takovou mapu, aby zarucene fungovala pro vsechny
  //RequestFactories.
  BOOST_AUTO_TEST_CASE( test_1a_nothrow ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( getKnownFactoryIds() );
  }

  BOOST_AUTO_TEST_CASE( test_2a_createFactory_throw_on_invalid_factoryId ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( RequestFactory<DummyRequest>::createFactory( notAFactoryId ), std::invalid_argument );
  }

  BOOST_AUTO_TEST_CASE( test_2b_createFactory_sanity_check ) {
    //arrange
    for ( auto factoryId : getKnownFactoryIds() ) {
      //act
      //assert
      BOOST_CHECK_NO_THROW( RequestFactory<DummyRequest>::createFactory( factoryId ) );
    }
  }
BOOST_AUTO_TEST_SUITE_END()
