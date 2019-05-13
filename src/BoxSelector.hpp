#pragma once
#include "boost/range/iterator_range_core.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>
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
      using BS_UPtr = std::unique_ptr<BoxSelector>;
      using BS_Range = boost::iterator_range<BS_Iter>;

      using IntIter = std::vector<int>::iterator;
      using IntRange = boost::iterator_range<IntIter>;

      static BS_Range getKnownBsIds();

      //static int select(IntIter first, IntIter last, BS_ID nbsId);
      static std::size_t select( IntRange inputRange, BS_ID nbsId );

      static BS_UPtr& create( BS_ID nbsId );

      //virtual int select( IntIter first, IntIter last ) const = 0;
      virtual std::size_t select( IntRange inputRange ) const = 0;

      virtual ~BoxSelector() = 0;
    private:
      static std::map<BS_ID, BS_UPtr> selectorsMap_;
  };
}
