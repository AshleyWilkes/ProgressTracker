#pragma once
#include "../RequestFactory.hpp"
#include "../Request/TimePointRequest.hpp"

namespace ashley::progress_tracker::request_factory {
  template<typename I>
  class TimePointRequestFactory : public RequestFactory<request::TimePointRequest, I> {
    public:
      static RF_ID id;

      std::unique_ptr<request::TimePointRequest> createRequest( const I& input ) const override;
  };

  template<typename I>
  RF_ID
  TimePointRequestFactory<I>::id = "TimePointRequestFactory";

  template<typename I>
  std::unique_ptr<request::TimePointRequest>
  TimePointRequestFactory<I>::createRequest( const I& input ) const {
    std::size_t capacity = input.template getAs<std::size_t>( "capacity" );
    boost::posix_time::ptime timePoint = input.template getAs<boost::posix_time::ptime>( "time" );
    return std::make_unique<request::TimePointRequest>( capacity, timePoint );
  }
}
