#pragma once
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include "../Request.hpp"
#include "../ProgressStep/TimeIntervalProgressStep.hpp"

namespace ashley::progress_tracker::request {
  class TimeIntervalRequest : public Request<progress_step::TimeIntervalProgressStep> {
    public:
      TimeIntervalRequest( std::size_t capacity, boost::posix_time::ptime beginTimePoint, boost::posix_time::ptime endTimePoint ) : Request{ capacity }, beginTimePoint_{ beginTimePoint }, endTimePoint_{ endTimePoint } {
        if ( beginTimePoint.is_not_a_date_time() ) {
          throw std::domain_error{ "Invalid date time (beginTimePoint)" };
        }
        if ( endTimePoint.is_not_a_date_time() ) {
          throw std::domain_error{ "Invalid date time (endTimePoint)" };
        }
        if ( beginTimePoint > endTimePoint ) {
          throw std::domain_error{ "Invalid interval" };
        }
      }
    private:
      UPS createProgressStep( std::size_t capacity ) override;
      const boost::posix_time::ptime beginTimePoint_;
      const boost::posix_time::ptime endTimePoint_;
  };
}
