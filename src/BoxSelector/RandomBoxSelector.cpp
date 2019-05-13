#include "RandomBoxSelector.hpp"
#include <random>

namespace ashley::progress_tracker::box_selector {

BoxSelector::BS_ID RandomBoxSelector::id = "RandomBoxSelector";

std::size_t
RandomBoxSelector::select( IntRange inputRange ) const {
  if ( inputRange.empty() ) {
    throw std::domain_error( "Select on empty range is not supported" );
  }

  std::random_device rd;
  std::mt19937 gen( rd() );
  std::uniform_int_distribution<> dis( 0, inputRange.size() - 1 );
  return dis( gen );
}

}
