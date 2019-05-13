#pragma once
#include "../BoxSelector.hpp"

namespace ashley::progress_tracker::box_selector {
  class RandomBoxSelector : public BoxSelector {
    public:
      static BS_ID id;

      virtual std::size_t select( IntRange inputRange ) const override;
  };
}
