cmake_minimum_required(VERSION 3.25)
project(fatctl DESCRIPTION "MVP for Arduino Nano MC" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "fatcnt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libserial")
set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(JSON_VERSION v3.11.3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_TEST ${CMAKE_BINARY_DIR}/test)

set(fatcnt_SOURCES
  src/fatcnt/environment/environmentProcessor.cpp
  src/fatcnt/environment/processorbase.cpp
  src/fatcnt/state/statefactory.cpp
  src/fatcnt/state/rrqueues.cpp
  src/fatcnt/events/eventhandler.cpp
  src/fatcnt/events/uihandler.cpp
  src/fatcnt/events/statushandler.cpp
  src/fatcnt/protocols/common/curators/rrp/msp_ident_curator.cpp
  src/fatcnt/events/ui/jserializer.cpp
  src/fatcnt/events/catagorizer.cpp
  src/fatcnt/events/catagorizer/ldsqu001mapper.cpp
  src/fatcnt/events/catagorizer/mapperfactory.cpp
  src/fatcnt/com/server.cpp
)

# Include directory
add_subdirectory(include)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})

pkg_check_modules(LIBSERIAL REQUIRED libserial)

include_directories(${json_SOURCE_DIR}/include src ${LIBSERIAL_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include)
pkg_search_module(DLIB REQUIRED dlib-1)

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

add_executable(test_environmentprocessor test/test_environmentprocessor.cpp)
set_target_properties(test_environmentprocessor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_environmentprocessor GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_environmentprocessor  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_statefactory test/test_statefactory.cpp)
set_target_properties(test_statefactory PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_statefactory GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_statefactory  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_eventhandler test/test_eventhandler.cpp)
set_target_properties(test_eventhandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_eventhandler GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_eventhandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")


add_executable(test_rrpcurator test/test_rrpcurator.cpp)
set_target_properties(test_rrpcurator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_rrpcurator GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_rrpcurator  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_jserializer test/test_jserializer.cpp)
set_target_properties(test_jserializer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_jserializer GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_jserializer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_uihandler test/test_uihandler.cpp)
set_target_properties(test_uihandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_uihandler GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_uihandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_catagorizer test/test_catagorizer.cpp)
set_target_properties(test_catagorizer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_catagorizer GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_catagorizer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")