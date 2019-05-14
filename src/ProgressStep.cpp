#include "ProgressStep.hpp"
#include <stdexcept>

namespace ashley::progress_tracker {

ProgressStep::ProgressStep( std::size_t capacity ) : capacity_{ capacity } {
  if ( capacity < 1 ) {
    throw std::domain_error{ "Invalid initial capacity: " + std::to_string( capacity ) };
  }
}

std::size_t
ProgressStep::getCapacity() const {
  return capacity_;
}

ProgressStep::~ProgressStep() = default;

bool
operator==( const ProgressStep& lhs, const ProgressStep& rhs ) {
  return lhs.capacity_ == rhs.capacity_;
}

bool
operator!=( const ProgressStep& lhs, const ProgressStep& rhs ) {
  return ! ( lhs == rhs );
}

}
