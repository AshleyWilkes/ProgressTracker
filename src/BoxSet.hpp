#pragma once
#include "Box.hpp"

#include "boost/range/adaptor/filtered.hpp"
#include "boost/range/adaptor/indirected.hpp"
#include "boost/serialization/unique_ptr.hpp"

namespace ashley::progress_tracker {
  //BoxSetu umoznim, aby byl prazdny, neni to proti nicemu
  //naopak mu zatim neumoznim, aby byl heterogenni, jinak receno
  //  vsechny boxy v setu budou stejneho typu (podporuji stejny
  //  ProgressStep a stejny Cont, dokud tento neni zrusen)
  template<typename PS>
  class BoxSet {
    public:
      using SizeIter = std::vector<std::size_t>::iterator;
      using SizeRange = boost::iterator_range<SizeIter>;

      using BoxVector = typename std::vector<Box<PS>>;
      using BoxIter = typename BoxVector::iterator;
      using BoxRange = boost::iterator_range<BoxIter>;

      BoxSet() {}
      BoxSet( const SizeRange& sizesRange );

      auto getBoxes() const { return getBoxesWithStatuses( { Box<PS>::Status::Empty, Box<PS>::Status::HalfFull, Box<PS>::Status::Full } ); }
      auto getBoxesWithStatus( typename Box<PS>::Status status ) const { return getBoxesWithStatuses( { status } ); }
      auto getBoxesWithStatuses( const std::vector<typename Box<PS>::Status>& statuses ) const;

      std::size_t countBoxes() const { return boost::size( getBoxes() ); }
      std::size_t countBoxesWithStatus( typename Box<PS>::Status status ) const { return boost::size( getBoxesWithStatus( status ) ); }
      std::size_t countBoxesWithStatuses( const std::vector<typename Box<PS>::Status>& statuses ) const { return boost::size( getBoxesWithStatuses( statuses ) ); }

      template<typename Archive>
      void serialize( Archive& ar, const unsigned int version );
    private:
      std::vector<std::unique_ptr<Box<PS>>> boxes_;
  };

  template<typename PS>
  BoxSet<PS>::BoxSet( const SizeRange& sizesRange ) {
    for ( std::size_t size : sizesRange ) {
      boxes_.push_back( std::make_unique<Box<PS>>( size ) );
    }
  }

  template<typename PS>
  auto
  BoxSet<PS>::getBoxesWithStatuses( const std::vector<typename Box<PS>::Status>& statuses ) const {
    using namespace boost::adaptors;
    return boxes_
      | indirected
      | filtered( [statuses]( const Box<PS>& box ) {
            return std::find( statuses.begin(), statuses.end(), box.getStatus() ) != statuses.end();
          });
  }

  template<typename PS>
  template<typename Archive>
  void
  BoxSet<PS>::serialize( Archive& ar, const unsigned int version ) {
    ar & boxes_;
  }

  template<typename PS>
  bool operator==( const BoxSet<PS>& lhs, const BoxSet<PS>& rhs ) {
    return lhs.getBoxes() == rhs.getBoxes();
  }

  template<typename PS>
  bool operator!=( const BoxSet<PS>& lhs, const BoxSet<PS>& rhs ) {
    return ! ( lhs == rhs );
  }
}
