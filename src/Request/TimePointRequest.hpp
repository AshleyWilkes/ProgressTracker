#pragma once
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include "../Request.hpp"
#include "../ProgressStep/TimePointProgressStep.hpp"

namespace ashley::progress_tracker::request {
  class TimePointRequest : public Request<progress_step::TimePointProgressStep> {
    public:
      TimePointRequest( std::size_t capacity, boost::posix_time::ptime timePoint ) : Request{ capacity }, timePoint_{ timePoint } {
        if ( timePoint.is_not_a_date_time() ) {
          throw std::domain_error{ "Invalid date time" };
        }
      }
    private:
      UPS createProgressStep( std::size_t capacity ) override;
      const boost::posix_time::ptime timePoint_;
  };
}
