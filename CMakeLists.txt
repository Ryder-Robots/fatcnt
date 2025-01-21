cmake_minimum_required(VERSION 3.25)
project(fatcnt DESCRIPTION "MVP for Ryder Robots" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)
include(CMakeDependentOption)

cmake_dependent_option(MSP_SKULD001
    "Enable ${PROJECT_NAME} with SKULD001 support" OFF
    "MSP_SKULD001" OFF)
  
cmake_dependent_option(MSP_SKULD002
    "Enable ${PROJECT_NAME} with SKULD002 support" ON
    "MSP_SKULD002" ON)

message(CHECK_START "MSP_VERSION")
if (MSP_SKULD001)
  message(CHECK_PASS  "found SKULD001")
  set(MANIFEST_MSPVERSION "SKULD001")
  set(MANIFEST_DESCRIPTION "Configuration for SKULD001")
elseif (MSP_SKULD002) 
  message(CHECK_PASS " found SKULD002")
  set(MANIFEST_MSPVERSION "SKULD002")
  set(MANIFEST_DESCRIPTION "Configuration for SKULD002")
else()
  message(CHECK_FAILED "MSP_VERSION NOT SET!")  
endif()

set(CPACK_GENERATOR "DEB")    
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "fatcnt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "4")

# TODO: Move this EXTRA_LIB for drone that needs it
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libserial-dev")
set(CPACK_SOURCE_IGNORE_FILES "/gtest/;/googletest/")
set(CPACK_PACKAGE_CONTACT "Aaron Spiteri")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/scripts/postinst")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/scripts/prerm")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/resources/fatcnt.env"
  DESTINATION /opt/fatcnt/etc/
  PERMISSIONS OWNER_READ OWNER_WRITE  GROUP_READ  WORLD_READ
)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/fatcnt.service"
        DESTINATION /etc/systemd/system
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/resources/manifests/manifest.json"
        DESTINATION /opt/fatcnt/etc/
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/bin/fatcnt" DESTINATION 
  /opt/fatcnt/bin 
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lib/libctl.so" DESTINATION 
  /opt/fatcnt/lib 
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
  src/fatcnt/events/msp104/msp104_ctl_in.cpp
  src/fatcnt/events/eai_handler.cpp
  src/fatcnt/events/ai/ai_training_data.cpp
  src/fatcnt/protocols/common/curators/rrp/msp_ident_curator.cpp
  src/fatcnt/protocols/common/curators/rrp/msp_mode_curator.cpp
  src/fatcnt/events/ui/jserializer.cpp
  src/fatcnt/events/catagorizer.cpp
  src/fatcnt/events/catagorizer/mapperfactory.cpp
  src/fatcnt/com/server.cpp
)

# SOURCE CODE SPECIFIC LD drones without micro-processors
set(EXTRA_LIBS "")
if (MSP_SKULD001 OR MSP_SKULD002)
  set(EXTRA_LIBS "wiringPi")
  set(MANIFEST_MULTITYPE "LANDDRONE_2W")
  set(MANIFEST_AI_QUEUE_FC "MICROCONTROLLER")
  set(MANIFEST_AI_QUEUE_MC "MICROCONTROLLER")
  configure_file(resources/manifests/manifest.json.in resources/manifests/manifest.json @ONLY)
  configure_file(resources/fatcnt.env.in resources/fatcnt.env @ONLY)
  list(APPEND fatcnt_SOURCES
    src/fatcnt/events/msp104/msp104_hbridge_serializer.cpp
    src/fatcnt/events/catagorizer/ldsqu001mapper.cpp 
    src/fatcnt/events/hbridgecontroller.cpp)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "wiringpi, wiringpi (>= 3.2)")
endif()
add_library(ctl SHARED  ${fatcnt_SOURCES})
target_link_libraries(ctl pthread  ${LIBSERIAL_LIBRARIES} ${EXTRA_LIBS})

add_executable(fatcnt src/fatcnt/fatcnt.hpp src/fatcnt/fatcnt.cpp)
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

add_executable(test_statushandler test/test_statushandler.cpp)
set_target_properties(test_statushandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_statushandler GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_statushandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_status_processor test/test_status_processor.cpp)
set_target_properties(test_status_processor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_status_processor GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_status_processor  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_ratio test/test_ratio.cpp)
set_target_properties(test_ratio PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_ratio GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_ratio  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_msp104_ctl_in test/test_msp104_ctl_in.cpp)
set_target_properties(test_msp104_ctl_in PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_msp104_ctl_in GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_msp104_ctl_in  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_ai_training_data test/test_ai_training_data.cpp)
set_target_properties(test_ai_training_data PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_ai_training_data GTest::gtest_main ctl dlib gmock)
gtest_discover_tests(test_ai_training_data  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

if (MSP_SKULD001 OR MSP_SKULD002)
  add_executable(test_hbridgehandler test/test_hbridgehandler.cpp)
  set_target_properties(test_hbridgehandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
  target_link_libraries(test_hbridgehandler GTest::gtest_main ctl dlib gmock)
  gtest_discover_tests(test_hbridgehandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

  add_executable(test_msp104_hbridge_serializer test/test_msp104_hbridge_serializer.cpp)
  set_target_properties(test_msp104_hbridge_serializer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
  target_link_libraries(test_msp104_hbridge_serializer GTest::gtest_main ctl dlib gmock)
  gtest_discover_tests(test_msp104_hbridge_serializer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources") 
endif()