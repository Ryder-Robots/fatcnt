#ifndef EXTERNAL_HPP
#define EXTERNAL_HPP

#include <sys/socket.h>
#include <stdint.h>

namespace rrobot {

    /**
     * @class External
     * 
     * @brief
     * allows communication with an external interface,  this may be LAN, WAN, or USB. 
     * 
     * Note that all methods are postfixed with _rr so that they do not conflict with existing symbols.
     * 
     * A specific interface needs to be written to handle specific communication.
     */
    class External {
        public:
        /**
         * @fn recv_rr
         * @brief
         * reads data from communications port, or serial device. Note that reads are done using BLOCKING mode, therefore 
         * use the available() to determine if blocks are available.
         * 
         * @param buffer that data will be placed into.
         * @param max amount of bytes that can be saved
         * @return actual bytes written to buffer, -1 if no bytes are available.
         */
        virtual ssize_t recv_rr(void* buffer, size_t bufsz);

        /**
         * @fn accept_rr
         * @brief 
         * internal connection can be different to the behaviour of this method, it does return the file descriptor, however this should
         * only be detected to ensure that it does not return -1, the calling process can send 'false' to this method which will result
         * in the connection being rejected.  Drones should only have one connected user at a time. For obvious reasons ;)
         * 
         * @param accept the connection or reject.
         * @return file descriptor.
         */
        virtual int accept_rr(bool accept);

        /**
         * @fn close_rr
         * @brief 
         * closes the connection that is getting used, this must the last call done after a connection, or when shutting down.
         * It may also indicate failed authentication.
         */
        virtual void close_rr();

        /**
         * @fn send_rr
         * @brief
         * send bufsz bytes to file handle, all bytes will be from buff.
         * @param buf bytes to send
         * @param bufsz the bytes to send
         * @return actual size of bytes sent.
         */
        virtual ssize_t send_rr(const void *buf, size_t bufsz);

        /**
         * @fn available
         * @brief 
         * returns how many bytes are available.
         */
        virtual size_t available();
    };
}

#endif //EXTERNAL_HPP