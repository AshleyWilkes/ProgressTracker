#pragma once
#include "Request.hpp"

#include "InputMap.hpp"

#include "boost/program_options.hpp"
#include "boost/range/sub_range.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ashley::progress_tracker::request_factory {
  using RF_ID = std::string;
  using RF_Range = boost::sub_range<std::vector<RF_ID>>;
  template<typename R, typename I>
  class RequestFactory {
    public:
      using RF_UPtr = std::unique_ptr<RequestFactory<R, I>>;

      virtual std::unique_ptr<R> createRequest( const I& input ) const = 0;

      virtual ~RequestFactory() = 0;

      static std::unique_ptr<RequestFactory<R, I>> createFactory( RF_ID rfId );
    private:
      static std::map<RF_ID, RF_UPtr> factoriesMap_;
  };

  template<typename R>
  RF_Range getKnownFactoryIds();

  template<typename R, typename I>
  std::unique_ptr<RequestFactory<R, I>>
  createFactory( RF_ID rfId ) {
    auto rfIds = getKnownFactoryIds<R>();
    if ( rfIds.find( rfId ) != rfIds.end() ) {
      std::unique_ptr<RequestFactory<R, I>> result = RequestFactory<R, I>::createFactory( rfId );
      if ( ! result ) {
        throw std::invalid_argument( "RequestFactory with id " + rfId + " should have been created, but nullptr was returned instead!" );
      }
      return result;
    } else {
      throw std::invalid_argument( "RequestFactory with id " + rfId + " is not supported for the given types!" );
    }
  }

  template<typename R, typename I>
  std::unique_ptr<R>
  createRequest( const I& input, RF_ID rfId ) {
    return RequestFactory<R, I>::createFactory( rfId ).createRequest( input );
  }

  template<typename R, typename I>
  RequestFactory<R, I>::~RequestFactory() = default;
}
