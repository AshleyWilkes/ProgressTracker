#include "TimeIntervalRequestFactory.hpp"
#include "../InputMap/CliInputMap.hpp"
#include "../InputMap/StringInputMap.hpp"

namespace ashley::progress_tracker::request_factory {

  template<>
  RF_Range getKnownFactoryIds<request::TimeIntervalRequest>() {
    static std::vector<RF_ID> ids = {
      TimeIntervalRequestFactory<input_map::StringInputMap>::id
    };
    return RF_Range{ ids };
  }

  template<>
  std::unique_ptr<RequestFactory<request::TimeIntervalRequest, input_map::StringInputMap>>
  RequestFactory<request::TimeIntervalRequest, input_map::StringInputMap>::createFactory( RF_ID rfId ) {
    if ( rfId == TimeIntervalRequestFactory<input_map::StringInputMap>::id ) {
      return std::make_unique<TimeIntervalRequestFactory<input_map::StringInputMap>>();
    } else {
      throw std::invalid_argument( "RequestFactory with id " + rfId + " is not supported!" );
    }
  }

  template<>
  std::unique_ptr<RequestFactory<request::TimeIntervalRequest, input_map::CliInputMap>>
  RequestFactory<request::TimeIntervalRequest, input_map::CliInputMap>::createFactory( RF_ID rfId ) {
    if ( rfId == TimeIntervalRequestFactory<input_map::CliInputMap>::id ) {
      return std::make_unique<TimeIntervalRequestFactory<input_map::CliInputMap>>();
    } else {
      throw std::invalid_argument( "RequestFactory with id " + rfId + " is not supported!" );
    }
  }
}
