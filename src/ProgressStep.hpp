#pragma once
#include <cstddef>

namespace ashley::progress_tracker {
  class ProgressStep {
    public:
      ProgressStep( std::size_t capacity );
      std::size_t getCapacity() const;

      template<typename Archive>
      void serialize( Archive& ar, const unsigned int version );

      virtual ~ProgressStep() = 0;
    private:
      //radsi bych tu mel const, coz ale komplikuje serializaci,
      //nechavam takto
      std::size_t capacity_;

      friend bool operator==( const ProgressStep& lhs, const ProgressStep& rhs );
      friend bool operator!=( const ProgressStep& lhs, const ProgressStep& rhs );
  };
}
