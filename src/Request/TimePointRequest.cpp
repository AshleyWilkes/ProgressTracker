#include "TimePointRequest.hpp"

namespace ashley::progress_tracker::request {
  TimePointRequest::UPS
  TimePointRequest::createProgressStep( std::size_t capacity ) {
    return std::make_unique<progress_step::TimePointProgressStep>( capacity, timePoint_ );
  }
}
