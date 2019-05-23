#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/base_object.hpp"

#include "Box.hpp"
#include "BoxSet.hpp"
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

template<typename PS>
std::ostream& operator<<( std::ostream& os, const Box<PS>& box );

template<typename PS>
std::ostream& operator<<( std::ostream& os, const BoxSet<PS>& boxSet ) {
  os << "\n******** BoxSet *******\n" << boxSet.getBoxes() << "\n********************\n\n";
  return os;
}

template<typename PS>
void checkInvariants( const BoxSet<PS>& boxSet, std::size_t fulls, std::size_t halfFulls, std::size_t empties ) {
  std::size_t total = fulls + halfFulls + empties;
  BOOST_TEST( boxSet.countBoxes() == total );

  BOOST_TEST( boxSet.countBoxesWithStatus( Box<PS>::Status::Full ) == fulls );
  BOOST_TEST( boxSet.countBoxesWithStatus( Box<PS>::Status::HalfFull ) == halfFulls );
  BOOST_TEST( boxSet.countBoxesWithStatus( Box<PS>::Status::Empty ) == empties );

  BOOST_TEST( boxSet.countBoxesWithStatuses( { Box<PS>::Status::HalfFull, Box<PS>::Status::Empty } ) == halfFulls + empties );
  BOOST_TEST( boxSet.countBoxesWithStatuses( { Box<PS>::Status::Full, Box<PS>::Status::Empty } ) == fulls + empties );
  BOOST_TEST( boxSet.countBoxesWithStatuses( { Box<PS>::Status::Full, Box<PS>::Status::HalfFull } ) == fulls + halfFulls );
}

template<typename PS>
void createAndRunRequest( BoxSet<PS>& boxSet, std::size_t requestCapacity ) {
  DummyRequest request{ requestCapacity };
  for (auto& box : boxSet.getBoxes() ) {
    if ( ! request.isFullyProcessed() ) {
      box.processRequest( request );
    }
  }
}

BOOST_AUTO_TEST_SUITE( BoxSetTest )
  //1) konstruktor
  //2) boxesWithStatus
  //3) equal
  //4) serialize
  BOOST_AUTO_TEST_CASE( test_1a_create ) {
    std::vector<std::size_t> sizes{ 1 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    checkInvariants( boxSet, 0, 0, 1 );
  }

  BOOST_AUTO_TEST_CASE( test_1b_create ) {
    std::vector<std::size_t> sizes{ 15, 7 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    checkInvariants( boxSet, 0, 0, 2 );
  }

  BOOST_AUTO_TEST_CASE( test_1c_create ) {
    std::vector<std::size_t> sizes{ 1, 2, 3, 4 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    checkInvariants( boxSet, 0, 0, 4 );
  }

  BOOST_AUTO_TEST_CASE( test_2a_statuses_after_full ) {
    std::vector<std::size_t> sizes{ 1, 2, 3, 4 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    createAndRunRequest( boxSet, 10 );
    checkInvariants( boxSet, 4, 0, 0 );
  }

  BOOST_AUTO_TEST_CASE( test_2b_statuses_after_not_full ) {
    std::vector<std::size_t> sizes{ 3, 2, 1 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    createAndRunRequest( boxSet, 4 );
    checkInvariants( boxSet, 1, 1, 1 );
  }

  BOOST_AUTO_TEST_CASE( test_3a_equal_empty_identical_objects ) {
    std::vector<std::size_t> sizes{ 1, 2, 3, 4 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    BOOST_TEST( boxSet == boxSet );
  }

  BOOST_AUTO_TEST_CASE( test_3b_equal_empty_different_objects_same_initializer ) {
    std::vector<std::size_t> sizes1{ 1, 3, 5, 7 };
    BoxSet<DummyProgressStep> boxSet1{ sizes1 };
    BoxSet<DummyProgressStep> boxSet2{ sizes1 };
    BOOST_TEST( boxSet1 == boxSet2 );
  }

  BOOST_AUTO_TEST_CASE( test_3c_equal_empty_different_objects_different_initializer ) {
    std::vector<std::size_t> sizes1{ 1, 3, 5, 7 };
    BoxSet<DummyProgressStep> boxSet1{ sizes1 };
    std::vector<std::size_t> sizes2{ 2, 4, 6, 8 };
    BoxSet<DummyProgressStep> boxSet2{ sizes2 };
    BOOST_TEST( boxSet1 != boxSet2 );
  }

  BOOST_AUTO_TEST_CASE( test_3d_equal_non_empty_identical_objects ) {
    std::vector<std::size_t> sizes{ 5, 6, 3, 4 };
    BoxSet<DummyProgressStep> boxSet{ sizes };
    createAndRunRequest( boxSet, 10 );
    BOOST_TEST( boxSet == boxSet );
  }

  BOOST_AUTO_TEST_CASE( test_3e_equal_non_empty_same_steps ) {
    std::vector<std::size_t> sizes{ 5, 6, 3, 4 };
    BoxSet<DummyProgressStep> boxSet1{ sizes };
    createAndRunRequest( boxSet1, 10 );
    BoxSet<DummyProgressStep> boxSet2{ sizes };
    createAndRunRequest( boxSet2, 10 );
    BOOST_TEST( boxSet1 == boxSet2 );
  }

  BOOST_AUTO_TEST_CASE( test_3f_equal_non_empty_different_steps ) {
    std::vector<std::size_t> sizes{ 5, 6, 3, 4 };
    BoxSet<DummyProgressStep> boxSet1{ sizes };
    createAndRunRequest( boxSet1, 10 );
    BoxSet<DummyProgressStep> boxSet2{ sizes };
    createAndRunRequest( boxSet2, 15 );
    BOOST_TEST( boxSet1 != boxSet2 );
  }

  BOOST_AUTO_TEST_CASE( test_4a_serialize_Box_works ) {
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

  BOOST_AUTO_TEST_CASE( test_4b_serialize_BoxSet_works ) {
    //arrange
    std::vector<std::size_t> sizes{ 4, 3, 2, 1 };
    BoxSet<DummyProgressStep> boxSetSource{ sizes }, boxSetTarget{};
    std::stringstream stream;
    //act
    createAndRunRequest( boxSetSource, 6 );
    boost::archive::text_oarchive oa{ stream };
    oa << boxSetSource;
    boost::archive::text_iarchive ia{ stream };
    ia >> boxSetTarget;
    //assert
    BOOST_TEST( boxSetSource == boxSetTarget );
  }
BOOST_AUTO_TEST_SUITE_END()
