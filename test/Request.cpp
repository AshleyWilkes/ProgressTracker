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
  private:
    UPS createProgressStep( std::size_t capacity ) override {
      return std::make_unique<DummyProgressStep>( capacity );
    }
};

class LimitingDummyRequest : public Request<ProgressStep> {
  public:
    LimitingDummyRequest( std::size_t totalCapacity, std::size_t maxPSCapacity = 3 ) :
          Request( totalCapacity ), maxPSCapacity_{ maxPSCapacity } {}

  private:
    UPS createProgressStep( std::size_t capacity ) override {
      if ( capacity > maxPSCapacity_ ) {
        capacity = maxPSCapacity_;
      }
      return std::make_unique<DummyProgressStep>( capacity );
    }

    const std::size_t maxPSCapacity_;
};

class BadlyImpledRequest : public Request<ProgressStep> {
  public:
    BadlyImpledRequest( std::size_t capacity ) : Request( capacity ) {}
  private:
    UPS createProgressStep( std::size_t capacity) override {
      return nullptr;
    }
};

template<typename PS>
void checkRequestInvariants( const Request<PS>& req ) {
  BOOST_TEST( req.isFullyProcessed() == ( req.getAvailableCapacity() == 0 ) );
  BOOST_TEST( req.getTotalCapacity() > 0 );
  BOOST_TEST( req.getUsedCapacity() >= 0 );
  BOOST_TEST( req.getAvailableCapacity() >= 0 );
  BOOST_TEST( req.getUsedCapacity() + req.getAvailableCapacity() == req.getTotalCapacity() );
}

BOOST_AUTO_TEST_SUITE( RequestTest )
  void createRequestAndCheckInvariants( std::size_t capacity ) {
    DummyRequest req{ capacity };
    BOOST_TEST( req.getTotalCapacity() == capacity );
    checkRequestInvariants( req );
  }

  //tohle podporuje pouze jedine volani useCapacity, coz je skoda
  //upravit na vice volani a pripsat test, ktery toho vyuzije,
  //jinak ted neni duvod predpokladat, ze bude fungovat, kdyz se
  //jeden request ma rozhodit do vice boxu
  template<typename R>//R -- Request
  void checkUseCapacity( std::size_t initial, std::size_t requested, std::size_t received, std::size_t remaining ) {
    R req{ initial };
    //act
    ProgressStep* ps = req.useCapacity( requested ).get();
    //assert
    BOOST_CHECK( ps != nullptr );
    BOOST_CHECK( ps->getCapacity() == received );
    BOOST_CHECK( req.getAvailableCapacity() == remaining );
    checkRequestInvariants( req );
  }
  //1a) Parametr totalCapacity konstruktoru nekladny => vyjimka
  //1b) Parametr kladny => vytvoren objekt se spravnou kapacitou a invarianty 4 metod
  //2a) useCapacity throw, kdyz je pozadavek nekladny
  //2b) useCapacity throw, kdyz createProgressStep vrati nullptr
  //2c) useCapacity vrati ProgressStep s pozadovanou kapacitou, pokud ji jeste ma
  //        usedCapacity se prislusnym zpusobem zvetsi
  //        plati invarianty 4 metod
  //2d) useCapacity vrati ProgressStep s kapacitou, kterou ma, pokud pozadavek je vetsi
  //        usedCapacity se prislusnym zpusobem zvetsi
  //        plati invarianty 4 metod
  //2e) useCapacity throw, kdyz je capacity plne vyuzita
  //2f) useCapacity pridava k usedCaoacity_ skutecnou kapacity vraceneho ProgressStepu,
  //        i kdyz se tato lisi od pozadovane
  BOOST_AUTO_TEST_CASE( test_1a_create_not_ok_with_non_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( DummyRequest( 0 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_ok_with_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( DummyRequest( 1 ) );
    BOOST_REQUIRE_NO_THROW( DummyRequest( 42 ) );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create_invariants_hold ) {
    //arrange
    std::array<int, 3> vals{ 1, 21, 4267 };
    //act
    //assert
    std::for_each( vals.begin(), vals.end(), &createRequestAndCheckInvariants );
  }

  BOOST_AUTO_TEST_CASE( test_2a_use_capacity_throw_on_non_positive_parameter ) {
    //arrange
    DummyRequest req{ 1 };
    //act
    //assert
    BOOST_REQUIRE_THROW( req.useCapacity( 0 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_2b_use_capacity_throw_when_createProgressStep_returns_nullptr ) {
    //arrange
    BadlyImpledRequest req{ 1 };
    //act
    //assert
    BOOST_REQUIRE_THROW( req.useCapacity( 1 ), std::runtime_error );
  }

  BOOST_AUTO_TEST_CASE( test_2c_use_capacity_requested_when_available ) {
    //arrange
    //act
    //assert
    checkUseCapacity<DummyRequest>( 5, 1, 1, 4 );
    checkUseCapacity<DummyRequest>( 8, 8, 8, 0 );
  }

  BOOST_AUTO_TEST_CASE( test_2d_use_capacity_available_when_requested_too_much ) {
    //arrange
    //act
    //assert
    checkUseCapacity<DummyRequest>( 1248, 7261, 1248, 0 );
  }

  BOOST_AUTO_TEST_CASE( test_2e_use_capacity_throw_on_fully_processed ) {
    //arrange
    DummyRequest req{ 3 };
    //act
    req.useCapacity( 3 );
    //assert
    BOOST_REQUIRE_THROW( req.useCapacity( 1 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_2f_use_capacity_respects_actual_PS_capacity ) {
    //arrange
    //act
    //assert
    checkUseCapacity<LimitingDummyRequest>( 5, 1, 1, 4 );
    checkUseCapacity<LimitingDummyRequest>( 8, 8, 3, 5 );
  }
BOOST_AUTO_TEST_SUITE_END()
