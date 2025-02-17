cmake_minimum_required(VERSION 3.25)
project(fatcnt DESCRIPTION "MVP for Ryder Robots" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)
include(CMakeDependentOption)
include(GNUInstallDirs)

set(CPACK_GENERATOR "DEB")    
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "rrobot")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libserial-dev")
set(CPACK_SOURCE_IGNORE_FILES "/gtest/;/googletest/")
set(CPACK_PACKAGE_CONTACT "Aaron Spiteri")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lib/librrobot.so" 
  DESTINATION /usr/local/lib/ 
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


include(CPack)

set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(JSON_VERSION v3.11.3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_TEST ${CMAKE_BINARY_DIR}/test)

# Include directory
add_subdirectory(include)
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/fatcnt
        DESTINATION /usr/local/include
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})

pkg_check_modules(LIBSERIAL REQUIRED libserial)

include_directories(${json_SOURCE_DIR}/include src ${LIBSERIAL_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include)
pkg_search_module(DLIB REQUIRED dlib-1)

find_package(Boost 1.74.0 COMPONENTS program_options REQUIRED )

# Sources common to all drones
set(fatcnt_SOURCES
  src/fatcnt/environment/environmentProcessor.cpp
  src/fatcnt/environment/processorbase.cpp
  src/fatcnt/state/statefactory.cpp
  src/fatcnt/state/statemanager.cpp
  src/fatcnt/state/rrqueues.cpp
  src/fatcnt/events/eventhandler.cpp
  src/fatcnt/events/uihandler.cpp
  src/fatcnt/events/statushandler.cpp
  src/fatcnt/events/eai_handler.cpp
  src/fatcnt/events/ai/ai_training_data.cpp
  src/fatcnt/protocols/common/curators/rrp/msp_ident_curator.cpp
  src/fatcnt/protocols/common/curators/rrp/msp_mode_curator.cpp
  src/fatcnt/events/ui/jserializer.cpp
  src/fatcnt/events/catagorizer.cpp
  src/fatcnt/com/server.cpp
  src/fatcnt/main.cpp
)

# SOURCE CODE SPECIFIC LD drones without micro-processors
#TODO: This is to be removed
#if (MSP_SKULD001 OR MSP_SKULD002)
# list(APPEND fatcnt_SOURCES
#     src/fatcnt/events/msp104/msp104_hbridge_serializer.cpp
#     src/fatcnt/events/catagorizer/ldsqu001mapper.cpp 
#     src/fatcnt/events/hbridgecontroller.cpp)
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")
# set(EXTRA_LIBS "wiringPi")
#endif()

add_library(rrobot SHARED  ${fatcnt_SOURCES})
target_link_libraries(rrobot pthread  ${LIBSERIAL_LIBRARIES} ${EXTRA_LIBS}  ${Boost_LIBRARIES})

set(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/rrobot.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/rrobot.pc
  @ONLY
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/rrobot.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# add_executable(fatcnt src/fatcnt/fatcnt.hpp src/fatcnt/fatcnt.cpp)
# target_link_libraries(fatcnt rrobot)
# target_link_libraries(fatcnt dlib)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

#  src/fatcnt/events/ai/ai_virtual_predictor.cpp
#  src/fatcnt/events/msp104/msp104_ctl_in.cpp
#   src/fatcnt/events/msp104/msp104_hbridge_serializer.cpp

enable_testing()
include(GoogleTest)
include(Dart)
include_directories(${json_SOURCE_DIR}/include src ${LIBSERIAL_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/test)

add_executable(test_environmentprocessor test/test_environmentprocessor.cpp)
set_target_properties(test_environmentprocessor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_environmentprocessor GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_environmentprocessor  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_statefactory test/test_statefactory.cpp)
set_target_properties(test_statefactory PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_statefactory GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_statefactory  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_eventhandler test/test_eventhandler.cpp)
set_target_properties(test_eventhandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_eventhandler GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_eventhandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_rrpcurator test/test_rrpcurator.cpp)
set_target_properties(test_rrpcurator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_rrpcurator GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_rrpcurator  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_jserializer test/test_jserializer.cpp)
set_target_properties(test_jserializer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_jserializer GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_jserializer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_uihandler test/test_uihandler.cpp)
set_target_properties(test_uihandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_uihandler GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_uihandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_catagorizer test/test_catagorizer.cpp)
set_target_properties(test_catagorizer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_catagorizer GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_catagorizer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_statushandler test/test_statushandler.cpp)
set_target_properties(test_statushandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_statushandler GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_statushandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_status_processor test/test_status_processor.cpp)
set_target_properties(test_status_processor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_status_processor GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_status_processor  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_ratio test/test_ratio.cpp)
set_target_properties(test_ratio PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_ratio GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_ratio  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_msp104_ctl_in test/test_msp104_ctl_in.cpp test/fatcnt/events/msp104/msp104_ctl_in.cpp)
set_target_properties(test_msp104_ctl_in PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_msp104_ctl_in GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_msp104_ctl_in  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_ai_training_data test/test_ai_training_data.cpp)
set_target_properties(test_ai_training_data PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_ai_training_data GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_ai_training_data  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")


# #TODO Move this 
# if (MSP_SKULD001 OR MSP_SKULD002)
#   add_executable(test_hbridgehandler test/test_hbridgehandler.cpp)
#   set_target_properties(test_hbridgehandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
#   target_link_libraries(test_hbridgehandler GTest::gtest_main rrobot dlib gmock)
#   gtest_discover_tests(test_hbridgehandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

#   add_executable(test_msp104_hbridge_serializer test/test_msp104_hbridge_serializer.cpp)
#   set_target_properties(test_msp104_hbridge_serializer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
#   target_link_libraries(test_msp104_hbridge_serializer GTest::gtest_main rrobot dlib gmock)
#   gtest_discover_tests(test_msp104_hbridge_serializer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources") 
# endif()
