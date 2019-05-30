#include "RequestFactory.hpp"

namespace ashley::progress_tracker::request_factory {

  RF_Range getKnownFactoryIds() {
    static std::vector<RF_ID> knownFactoryIds {
    };
    return RF_Range( knownFactoryIds );
  }

}
