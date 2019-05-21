#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/base_object.hpp"

#include "Box.hpp"
#include "ProgressStep.hpp"

using namespace ashley::progress_tracker;

class DummyProgressStep : public ProgressStep {
  public:
    DummyProgressStep() : DummyProgressStep( -1 ) {}
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

class BadlyImpledRequest : public Request<DummyProgressStep> {
  public:
    BadlyImpledRequest( std::size_t capacity ) : Request( capacity ) {}
  private:
    UPS createProgressStep( std::size_t capacity) override {
      return nullptr;
    }
};

//implementace uz je v test/ProgressStep.cpp, coz brani linkovani, promyslet
std::ostream& operator<<( std::ostream& os, const DummyProgressStep& pS); /*{
  os << "Capacity: " << pS.getCapacity();
  return os;
}*/

template<typename PS>
std::ostream& operator<<( std::ostream& os, const Box<PS>& box ) {
  os << "Box with " << box.getProgressSteps().size() << " progress steps:\n";
  for ( auto pS : box.getProgressSteps() ) {
    os << "    --- " << pS << "\n";
  }
  return os;
}

template<typename PS>
void checkBoxInvariants( const Box<PS>& box, std::size_t expectedSize ) {
  BOOST_TEST( box.getTotalCapacity() > 0 );
  BOOST_TEST( box.getUsedCapacity() >= 0 );
  BOOST_TEST( box.getAvailableCapacity() >= 0 );
  BOOST_TEST( box.getUsedCapacity() + box.getAvailableCapacity() == box.getTotalCapacity() );

  BOOST_TEST( ( box.getStatus() == Box<PS>::Status::Empty ) == ( box.getUsedCapacity() == 0 ) );
  BOOST_TEST( ( box.getStatus() == Box<PS>::Status::HalfFull ) == ( box.getUsedCapacity() > 0 && box.getUsedCapacity() < box.getTotalCapacity() ) );
  BOOST_TEST( ( box.getStatus() == Box<PS>::Status::Full ) == ( box.getUsedCapacity() == box.getTotalCapacity() ) );

  BOOST_TEST( box.getProgressSteps().size() == expectedSize );
  std::size_t totalPSCapacity{ 0 };
  //??jak by tohle vypadalo pri pouziti algoritmu z (boost::) STL??
  for ( const PS& progressStep : box.getProgressSteps() ) {
    totalPSCapacity += progressStep.getCapacity();
  }
  BOOST_TEST( box.getUsedCapacity() == totalPSCapacity );
}

template<typename PS, typename SizeRange = boost::iterator_range<std::vector<std::size_t>::iterator>>
void createBoxProcessRequestsAndCheckInvariants( std::size_t startCapacity, std::size_t endCapacity, const SizeRange& requestSizesRange = SizeRange{} ) {
  Box<PS> box{ startCapacity };
  BOOST_TEST( box.getAvailableCapacity() == startCapacity );
  std::size_t curSize{ 0 };
  checkBoxInvariants( box, curSize );
  for ( std::size_t requestSize : requestSizesRange ) {
    DummyRequest request{ requestSize };
    box.processRequest( request );
    checkBoxInvariants( box, ++curSize );
  }
  BOOST_TEST( box.getAvailableCapacity() == endCapacity );
}

BOOST_AUTO_TEST_SUITE( BoxTest )
  //1) konstruktor: 0 a mensi haze vyjimku, vetsi nehaze
  //2) processRequest:
  //  a) throw, kdyz Request throws
  //  b) kdyz Request nethrows, prida ProgressStep s capacitou min( availZde, availVRequestu )
  //    invarianty jsou status, avail, used, total
  //    dalsi invarianty jsou begin, end, size, at
  //    checkProcessRequest vytvori Box<Request>, pak obecne nekolik Requestu a postupne je boxu posle
  //        overuje, ze vysledkem je Box ve spravnem stavu
  //3) operator== works
  //4) serialize() works
  BOOST_AUTO_TEST_CASE( test_1a_create_not_ok_with_non_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_CHECK_THROW( Box<DummyProgressStep>( 0 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create_ok_with_positive_value ) {
    //arrange
    //act
    //assert
    BOOST_REQUIRE_NO_THROW( Box<DummyProgressStep>( 12 ) );
    BOOST_REQUIRE_NO_THROW( Box<DummyProgressStep>( 421 ) );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create_invariants_hold ) {
    //arrange
    //act
    //assert
    createBoxProcessRequestsAndCheckInvariants<DummyProgressStep>( 1, 1 );
    createBoxProcessRequestsAndCheckInvariants<DummyProgressStep>( 2223, 2223 );
  }

  BOOST_AUTO_TEST_CASE( test_2a_use_capacity_throw_when_createProgressStep_returns_nullptr ) {
    //arrange
    Box<DummyProgressStep> box{ 1 };
    BadlyImpledRequest request{ 1 };
    //act
    //assert
    BOOST_REQUIRE_THROW( box.processRequest( request ), std::runtime_error );
  }

  BOOST_AUTO_TEST_CASE( test_2b_use_capacity_requested_when_available ) {
    //arrange
    //act
    //assert
    createBoxProcessRequestsAndCheckInvariants<DummyProgressStep>( 25, 15, std::vector<std::size_t>{ 10 } );
  }

  BOOST_AUTO_TEST_CASE( test_2c_use_capacity_available_when_requested_too_much ) {
    //arrange
    //act
    //assert
    createBoxProcessRequestsAndCheckInvariants<DummyProgressStep>( 25, 0, std::vector<std::size_t>{ 30 } );
  }

  BOOST_AUTO_TEST_CASE( test_2d_use_capacity_multiple_calls_when_enough_capacity_available ) {
    //arrange
    //act
    //assert
    createBoxProcessRequestsAndCheckInvariants<DummyProgressStep>( 25, 15, std::vector<std::size_t>{ 1, 2, 3, 4 } );
  }

  BOOST_AUTO_TEST_CASE( test_2e_use_capacity_throw_when_Full ) {
    //arrange
    Box<DummyProgressStep> box{ 2 };
    DummyRequest req1{ 2 }, req2{ 1 };
    //act
    //assert
    box.processRequest( req1 );
    BOOST_CHECK( box.getStatus() == Box<DummyProgressStep>::Status::Full );
    BOOST_REQUIRE_THROW( box.processRequest( req2 ), std::domain_error );
  }

  BOOST_AUTO_TEST_CASE( test_3a_equal_empty_identical_objects ) {
    //arrange
    Box<DummyProgressStep> box{ 1 };
    //act
    //assert
    BOOST_TEST( box == box );
  }

  BOOST_AUTO_TEST_CASE( test_3b_equal_different_empty_objects_same_initializer ) {
    //arrange
    Box<DummyProgressStep> box1{ 1 };
    Box<DummyProgressStep> box2{ 1 };
    //act
    //assert
    BOOST_TEST( box1 == box2 );
  }

  BOOST_AUTO_TEST_CASE( test_3c_equal_different_empty_objects_different_initializers ) {
    //arrange
    Box<DummyProgressStep> box1{ 1 };
    Box<DummyProgressStep> box2{ 198 };
    //act
    //assert
    BOOST_TEST( box1 != box2 );
  }

  BOOST_AUTO_TEST_CASE( test_3d_equal_non_empty_identical_objects ) {
    //arrange
    Box<DummyProgressStep> box{ 20 };
    DummyRequest req1{ 1 }, req2{ 2 };
    //act
    box.processRequest( req1 );
    box.processRequest( req2 );
    //assert
    BOOST_TEST( box == box );
  }

  BOOST_AUTO_TEST_CASE( test_3e_equal_non_empty_different_objects_same_steps ) {
    //arrange
    Box<DummyProgressStep> box1{ 20 }, box2{ 20 };
    DummyRequest req11{ 1 }, req12{ 2 }, req21{ 1 }, req22{ 2 };
    //act
    box1.processRequest( req11 );
    box1.processRequest( req12 );
    box2.processRequest( req21 );
    box2.processRequest( req22 );
    //assert
    BOOST_TEST( box1 == box2 );
  }

  BOOST_AUTO_TEST_CASE( test_3f_equal_non_empty_different_objects_different_steps ) {
    //arrange
    Box<DummyProgressStep> box1{ 20 }, box2{ 20 };
    DummyRequest req11{ 1 }, req12{ 2 }, req21{ 1 }, req22{ 2 };
    //act -- Note: box2 calls order is reversed
    box1.processRequest( req11 );
    box1.processRequest( req12 );
    box2.processRequest( req22 );
    box2.processRequest( req21 );
    //assert
    BOOST_TEST( box1 != box2 );
  }

  BOOST_AUTO_TEST_CASE( test_4a_serialize_PS_works ) {
    //arrange
    DummyProgressStep pSSource{ 12144 };
    DummyProgressStep pSTarget{ 1 };
    std::stringstream stream;
    //act
    boost::archive::text_oarchive oa{ stream };
    oa << pSSource;
    boost::archive::text_iarchive ia{ stream };
    ia >> pSTarget;
    //assert
    BOOST_TEST( pSSource == pSTarget );
  }

  BOOST_AUTO_TEST_CASE( test_4b_serialize_Box_works ) {
    //arrange
    Box<DummyProgressStep> boxSource{ 18 }, boxTarget{ 1 };
    DummyRequest req1{ 1 }, req2{ 2 }, req3{ 3 }, req4{ 4 };
    std::stringstream stream;
    //act
    boxSource.processRequest( req1 );
    boxSource.processRequest( req2 );
    boxSource.processRequest( req3 );
    boxSource.processRequest( req4 );
    boost::archive::text_oarchive oa{ stream };
    oa << boxSource;
    boost::archive::text_iarchive ia{ stream };
    ia >> boxTarget;
    //assert
    BOOST_TEST( boxSource == boxTarget );
  }
BOOST_AUTO_TEST_SUITE_END()
