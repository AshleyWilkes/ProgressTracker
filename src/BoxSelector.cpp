#include "BoxSelector.hpp"

namespace ashley::progress_tracker {

BoxSelector::BS_Range
BoxSelector::getKnownBsIds() {
  static std::vector<BoxSelector::BS_ID> knownBsIds {
  };
  return BoxSelector::BS_Range( knownBsIds );
}

std::size_t
BoxSelector::select( BoxSelector::IntRange inputRange, BS_ID nbsId ) {
  return create( nbsId )->select( inputRange );
}

BoxSelector::BS_UPtr&
BoxSelector:: create( BS_ID nbsId ) {
  throw std::invalid_argument( "BoxSelector with id " + nbsId + " is not supported!" );
}

BoxSelector::~BoxSelector() = default;

std::map<BoxSelector::BS_ID, BoxSelector::BS_UPtr> BoxSelector::selectorsMap_;
}
