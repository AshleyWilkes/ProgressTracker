#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "BoxSelector.hpp"

using namespace ashley::progress_tracker;
using vecI = std::vector<int>;
using itI = vecI::iterator;

vecI nonEmptyInputVals{ 1, 2, 3, 4, 5 };
boost::iterator_range<itI> nonEmptyInputRange{ nonEmptyInputVals };

vecI emptyInputVals;
boost::iterator_range<itI> emptyInputRange{ emptyInputVals };

std::string notABsId = "SAD^&!@This is not a BoxSe13ctor";

BOOST_AUTO_TEST_SUITE( BoxSelectorTest )
  //1) getKnownNbsIds() nevyhodi vyjimku a vrati pouzitelny Range nejvyse 100 hodnot
  //2) Pro kazdou hodnotu z Range metoda select vrati index z intervalu [0, size);
  //3) Pro kazdou hodnotu z Range metoda create vrati non-null_ptr hodnotu.
  //4) Pro kazdou hodnotu z Range metoda select() objektu z create() vrati index
  //    z intervalu [0, size)
  BOOST_AUTO_TEST_CASE( test_1a_nothrow ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( BoxSelector::getKnownBsIds() );
  }

  /*BOOST_AUTO_TEST_CASE( test_1b_result ) {
    //arrange
    //act
    //assert
    auto knownBsIdsRange = BoxSelector::getKnownBsIds();
    BOOST_TEST_REQUIRE( ! knownBsIdsRange.empty() );

    auto begin = knownBsIdsRange.begin();
    int maxExpectedBsCount = 100;
    while ( maxExpectedBsCount-- > 0 && begin++ != knownBsIdsRange.end() );
    bool endFound = begin == knownBsIdsRange.end();
    BOOST_TEST_REQUIRE( endFound, "Too long (perhaps invalid?) range returned" );
  }*/

  BOOST_AUTO_TEST_CASE( test_2a_select_throw_on_empty_input ) {
    //arrange
    for ( auto bsId : BoxSelector::getKnownBsIds() ) {
      //act
      //assert
      BOOST_CHECK_THROW(
          BoxSelector::select( emptyInputRange, bsId ),
          std::domain_error );
    }
  }

  BOOST_AUTO_TEST_CASE( test_2b_select_throw_on_invalid_bsId ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW(
        BoxSelector::select( nonEmptyInputRange, notABsId ),
        std::invalid_argument );
  }

  BOOST_AUTO_TEST_CASE( test_2c_select_return_value_sanity_check ) {
    //arrange
    for ( auto bsId : BoxSelector::getKnownBsIds() ) {
      //act
      std::size_t val = BoxSelector::select( nonEmptyInputRange, bsId );
      //assert
      BOOST_TEST( val >= 0 );
      BOOST_TEST( val < nonEmptyInputVals.size() );
    }
  }

  BOOST_AUTO_TEST_CASE( test_3a_create_sanity_check ) {
    //arrange
    for ( auto bsId : BoxSelector::getKnownBsIds() ) {
      //act
      auto& selector = BoxSelector::create( bsId );
      //assert
      BOOST_TEST( selector.get() != nullptr );
    }
  }

  BOOST_AUTO_TEST_CASE( test_3b_create_throw_on_invalid_bsId ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( BoxSelector::create( notABsId ), std::invalid_argument );
  }

  BOOST_AUTO_TEST_CASE( test_4a_create_select_return_value_sanity_check ) {
    //arrange
    for ( auto bsId : BoxSelector::getKnownBsIds() ) {
      auto& selector = BoxSelector::create( bsId );
      //act
      std::size_t val = selector->select( nonEmptyInputRange );
      //assert
      BOOST_TEST( val >= 0 );
      BOOST_TEST( val < nonEmptyInputVals.size() );
    }
  }

  BOOST_AUTO_TEST_CASE( test_4b_create_select_throw_on_empty_input ) {
    //arrange
    for ( auto bsId : BoxSelector::getKnownBsIds() ) {
      auto& selector = BoxSelector::create( bsId );
      //act
      //assert
      BOOST_CHECK_THROW( selector->select( emptyInputRange ), std::domain_error );
    }
  }

BOOST_AUTO_TEST_SUITE_END()
