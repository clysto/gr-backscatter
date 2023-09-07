find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_BACKSCATTER gnuradio-backscatter)

FIND_PATH(
    GR_BACKSCATTER_INCLUDE_DIRS
    NAMES gnuradio/backscatter/api.h
    HINTS $ENV{BACKSCATTER_DIR}/include
        ${PC_BACKSCATTER_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_BACKSCATTER_LIBRARIES
    NAMES gnuradio-backscatter
    HINTS $ENV{BACKSCATTER_DIR}/lib
        ${PC_BACKSCATTER_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-backscatterTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_BACKSCATTER DEFAULT_MSG GR_BACKSCATTER_LIBRARIES GR_BACKSCATTER_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_BACKSCATTER_LIBRARIES GR_BACKSCATTER_INCLUDE_DIRS)
