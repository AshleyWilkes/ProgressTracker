#include "WeightedRandomBoxSelector.hpp"
#include <random>

namespace ashley::progress_tracker::box_selector {

BoxSelector::BS_ID WeightedRandomBoxSelector::id = "WeightedRandomBoxSelector";

std::size_t
WeightedRandomBoxSelector::select( IntRange inputRange ) const {
  if ( inputRange.empty() ) {
    throw std::domain_error( "Select on empty range is not supported" );
  }

  std::random_device rd;
  std::mt19937 gen( rd() );
  std::discrete_distribution<> dis( inputRange.begin(), inputRange.end() );
  return dis( gen );
}

}
