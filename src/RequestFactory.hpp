#pragma once
#include "Request.hpp"

#include "boost/program_options.hpp"
#include "boost/range/sub_range.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ashley::progress_tracker::request_factory {
  using RF_ID = std::string;
  using RF_Range = boost::sub_range<std::vector<RF_ID>>;
  using Input = boost::program_options::variables_map;

  RF_Range getKnownFactoryIds();

  template<typename R>
  class RequestFactory {
    public:
      using RF_UPtr = std::unique_ptr<RequestFactory<R>>;

      static std::unique_ptr<R> createRequest( const Input& input, RF_ID rfId );

      static RequestFactory<R>& createFactory( RF_ID rfId );

      virtual std::unique_ptr<R> createRequest( const Input& input ) const = 0;

      virtual ~RequestFactory() = 0;
    private:
      static std::map<RF_ID, RF_UPtr> factoriesMap_;
  };

  template<typename R>
  typename std::unique_ptr<R>
  RequestFactory<R>::createRequest( const Input& input, RF_ID rfId ) {
    return createFactory( rfId )->createRequest( input );
  }

  template<typename R>
  RequestFactory<R>&
  RequestFactory<R>::createFactory( RF_ID rfId ) {
    throw std::invalid_argument( "RequestFactory with id " + rfId + " is not supported!" );
  }
}
