#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "RequestFactory.hpp"

using namespace ashley::progress_tracker::request_factory;

class DummyRequest{};
class DummyInput{};

class DummyRequestFactory : public RequestFactory<DummyRequest, DummyInput> {
  public:
    //cywe RF_ID ma bejt const uz samo o sobe!
    static const RF_ID id;

    std::unique_ptr<DummyRequest> createRequest( const DummyInput& input ) const override {
      return std::make_unique<DummyRequest>();
    }

    ~DummyRequestFactory() = default;
};

//cywe RF_ID ma bejt const uz samo o sobe!
const RF_ID DummyRequestFactory::id = "DummyRequestFactory";
const RF_ID unsupportedRequestFactoryId = "Unsupported request factory";
const RF_ID notAFactoryId = "JDWQH@*#)!#Not a request factory*#&!@#";

namespace ashley::progress_tracker::request_factory {
  template<>
  RF_Range getKnownFactoryIds<DummyRequest>() {
    static std::vector<RF_ID> ids = {
      DummyRequestFactory::id,
      unsupportedRequestFactoryId
    };
    return RF_Range{ ids };
  }
}

template<>
std::unique_ptr<RequestFactory<DummyRequest, DummyInput>>
RequestFactory<DummyRequest, DummyInput>::createFactory( RF_ID rfId ) {
  if ( rfId == DummyRequestFactory::id ) {
    return std::make_unique<DummyRequestFactory>();
  }
  return nullptr;
}

BOOST_AUTO_TEST_SUITE( RequestFactoryTest )
  //1) getKnownFactoryIds() nevyhodi vyjimku
  //2) metoda createFactory zkusi vytvorit tovarnu s predanym id
  //
  //Testovat createRequest() zde nelze, protoze funkce teto metody zavisi na vstupni
  //mape inputu a zde nelze vytvorit takovou mapu, aby zarucene fungovala pro vsechny
  //RequestFactories. !!Nene, to se udela tak, ze si tady primo implementuju nejakou
  //tovarnu a vyzkousim to na ni, jako u InputMapy!!
  BOOST_AUTO_TEST_CASE( test_1a_nothrow ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( getKnownFactoryIds<DummyRequest>() );
  }

  BOOST_AUTO_TEST_CASE( test_2a_createFactory_throw_on_invalid_factoryId ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( (createFactory<DummyRequest, DummyInput>( notAFactoryId ) ), std::invalid_argument );
  }

  BOOST_AUTO_TEST_CASE( test_2b_createFactory_throw_on_badly_impled_factory ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( (createFactory<DummyRequest, DummyInput>( unsupportedRequestFactoryId ) ), std::invalid_argument );
  }
BOOST_AUTO_TEST_SUITE_END()
