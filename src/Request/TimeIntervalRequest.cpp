#include "TimeIntervalRequest.hpp"

namespace ashley::progress_tracker::request {
  TimeIntervalRequest::UPS
  TimeIntervalRequest::createProgressStep( std::size_t capacity ) {
    auto duration = endTimePoint_ - beginTimePoint_;
    auto beginDuration = duration * getUsedCapacity() / getTotalCapacity();
    auto endDuration = duration * ( getUsedCapacity() + capacity ) / getTotalCapacity();
    return std::make_unique<progress_step::TimeIntervalProgressStep>( capacity, beginTimePoint_ + beginDuration, beginTimePoint_ + endDuration );
  }
}
