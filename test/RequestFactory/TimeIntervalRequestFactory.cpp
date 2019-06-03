#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "RequestFactory/TimeIntervalRequestFactory.hpp"

#include "InputMap/StringInputMap.hpp"

namespace {

using namespace ashley::progress_tracker::input_map;
using namespace ashley::progress_tracker::request;
using namespace ashley::progress_tracker::request_factory;

std::map<std::string, std::string> incomplete_input_;
StringInputMap incompleteInputMap{ incomplete_input_ };

std::map<std::string, std::string> complete_input_ = { { "capacity", "50" }, { "begin_time", "2019-01-06 01:00:00" }, { "end_time", "2019-01-07 03:00:00" } };
StringInputMap completeInputMap{ complete_input_ };

//1) id TimeIntervalRequestFactory jde pouzit k vytvoreni factory
std::unique_ptr<RequestFactory<TimeIntervalRequest, StringInputMap>> factory = createFactory<TimeIntervalRequest, StringInputMap>( TimeIntervalRequestFactory<StringInputMap>::id );

BOOST_AUTO_TEST_SUITE( TimeIntervalRequestFactory )
  //2) createRequest vyhodi vyjimku, pokud mu chybi nejaka input hodnota
  //   createRequest vrati TimeIntervalRequest se spravnymi hodnotami, pokud
  //      je input v poradku
  //AZ SE OVERI, ZE ptime jde normalne nacist ze stringu, je treba pridat
  //neco jako setTimeParser()
  BOOST_AUTO_TEST_CASE( test_2a_throw_on_incomplete_input ) {
    BOOST_CHECK_THROW( factory->createRequest( incompleteInputMap ), std::out_of_range );
  }

  BOOST_AUTO_TEST_CASE( test_2b_ok ) {
    std::unique_ptr<TimeIntervalRequest> request = factory->createRequest( completeInputMap );
    BOOST_TEST( request->getTotalCapacity() == 50 );

    BOOST_TEST( request->getBeginTimePoint().date().year() == 2019 );
    BOOST_TEST( request->getBeginTimePoint().date().month() == 1 );
    BOOST_TEST( request->getBeginTimePoint().date().day() == 6 );
    BOOST_TEST( request->getBeginTimePoint().time_of_day().hours() == 1 );
    BOOST_TEST( request->getBeginTimePoint().time_of_day().minutes() == 0 );
    BOOST_TEST( request->getBeginTimePoint().time_of_day().seconds() == 0 );

    BOOST_TEST( request->getEndTimePoint().date().year() == 2019 );
    BOOST_TEST( request->getEndTimePoint().date().month() == 1 );
    BOOST_TEST( request->getEndTimePoint().date().day() == 7 );
    BOOST_TEST( request->getEndTimePoint().time_of_day().hours() == 3 );
    BOOST_TEST( request->getEndTimePoint().time_of_day().minutes() == 0 );
    BOOST_TEST( request->getEndTimePoint().time_of_day().seconds() == 0 );
  }
BOOST_AUTO_TEST_SUITE_END()
}
