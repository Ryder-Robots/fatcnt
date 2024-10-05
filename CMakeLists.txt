cmake_minimum_required(VERSION 3.25)
project(fatctl DESCRIPTION "MVP for Arduino Nano MC" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "fatctl")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")
set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(JSON_VERSION v3.11.3)

set(fatctl_SOURCES 
    src/fatctl/ui/client_manifest.cpp
    src/fatctl/ui/server.cpp
    src/fatctl/environment/state.cpp
)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})


include_directories(${json_SOURCE_DIR}/include include src ${llhttp_SOURCE_DIR}/include)
pkg_search_module(DLIB REQUIRED dlib-1)


add_executable(fatctl src/fatctl/fatctl.h src/fatctl/fatctl.cpp  ${fatctl_SOURCES})
target_link_libraries(fatctl wiringPi)
target_link_libraries(fatctl dlib)
target_link_libraries(fatctl pthread)

enable_testing()
# include_directories(src)
# # add_executable(simpleusbtest test/simpleusbtest.cpp)
# # target_link_libraries(simpleusbtest fatctl)

include_directories(${json_SOURCE_DIR}/include include src)
add_executable(simplejwttest test/simplejwttest.cpp src/fatctl/ui/client_manifest.cpp)

include_directories(${json_SOURCE_DIR}/include include src)
add_executable(simplesockettest test/simplesockettest.cpp src/fatctl/ui/server.cpp)
target_link_libraries(simplesockettest dlib)
target_link_libraries(simplesockettest pthread)