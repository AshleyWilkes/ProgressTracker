#pragma once
#include <memory>
namespace ashley::progress_tracker {
  template<typename PS>//PS -- ProgressStep
  class Request {
    public:
      using UPS = std::unique_ptr<PS>;

      Request( std::size_t totalCapacity );

      virtual UPS useCapacity( std::size_t capacity ) = 0;

      virtual bool isFullyProcessed() const = 0;

      virtual ~Request() = 0;
    private:
      const std::size_t totalCapacity_;
  };

  template<typename PS> Request<PS>::Request( std::size_t totalCapacity ) : totalCapacity_{ totalCapacity } {
    if ( totalCapacity < 1 ) {
      throw std::domain_error{ "Invalid total capacity: " + std::to_string( totalCapacity ) };
    }
  }

  template<typename PS>
  Request<PS>::~Request() = default;
}
