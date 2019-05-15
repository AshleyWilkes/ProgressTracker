#pragma once
#include <memory>
namespace ashley::progress_tracker {
  template<typename PS>//PS -- ProgressStep
  class Request {
    public:
      using UPS = std::unique_ptr<PS>;

      Request( std::size_t totalCapacity );
      UPS useCapacity( std::size_t capacity );

      bool isFullyProcessed() const;
      std::size_t getTotalCapacity() const;
      std::size_t getUsedCapacity() const;
      std::size_t getAvailableCapacity() const;

      virtual ~Request() = 0;
    private:
      virtual UPS createProgressStep( std::size_t capacity ) = 0;

      const std::size_t totalCapacity_;
      const std::size_t usedCapacity_;
  };

  template<typename PS> Request<PS>::Request( std::size_t totalCapacity ) : totalCapacity_{ totalCapacity }, usedCapacity_{ 0 } {
    if ( totalCapacity < 1 ) {
      throw std::domain_error{ "Invalid total capacity: " + std::to_string( totalCapacity ) };
    }
  }

  template<typename PS>
  Request<PS>::~Request() = default;
}
