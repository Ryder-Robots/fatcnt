/*
 * TODO: Add licence
 *
 *
 */

/** 
 * @brief defines standard websocket errors as per rfc6455 
 */
namespace rrobot {
    // indicates that an endpoint is terminating the connection due 
    // to a protocol error.
    #define RR_WS_STATUS_PROTOCOL_ERR  1002

    // indicates that an endpoint is terminating the connection
    // because it has received a type of data it cannot accept
    #define RR_WS_STATUS_CANNOT_ACCEPT 1003
    
    // indicates that an endpoint is terminating the connection
    // because it has received data within a message that was not
    //  consistent with the type of the message
    #define RR_WS_NON_CONSISTENT       1007

    // indicates that an endpoint is terminating the connection
    //  because it has received a message that violates its policy.  This
    //  is a generic status code that can be returned when there is no
    //  other more suitable status code
    #define RR_WS_POLICY_VIOLATION     1008
    
    // it has received a message that is too big for it to process.
    #define RR_WS_TOO_LARGE            1009

    // indicates that a server is terminating the connection because
    //  it encountered an unexpected condition that prevented it from
    //  fulfilling the request.
    #define RR_WS_UNEXPECTED_COND      1011

    // Occurs when we are unable to create a socket.
    #define RR_WS_UNABLE_TO_CONNECT    1012
}