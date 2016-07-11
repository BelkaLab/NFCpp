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
#define NFCPP_VERSION_HEX ((NFCPP_VERSION_MAJOR << 16) |  \
                           (NFCPP_VERSION_MINOR << 8) |  \
                           (NFCPP_VERSION_PATCH << 0))

/* Evaluates to True if the version is greater than @major, @minor and @patch
*/
#define NFCPP_VERSION_CHECK(major,minor,patch)      \
    (NFCPP_VERSION_MAJOR > (major) ||               \
     (NFCPP_VERSION_MAJOR == (major) &&             \
      NFCPP_VERSION_MINOR > (minor)) ||             \
     (NFCPP_VERSION_MAJOR == (major) &&             \
      NFCPP_VERSION_MINOR == (minor) &&             \
      NFCPP_VERSION_PATCH >= (patch)))

#endif /* NFCPP_VERSION_H */
