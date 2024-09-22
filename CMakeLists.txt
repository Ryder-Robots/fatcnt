cmake_minimum_required(VERSION 3.25)
project(fatctl DESCRIPTION "MVP for Arduino Nano MC" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "fatctl")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")
set(PUBLIC_HEADER include/fatctl.h)
set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

# set(fatctl_SOURCES 
#     src/fatctl/fatctl.cpp
# )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")

include_directories(include src)
pkg_search_module(DLIB REQUIRED dlib-1)

# Create library outline
# add_library(fatctl SHARED  ${fatctl_SOURCES} )
# target_link_libraries(fatctl /usr/lib/libwiringPi.so)
# target_link_libraries(fatctl /lib/aarch64-linux-gnu/libusb-1.0.so)

add_executable(fatctl src/fatctl/fatctl.cpp)
target_link_libraries(fatctl /usr/lib/libwiringPi.so)
target_link_libraries(fatctl /lib/aarch64-linux-gnu/libusb-1.0.so)

# enable_testing()
# include_directories(src)
# add_executable(simpleusbtest test/simpleusbtest.cpp)
# target_link_libraries(simpleusbtest fatctl)


# include_directories(src)
# add_executable(testarduinousb test/testarduinousb.cpp)
# target_link_libraries(testarduinousb fatctl)