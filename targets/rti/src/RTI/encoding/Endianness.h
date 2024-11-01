#pragma once
#include <cstring>

namespace tt {
  /// If this is a little endian system, this will simply memcpy, otherwise it will swap the endianness.
  /// \param dest
  /// \param source
  /// \param length
  inline void memcpyle(char* dest, const char* source, size_t length) {
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        for(size_t i = 0 ; i < length ; ++i) {
            dest[length - 1 - i] = source[i];
        }
#else
    memcpy(dest, source, length);
#endif
  }

  /// If this is a big endian system, this will simply memcpy, otherwise it will swap the endianness.
  /// \param dest
  /// \param source
  /// \param length
  inline void memcpybe(char* dest, const char* source, size_t length) {
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        memcpy( dest, source, length );
#else
    for (size_t i = 0; i < length; ++i) {
      dest[length - 1 - i] = source[i];
    }
#endif
  }
}
