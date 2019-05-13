#include "FirstAvailableBoxSelector.hpp"

namespace ashley::progress_tracker::box_selector {

BoxSelector::BS_ID FirstAvailableBoxSelector::id = "FirstAvailableBoxSelector";

std::size_t
FirstAvailableBoxSelector::select( IntRange inputRange ) const {
  if ( inputRange.empty() ) {
    throw std::domain_error( "Select on empty range is not supported" );
  }
  return 0;
}

}
