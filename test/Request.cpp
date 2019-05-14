#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "ProgressStep.hpp"
#include "Request.hpp"

using namespace ashley::progress_tracker;

class DummyProgressStep : public ProgressStep {
  public:
    DummyProgressStep( std::size_t capacity ) : ProgressStep( capacity ) {}
};

class DummyRequest : public Request<DummyProgressStep> {
  public:
    DummyRequest( std::size_t totalCapacity ) : Request( totalCapacity ) {}

    UPS useCapacity( std::size_t capacity ) override {
      return nullptr;
    }

    bool isFullyProcessed() const override { return true; }
};

BOOST_AUTO_TEST_SUITE( RequestTest )
  //1) Parametr totalCapacity konstruktoru musi byt kladny, jinak vyjimka
  BOOST_AUTO_TEST_CASE( test_1a_create_ok_with_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( DummyRequest( 1 ) );
    BOOST_REQUIRE_NO_THROW( DummyRequest( 42 ) );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_not_ok_with_non_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( DummyRequest( 0 ), std::domain_error );
  }
BOOST_AUTO_TEST_SUITE_END()
