#include "BoxSelector.hpp"
#include "BoxSelector/FirstAvailableBoxSelector.hpp"

namespace ashley::progress_tracker {

BoxSelector::BS_Range
BoxSelector::getKnownBsIds() {
  using namespace box_selector;
  static std::vector<BoxSelector::BS_ID> knownBsIds {
    FirstAvailableBoxSelector::id
  };
  return BoxSelector::BS_Range( knownBsIds );
}

std::size_t
BoxSelector::select( BoxSelector::IntRange inputRange, BS_ID nbsId ) {
  return create( nbsId )->select( inputRange );
}

BoxSelector::BS_UPtr&
BoxSelector:: create( BS_ID nbsId ) {
  using namespace box_selector;
  if ( selectorsMap_.find( nbsId ) == selectorsMap_.end() ) {
    if ( nbsId == FirstAvailableBoxSelector::id ) {
      selectorsMap_.insert({ nbsId, std::make_unique<FirstAvailableBoxSelector>() });
    } else {
      throw std::invalid_argument( "BoxSelector with id " + nbsId + " is not supported!" );
    }
  }
  auto resultIter = selectorsMap_.find( nbsId );
  return resultIter->second;
}

BoxSelector::~BoxSelector() = default;

std::map<BoxSelector::BS_ID, BoxSelector::BS_UPtr> BoxSelector::selectorsMap_;
}
