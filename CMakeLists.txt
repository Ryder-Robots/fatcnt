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

set(fatcnt_SOURCES
  src/fatcnt/environment/environmentProcessor.cpp
  src/fatcnt/eventprocessors/EventProcessor.cpp
)
configure_file(include/fatcnt/environment/version.hpp.in include/fatcnt/environment/version.hpp @ONLY)

# Keys for valid multitype
set(LOOP_CONTENT "")
set(MULTITYPE_LIST TRI QUADP QUADX BI GIMBAL Y6 HEX6 FLYING_WING Y4 HEX6X OCTOX8 OCTOFLATP OCTOFLATX AIRPLANE HELI_120 HELI_90 VTAIL4 HEX6H SINGLECOPTER DUALCOPTER LANDDRONE_2M LANDDRONE_4W)
foreach(item ${MULTITYPE_LIST})
    string(APPEND LOOP_CONTENT "${item},\n")
endforeach()

foreach(item ${MULTITYPE_LIST})
  string(APPEND VALID_MT "\{\"${item}\"\,${item}\},")
endforeach()

set(GENERATED_TYPE ${LOOP_CONTENT})
set(VALID_MT ${VALID_MT})
configure_file(include/fatcnt/protocols/common/multitype.hpp.in include/fatcnt/protocols/common/multitype.hpp  @ONLY)


set(BAUD_RATES BAUD_9600 BAUD_19200 BAUD_38400 BAUD_57600 BAUD_115200 BAUD_DEFAULT)
set(LOOP_CONTENT "")
foreach(item ${BAUD_RATES})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::BaudRate::${item}\},")
endforeach()
set(VALID_BAUDRATES ${LOOP_CONTENT})


set(VALID_CHAR_SZ CHAR_SIZE_5 CHAR_SIZE_6 CHAR_SIZE_7 CHAR_SIZE_8 CHAR_SIZE_DEFAULT)
set(LOOP_CONTENT "")
foreach(item ${VALID_CHAR_SZ})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::CharacterSize::${item}\},")
endforeach()
set(VALID_CHAR_SZ ${LOOP_CONTENT})

set(VALID_FLOW_CNT FLOW_CONTROL_HARDWARE FLOW_CONTROL_SOFTWARE FLOW_CONTROL_NONE)
set(LOOP_CONTENT "")
foreach(item ${VALID_FLOW_CNT})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::FlowControl::${item}\},")
endforeach()
set(VALID_FLOW_CNT ${LOOP_CONTENT})

set(VALID_PARITY PARITY_EVEN PARITY_ODD PARITY_NONE)
set(LOOP_CONTENT "")
foreach(item ${VALID_PARITY})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::Parity::${item}\},")
endforeach()
set(VALID_PARITY ${LOOP_CONTENT})

set(VALID_STOPBIT STOP_BITS_1 STOP_BITS_2 STOP_BITS_DEFAULT)
set(LOOP_CONTENT "")
foreach(item ${VALID_STOPBIT})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::StopBits::${item}\},")
endforeach()
set(VALID_STOPBIT ${LOOP_CONTENT})

configure_file(include/fatcnt/environment/rrserial.hpp.in include/fatcnt/environment/rrserial.hpp @ONLY)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})

set(EXCEPTIONS InvalidManifestException MissingRequiredAttributeException)
set(INCLUDE_CONTENT "")
foreach(item ${EXCEPTIONS})
  set(EXCEPTION_NAME ${item})
  set(OUTPUT_NAME "include/fatcnt/exceptions/${EXCEPTION_NAME}.hpp")
  string(TOUPPER "${EXCEPTION_NAME}_HPP" EXCEPTION_HEADING)
  configure_file(include/fatcnt/exceptions/exception.hpp.in ${OUTPUT_NAME} @ONLY)
  string(APPEND INCLUDE_CONTENT "\#include <fatcnt/exceptions/${EXCEPTION_NAME}.hpp>\n")
endforeach()
set(GENERATED_INCLUDES ${INCLUDE_CONTENT})
configure_file(include/fatcnt/exceptions/exceptions.hpp.in include/fatcnt/exceptions/exceptions.hpp @ONLY)

pkg_check_modules(LIBSERIAL REQUIRED libserial)

include_directories(${json_SOURCE_DIR}/include include src ${LIBSERIAL_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include)
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
target_link_libraries(test_environmentprocessor GTest::gtest_main)
target_link_libraries(test_environmentprocessor ctl)
target_link_libraries(test_environmentprocessor dlib)
gtest_discover_tests(test_environmentprocessor  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")
