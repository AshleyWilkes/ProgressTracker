#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "RequestFactory/TimePointRequestFactory.hpp"

#include "InputMap/StringInputMap.hpp"

namespace {

using namespace ashley::progress_tracker::input_map;
using namespace ashley::progress_tracker::request;
using namespace ashley::progress_tracker::request_factory;

std::map<std::string, std::string> incomplete_input_;
StringInputMap incompleteInputMap{ incomplete_input_ };

std::map<std::string, std::string> complete_input_ = { { "capacity", "2224" }, { "time", "1912-04-14 23:40:00" } };
StringInputMap completeInputMap{ complete_input_ };

//1) id TimePointRequestFactory jde pouzit k vytvoreni factory
std::unique_ptr<RequestFactory<TimePointRequest, StringInputMap>> factory = createFactory<TimePointRequest, StringInputMap>( TimePointRequestFactory<StringInputMap>::id );

BOOST_AUTO_TEST_SUITE( TimePointRequestFactory )
  //2) createRequest vyhodi vyjimku, pokud mu chybi nejaka input hodnota
  //   createRequest vrati TimePointRequest se spravnymi hodnotami, pokud
  //      je input v poradku
  //AZ SE OVERI, ZE ptime jde normalne nacist ze stringu, je treba pridat
  //neco jako setTimeParser()
  BOOST_AUTO_TEST_CASE( test_2a_throw_on_incomplete_input ) {
    BOOST_CHECK_THROW( factory->createRequest( incompleteInputMap ), std::out_of_range );
  }

  BOOST_AUTO_TEST_CASE( test_2b_ok ) {
    std::unique_ptr<TimePointRequest> request = factory->createRequest( completeInputMap );
    BOOST_TEST( request->getTotalCapacity() == 2224 );
    BOOST_TEST( request->getTimePoint().date().year() == 1912 );
    BOOST_TEST( request->getTimePoint().date().month() == 4 );
    BOOST_TEST( request->getTimePoint().date().day() == 14 );
    BOOST_TEST( request->getTimePoint().time_of_day().hours() == 23 );
    BOOST_TEST( request->getTimePoint().time_of_day().minutes() == 40 );
    BOOST_TEST( request->getTimePoint().time_of_day().seconds() == 0 );
  }
BOOST_AUTO_TEST_SUITE_END()

}
