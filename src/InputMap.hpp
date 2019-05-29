#pragma once

namespace ashley::progress_tracker::input_map {
  template<typename K, typename I>//K -- key, I -- internal value implementation
  class InputMap {
    public:
      template<typename T>
      T& getAs( const K& key ) { return get( key ).template as<T>(); }

      virtual ~InputMap() = 0;
    private:
      virtual I& get( const K& key ) = 0;
  };

  template<typename K, typename I>
  InputMap<K, I>::~InputMap() = default;
}
