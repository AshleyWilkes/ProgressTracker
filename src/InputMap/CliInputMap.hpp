#pragma once

#include "../InputMap.hpp"

#include <boost/program_options.hpp>

namespace ashley::progress_tracker::input_map {
  using namespace boost::program_options;

  class CliInputMap : public InputMap<std::string, variable_value> {
    public:
      CliInputMap( const variables_map& input_map ) : input_map_{ input_map } {}
    private:
      variable_value& get( const std::string& key ) override { return input_map_.at( key ); }
      variables_map input_map_;
  };
}
