#pragma once

#include "../InputMap.hpp"

namespace ashley::progress_tracker::input_map {
  class StringInputValue {
    public:
      StringInputValue( std::string value ) : value_{ value } {}

      template<typename T>
      T& as();
    private:
      std::string value_;
  };

  template<>
  std::string&
  StringInputValue::as<std::string>() {
    return value_;
  }

  class StringInputMap : public InputMap<std::string, StringInputValue> {
    public:
      StringInputMap( const std::map<std::string, std::string>& values ) : values_{} {
        for ( auto value : values ) {
          values_.insert({ value.first, StringInputValue{ value.second } });
        }
      }
    private:
      StringInputValue& get( const std::string& key ) override { return values_.at( key ); }
      std::map<std::string, StringInputValue> values_;
  };
}
