#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cassert>
#include <libusb.h>
#include <err.h>

#define MFGR_ID 0x2341 // given manufacturer ID 
#define DEV_ID 0x805a  // given device ID

#define BULK_EP_IN 0x81

/* If device IDs are not known, use libusb_get_device_list() to see a 
list of all USB devices connected to the machine. Follow this call with    
libusb_free_device_list() to free the allocated device list memory.
*/


int main() {
    libusb_context *context = NULL;
    libusb_device **list = NULL;
    int rc = 0;
    ssize_t count = 0;

    int init = libusb_init(NULL); // NULL is the default libusb_context
    if (init < 0) {
        errx(1,"\n\nERROR: Cannot Initialize libusb\n\n");  
    }

    count = libusb_get_device_list(context, &list);
        for (size_t idx = 0; idx < count; ++idx) {
        libusb_device *device = list[idx];
        libusb_device_descriptor desc = {0};

        rc = libusb_get_device_descriptor(device, &desc);
        assert(rc == 0);

        printf("Vendor:Device = %04x:%04x\n", desc.idVendor, desc.idProduct);
    }

    libusb_free_device_list(list, 1);


    struct libusb_device_handle *dh = NULL; // The device handle
    dh = libusb_open_device_with_vid_pid(NULL, MFGR_ID, DEV_ID);

    if (!dh) {
        errx(1, "\n\nERROR: Cannot connect to device %04x\n\n",DEV_ID);
    }
    printf("interface claimed\n");

    // reference: 
    // set fields for the setup packet as needed              
    uint8_t       bmReqType = 0;   // the request type (direction of transfer)
    uint8_t            bReq = 0x01;   // the request field for this packet
    uint16_t           wVal = 0;   // the value field for this packet
    uint16_t         wIndex = 0;   // the index field for this packet
    unsigned char*   data = reinterpret_cast<uint8_t *>(calloc(32,1));   // the data buffer for the in/output data
    uint16_t           wLen = 0;   // length of this setup packet 
    unsigned int     to = 0;       // timeout duration (if transfer fails)

    data[0] = ' ';
    // transfer the setup packet to the USB device
    //int config = libusb_control_transfer(dh, bmReqType, bReq,wVal, wIndex, data, wLen, to);

    int transferred = 0;
    int received = 0;
    int length = 0;
    int config;
    int e = libusb_get_configuration(dh, &config);
    e = libusb_bulk_transfer(dh, BULK_EP_IN, data, length, &received, 0);

    //libusb_control_transfer_get_data()

    if (config < 0) {
        errx(1, "\n\nERROR: No data transmitted to device %04x\n\n", DEV_ID);
    }

    printf("recieved %d packets\n", received);

    libusb_exit(NULL);
}