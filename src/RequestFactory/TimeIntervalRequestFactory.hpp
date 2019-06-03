#pragma once
#include "../RequestFactory.hpp"
#include "../Request/TimeIntervalRequest.hpp"

namespace ashley::progress_tracker::request_factory {
  template<typename I>
  class TimeIntervalRequestFactory : public RequestFactory<request::TimeIntervalRequest, I> {
    public:
      static RF_ID id;

      std::unique_ptr<request::TimeIntervalRequest> createRequest( const I& input ) const override;
  };

  template<typename I>
  RF_ID
  TimeIntervalRequestFactory<I>::id = "TimeIntervalRequestFactory";

  template<typename I>
  std::unique_ptr<request::TimeIntervalRequest>
  TimeIntervalRequestFactory<I>::createRequest( const I& input ) const {
    std::size_t capacity = input.template getAs<std::size_t>( "capacity" );
    boost::posix_time::ptime beginTimePoint = input.template getAs<boost::posix_time::ptime>( "begin_time" );
    boost::posix_time::ptime endTimePoint = input.template getAs<boost::posix_time::ptime>( "end_time" );
    return std::make_unique<request::TimeIntervalRequest>( capacity, beginTimePoint, endTimePoint );
  }
}
