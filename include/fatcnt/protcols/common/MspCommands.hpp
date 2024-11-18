#ifndef MSPCOMMANDS_HPP
#define MSPCOMMANDS_HPP

#include <stdint.h>

namespace rrobot {

/**
 * @brief list of currently supported commands. Note that the list is taken from
 * http://www.multiwii.com/wiki/index.php?title=Multiwii_Serial_Protocol
 */

enum MspCommands : uint8_t {

    MSP_NONE = 0,

    MSP_AUTHKEY = 7,

    /**
     * identication response.
     */
    MSP_IDENT = 100,

    /**
     * Heartbeat command given by Microprocessor.
     */
    MSP_STATUS = 101,

    /**
     * payload can is a value between 1000:2000, supports up to 8 motors.
     */
    MSP_MOTOR = 104,

    /**
     * For quadcopter, and flight drones use to set individual motor value (to be used only with DYNBALANCE config)
     *
     * For land drones using H-Bridge used to set motor polarity for each motor. For instance 0x01010101 indicates that
     * Motor A IN1 = LOW, Motor A IN2 = HIGH, Motor A IN3 = LOW, Motor A IN4 = LOW,
     * Motor B IN1 = LOW, Motor B IN2 = HIGH, Motor B IN3 = LOW, Motor B IN4 = LOW,
     */
    MSP_SET_MOTOR = 214,
};
}  // namespace rrobot

#endif  // MSPCOMMANDS_HPP