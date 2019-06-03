#include "StringInputMap.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"

namespace ashley::progress_tracker::input_map {

template<>
std::string
StringInputValue::as<std::string>() const {
  return value_;
}

template<>
int
StringInputValue::as<int>() const {
  return std::stoi( value_ );
}

template<>
std::size_t
StringInputValue::as<std::size_t>() const {
  return (std::size_t) std::stoi( value_ );
}

template<>
boost::posix_time::ptime
StringInputValue::as<boost::posix_time::ptime>() const {
  return boost::posix_time::time_from_string( value_ );
}

}
