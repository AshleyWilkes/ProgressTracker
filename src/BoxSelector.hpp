#include "boost/range/iterator_range_core.hpp"
#include <string>
//Abstract class. Provides template method select, which clients may
//use to select an element from a range.
//
//The remaining, static methods provide access to known implementations.
//No automatic lookup is provided, the implementations list must be kept
//up-to-date manually.
namespace ashley::progress_tracker {
  class BoxSelector {
    public:
      using BS_ID = std::string;
      using BS_Iter = std::vector<std::string>::iterator;
      using IntIter = std::vector<int>::iterator;

      static boost::iterator_range<BS_Iter> getKnownBsIds();

      //static int select(IntIter first, IntIter last, BS_ID nbsId);
      static std::size_t select( boost::iterator_range<IntIter> inputRange, BS_ID nbsId );

      static std::unique_ptr<BoxSelector> create( BS_ID nbsId );

      //virtual int select( IntIter first, IntIter last ) const = 0;
      virtual std::size_t select( boost::iterator_range<IntIter> inputRange ) const = 0;

      virtual ~BoxSelector() = 0;
  };
}
