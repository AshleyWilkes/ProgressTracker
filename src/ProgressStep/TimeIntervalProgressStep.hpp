#pragma once
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/serialization/base_object.hpp"

#include "../ProgressStep.hpp"

namespace ashley::progress_tracker::progress_step {
  class TimeIntervalProgressStep : public ProgressStep {
    public:
      TimeIntervalProgressStep( std::size_t capacity, boost::posix_time::ptime beginTimePoint, boost::posix_time::ptime endTimePoint ) : ProgressStep{ capacity }, beginTimePoint_{ beginTimePoint }, endTimePoint_{ endTimePoint } {
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
      boost::posix_time::ptime getBeginTimePoint() const { return beginTimePoint_; }
      boost::posix_time::ptime getEndTimePoint() const { return endTimePoint_; }
      boost::posix_time::time_duration getDuration() const { return endTimePoint_ - beginTimePoint_; }

      template<typename Archive>
      void serialize( Archive& ar, const unsigned int version );
    private:
      //radsi bych tu mel const, coz ale komplikuje serializaci,
      //nechavam takto
      boost::posix_time::ptime beginTimePoint_;
      boost::posix_time::ptime endTimePoint_;
      
      friend bool operator==( 
          const TimeIntervalProgressStep& lhs,
          const TimeIntervalProgressStep& rhs ) {
        return lhs.getCapacity() == rhs.getCapacity()
          && lhs.getBeginTimePoint() == rhs.getBeginTimePoint()
          && lhs.getEndTimePoint() == rhs.getEndTimePoint();
      }
      
      friend bool operator!=( 
          const TimeIntervalProgressStep& lhs,
          const TimeIntervalProgressStep& rhs ) {
        return !( lhs == rhs );
      }
  };

  template<typename Archive>
  void
  TimeIntervalProgressStep::serialize( Archive& ar, const unsigned int version ) {
    ar & boost::serialization::base_object<ProgressStep>( *this );
    ar & beginTimePoint_;
    ar & endTimePoint_;
  }
}
