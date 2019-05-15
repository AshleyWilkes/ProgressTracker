#pragma once
#include <memory>

namespace ashley::progress_tracker {
  template<typename PS>//PS -- ProgressStep
  class Request {
    public:
      using UPS = std::unique_ptr<PS>;

      Request( std::size_t totalCapacity );
      UPS useCapacity( std::size_t capacity );

      bool isFullyProcessed() const { return totalCapacity_ == usedCapacity_; }
      std::size_t getTotalCapacity() const { return totalCapacity_; }
      std::size_t getUsedCapacity() const { return usedCapacity_; }
      std::size_t getAvailableCapacity() const { return totalCapacity_ - usedCapacity_; }

      virtual ~Request() = 0;
    private:
      //this is called by useCapacity to create a new ProgressStep;
      //implementations may suppose the argument to be at least 1
      //and at most the available capacity;
      //implementation may choose to create a ProgressStep with different capacity
      //than is the provided argument;
      //implementation may choose to throw an exception which would be propagated out of useCapacity;
      //if an excpetion is not thrown, Request will add the capacity of the returned ProgressStep
      //to its usedCapacity_, so implementation should change its internal state accordingly;
      virtual UPS createProgressStep( std::size_t capacity ) = 0;

      const std::size_t totalCapacity_;
      std::size_t usedCapacity_;
  };

  template<typename PS> Request<PS>::Request( std::size_t totalCapacity ) : totalCapacity_{ totalCapacity }, usedCapacity_{ 0 } {
    if ( totalCapacity < 1 ) {
      throw std::domain_error{ "Invalid total capacity: " + std::to_string( totalCapacity ) };
    }
  }

  template<typename PS>
  typename Request<PS>::UPS
  Request<PS>::useCapacity( std::size_t capacity ) {
    if ( capacity < 1 ) {
      throw std::domain_error{ "Invalid requested capacity: " + std::to_string( capacity ) };
    }
    //urcit, jakou kapacitu skutecne pouziju
    if ( capacity > totalCapacity_ - usedCapacity_ ) {
      capacity = totalCapacity_ - usedCapacity_;
    }
    //vytvorit vysledek
    auto result = createProgressStep( capacity );
    //pridat skutecne pouzitou kapacitu k usedCapacity_
    usedCapacity_ += result->getCapacity();
    //vratit vysledek
    return result;
  }

  template<typename PS>
  Request<PS>::~Request() = default;
}
