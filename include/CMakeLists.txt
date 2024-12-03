configure_file(fatcnt/environment/version.hpp.in fatcnt/environment/version.hpp @ONLY)

# multitypes
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
configure_file(fatcnt/protocols/common/multitype.hpp.in fatcnt/protocols/common/multitype.hpp  @ONLY)
#end multitypes

# Serial type configuration.
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

configure_file(fatcnt/environment/rrserial.hpp.in fatcnt/environment/rrserial.hpp @ONLY)
# End Serial type configuration

#Exceptions
set(EXCEPTIONS InvalidManifestException MissingRequiredAttributeException QueueDoesNotExit ExceedMaxMessage InvalidMessageSent InvalidFormat InvalidCommand UnsupportedAttribute)
set(INCLUDE_CONTENT "")
foreach(item ${EXCEPTIONS})
  set(EXCEPTION_NAME ${item})
  set(OUTPUT_NAME "fatcnt/exceptions/${EXCEPTION_NAME}.hpp")
  string(TOUPPER "${EXCEPTION_NAME}_HPP" EXCEPTION_HEADING)
  configure_file(fatcnt/exceptions/exception.hpp.in ${OUTPUT_NAME} @ONLY)
  string(APPEND INCLUDE_CONTENT "\#include <fatcnt/exceptions/${EXCEPTION_NAME}.hpp>\n")
endforeach()
set(GENERATED_INCLUDES ${INCLUDE_CONTENT})
configure_file(fatcnt/exceptions/exceptions.hpp.in fatcnt/exceptions/exceptions.hpp @ONLY)

#end Exceptions

# static header files
set(fatcnt_INCLUDE 
  fatcnt/environment/environment.hpp
  fatcnt/environment/hwmodel.hpp
  fatcnt/environment/queues.hpp
  fatcnt/events/Event.hpp
  fatcnt/protocols/common/mspdirection.hpp
  fatcnt/protocols/common/mspversion.hpp
  fatcnt/state/state.hpp
  fatcnt/state/stateiface.hpp
  fatcnt/protocols/common/curators/rrp/rrpcuratorabstract.hpp
)
foreach(item ${fatcnt_INCLUDE})
  configure_file("${item}.in" ${item} @ONLY)
endforeach()
# these are copied straight accross without much change


# MSP COMMANDS
# structure of hash is
# 0 command
# 1 command value
# 2 if true then generate serializer
# 3 if true then generate deseralizer
# 4 direction (which queue to message should be directed to)
# 5..n members in pairs, type [attribute]
set(MSPCOMMANDS 
  "MSP_NONE 0 INVALID 1 1" 
  "MSP_AUTHKEY 7 USER_INTERFACE 1 1 string key"
  "MSP_IDENT 100 USER_INTERFACE 0 0 uint8_t version MULTITYPE_T multitype MSP_VERSION msp_version uint32_t capability"
  "MSP_STATUS 101 CATEGORIZER 1 1 uint16_t cycletime uint16_t i2c_errors_count uint16_t sensor uint32_t flag uint8_t current_set"
  "MSP_MOTOR 104 FLIGHT_CONTROLLER 1 1 uint16_t motor1 uint16_t motor2 uint16_t motor3 uint16_t motor4 uint16_t motor5 uint16_t motor6 uint16_t motor7 uint16_t motor8"
  "MSP_SET_MOTOR 214 FLIGHT_CONTROLLER 1 1 uint16_t motor1 uint16_t motor2 uint16_t motor3 uint16_t motor4 uint16_t motor5 uint16_t motor6 uint16_t motor7 uint16_t motor8")


set(LOOP_CONTENT "")
set(VALID_MSPCOMMANDS "")
set(INCLUDE_CONTENT "")
set(INCLUDE_CURATOR_CONTENT "")
set(CURATOR_PAIR "")
foreach(item ${MSPCOMMANDS})
  separate_arguments(_elements UNIX_COMMAND ${item})
  list(LENGTH _elements _length)
  list(GET _elements 0 _key)
  list(GET _elements 1 _value)
  list(GET _elements 2 _direction)
  list(GET _elements 3 _serializer)
  list(GET _elements 4 _deserializer)

  set(CURATATOR_SERIALIZER ${_serializer})
  set(CURATATOR_DESERIALIZER ${_deserializer})
  string(TOUPPER "${_key}_MCURATATOR_SERIALIZER" MCURATATOR_SERIALIZER)
  string(TOUPPER  "${_key}_MCURATATOR_DESERIALIZER" MCURATATOR_DESERIALIZER)

  string(APPEND LOOP_CONTENT "${_key} = ${_value},\n")
  string(APPEND VALID_MSPCOMMANDS "\{\"${_key}\"\, ${_key}\},\\\n")

  string(TOUPPER "${_key}_HPP" EVENT_HEADING)
  string(TOUPPER "${_key}_CURATOR_HPP" CURATOR_HEADING)
  string(TOLOWER "${_key}" CLASS_NAME)
  string(TOLOWER "${_key}_curator" CURATOR_CLASS_NAME)
  string(TOLOWER "fatcnt/protocols/common/curators/rrp/${_key}.hpp" OUTPUT_NAME)
  string(TOLOWER "fatcnt/protocols/common/curators/rrp/${_key}_curator.hpp" OUTPUT_CURATOR_NAME)
  string(APPEND INCLUDE_CONTENT "\#include <${OUTPUT_NAME}>\n")
  string(APPEND INCLUDE_CURATOR_CONTENT "\#include <${OUTPUT_CURATOR_NAME}>\n")
  string(APPEND CURATOR_PAIR "\{${_key}, new ${CURATOR_CLASS_NAME}()\},")

  set(MSP_COMMAND ${_key})
  set(GETTERS "")
  set(MEMBERS "")
  set(CURATOR_SETTERS "")
  set(DIRECTION ${_direction})
  set(CURATOR_SERIALIZER_SETTERS "")
  
  math(EXPR last_index "${_length} - 1")
  if (${_length} GREATER 5)
    set(start_index 5)
    foreach(i RANGE ${start_index} ${last_index} 2)
      list(SUBLIST _elements ${i} 2 pair)
      list(GET pair 0 type)
      list(GET pair 1 member)
      string(APPEND GETTERS "    void set_${member}(${type} ${member})\{_${member} = ${member};\}\n    ${type} get_${member}()\{return _${member};\}\n")
      string(APPEND MEMBERS "    ${type} _${member};\n")
      string(APPEND CURATOR_SETTERS "            validate(\"${member}\", in);\n            payload->set_${member}(in[\"${member}\"]);\n")
      string(APPEND CURATOR_SERIALIZER_SETTERS "                    \{\"${member}\", payload.get_${member}()\},\n")
    endforeach()
  endif()
  configure_file(fatcnt/protocols/common/curators/rrp/event.tmpl.hpp.in ${OUTPUT_NAME} @ONLY)
  configure_file(fatcnt/protocols/common/curators/rrp/curator.tmpl.hpp.in ${OUTPUT_CURATOR_NAME} @ONLY)
endforeach()
configure_file(fatcnt/protocols/common/mspcommands.hpp.in fatcnt/protocols/common/mspcommands.hpp @ONLY)
configure_file(fatcnt/protocols/common/curators/rrp/mspevent.hpp.in fatcnt/protocols/common/curators/rrp/mspevent.hpp)
configure_file(fatcnt/protocols/common/curators/rrp/rrpcurator.hpp.in fatcnt/protocols/common/curators/rrp/rrpcurator.hpp)


