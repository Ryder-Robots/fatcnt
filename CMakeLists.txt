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
set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(JSON_VERSION v3.11.3)

set(fatctl_SOURCES 
    src/fatctl/ui/server.hpp
    src/fatctl/ui/server.cpp
    src/fatctl/ui/connection.hpp
    src/fatctl/ui/connection.cpp
    src/fatctl/environment/state.hpp
    src/fatctl/environment/state.cpp
)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})

include_directories(${json_SOURCE_DIR}/include include src)
pkg_search_module(DLIB REQUIRED dlib-1)


add_executable(fatctl src/fatctl/fatctl.h src/fatctl/fatctl.cpp ${fatctl_SOURCES})
target_link_libraries(fatctl /usr/lib/libwiringPi.so)
target_link_libraries(fatctl /usr/local/lib/libmicrohttpd_ws.so)
target_link_libraries(fatctl /usr/local/lib/libmicrohttpd.so)


enable_testing()
# include_directories(src)
# # add_executable(simpleusbtest test/simpleusbtest.cpp)
# # target_link_libraries(simpleusbtest fatctl)

include_directories(${json_SOURCE_DIR}/include include src)
add_executable(simplejwttest test/simplejwttest.cpp src/fatctl/ui/connection.hpp src/fatctl/ui/connection.cpp)
