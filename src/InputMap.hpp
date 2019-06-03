#pragma once
#include <stdexcept>
#include "boost/any.hpp"

#include <string>

namespace ashley::progress_tracker::input_map {
  template<typename T>
  std::string
  inline to_string( const T& val ) {
    return std::to_string( val );
  }

  template<>
  std::string
  inline to_string<std::string>( const std::string& val ) {
    return val;
  }

  template<typename K, typename I>//K -- key, I -- internal value implementation
  class InputMap {
    public:
      template<typename T>
      T getAs( const K& key ) const;

      virtual ~InputMap() = 0;
    private:
      virtual const I& get( const K& key ) const = 0;
  };

  template<typename K, typename I>
  template<typename T>
  T
  inline InputMap<K, I>::getAs( const K& key ) const {
    try {
      auto valueObject = get( key );
      auto castValue = valueObject.template as<T>();
      return castValue;
    } catch ( const std::out_of_range& ) {
      throw std::out_of_range{ "Key '" + to_string( key ) + "' not found in the input map!" };
    } catch ( const std::invalid_argument& ) {
      throw std::invalid_argument{ "Value of key '" + to_string( key ) + "' couldn't be converted to requested type!" };
    } catch ( const boost::bad_any_cast& ) {
      throw std::invalid_argument{ "Value of key '" + to_string( key ) + "' couldn't be converted to requested type!" };
    }
  } 

  template<typename K, typename I>
  InputMap<K, I>::~InputMap() = default;
}
