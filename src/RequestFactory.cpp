#include "RequestFactory.hpp"
#include "RequestFactory/TimePointRequestFactory.hpp"

namespace ashley::progress_tracker::request_factory {

  RF_Range getKnownFactoryIds() {
    static std::vector<RF_ID> knownFactoryIds {
      "TimePointRequestFactory"
    };
    return RF_Range( knownFactoryIds );
  }

}
