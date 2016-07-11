#pragma once

#ifndef NFCPP_VERSION_H
#define NFCPP_VERSION_H

/* The major version, (1, if %NFCPP_VERSION is 1.2.3) */
#define NFCPP_VERSION_MAJOR 0

/* The minor version (2, if %NFCPP_VERSION is 1.2.3) */
#define NFCPP_VERSION_MINOR 1

/* The patch version (3, if %NFCPP_VERSION is 1.2.3) */
#define NFCPP_VERSION_PATCH 0

/* The full version, like 1.2.3 */
#define NFCPP_VERSION        "0.1.0"

/* Numerically encoded version, like 0x010203 */
#define NFCPP_VERSION_HEX ((LIBMODBUS_VERSION_MAJOR << 24) |  \
                           (LIBMODBUS_VERSION_MINOR << 16) |  \
                           (LIBMODBUS_VERSION_MICRO << 8))

/* Evaluates to True if the version is greater than @major, @minor and @patch
*/
#define NFCPP_VERSION_CHECK(major,minor,patch)      \
    (LIBMODBUS_VERSION_MAJOR > (major) ||               \
     (LIBMODBUS_VERSION_MAJOR == (major) &&             \
      LIBMODBUS_VERSION_MINOR > (minor)) ||             \
     (LIBMODBUS_VERSION_MAJOR == (major) &&             \
      LIBMODBUS_VERSION_MINOR == (minor) &&             \
      LIBMODBUS_VERSION_MICRO >= (patch)))

#endif /* NFCPP_VERSION_H */
