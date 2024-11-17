cmake_minimum_required(VERSION 3.25)
project(fatctl DESCRIPTION "MVP for Arduino Nano MC" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "fatcnt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libserial, libserial-dev")
set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(JSON_VERSION v3.11.3)

set(fatcnt_SOURCES 
  src/fatcnt/eventprocessors/EventProcessor.cpp
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})

pkg_check_modules(LIBSERIAL REQUIRED libserial)

include_directories(${json_SOURCE_DIR}/include include src ${LIBSERIAL_INCLUDE_DIRS})
pkg_search_module(DLIB REQUIRED dlib-1)

find_package(Boost 1.74.0 COMPONENTS program_options REQUIRED )

find_package(Boost 1.74.0 COMPONENTS program_options REQUIRED )

add_library(ctl SHARED  ${fatcnt_SOURCES})
target_link_libraries(ctl pthread)
target_link_libraries(ctl ${LIBSERIAL_LIBRARIES})

add_executable(fatcnt src/fatcnt/fatcnt.hpp src/fatcnt/fatcnt.cpp  ${fatcnt_SOURCES})
target_link_libraries(fatcnt ctl)
target_link_libraries(fatcnt dlib)
target_link_libraries(fatcnt ${Boost_LIBRARIES})

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)


enable_testing()
include(GoogleTest)
include(Dart)

