#pragma once
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/serialization/base_object.hpp"

#include "../ProgressStep.hpp"

namespace ashley::progress_tracker::progress_step {
  class TimePointProgressStep : public ProgressStep {
    public:
      TimePointProgressStep( std::size_t capacity, boost::posix_time::ptime timePoint ) : ProgressStep{ capacity }, timePoint_{ timePoint } {
        if ( timePoint.is_not_a_date_time() ) {
          throw std::domain_error{ "Invalid date time" };
        }
      }
      boost::posix_time::ptime getTimePoint() const { return timePoint_; }

      template<typename Archive>
      void serialize( Archive& ar, const unsigned int version );
    private:
      //radsi bych tu mel const, coz ale komplikuje serializaci,
      //nechavam takto
      boost::posix_time::ptime timePoint_;
      
      friend bool operator==( 
          const TimePointProgressStep& lhs,
          const TimePointProgressStep& rhs ) {
        return lhs.getCapacity() == rhs.getCapacity()
          && lhs.getTimePoint() == rhs.getTimePoint();
      }
      
      friend bool operator!=( 
          const TimePointProgressStep& lhs,
          const TimePointProgressStep& rhs ) {
        return !( lhs == rhs );
      }
  };

  template<typename Archive>
  void
  TimePointProgressStep::serialize( Archive& ar, const unsigned int version ) {
    ar & boost::serialization::base_object<ProgressStep>( *this );
    ar & timePoint_;
  }
}
