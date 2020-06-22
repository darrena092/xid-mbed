#include "XboxUSBGamepadNew.h"
#include "usb_phy_api.h"
#include <stdio.h>

static uint8_t desc1[] = {0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40};
static uint8_t desc2[] = {
    0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32, 0x09, 0x04,
    0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x09, 0x21, 0x10, 0x01,
    0x00, 0x01, 0x22, 0x65, 0x00, 0x07, 0x05, 0x81, 0x03, 0x40, 0x00,
    0x04, 0x07, 0x05, 0x02, 0x03, 0x40, 0x00, 0x04};
static uint8_t desc3[] = {0x10, 0x42, 0x00, 0x01, 0x01, 0x01, 0x14, 0x06,
                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

XboxUSBGamepadNew::XboxUSBGamepadNew() : USBDevice(get_usb_phy(), 0xFAFA, 0xFAFA, 0x0001){
    connect();
    debugThread.start(callback(&debugQueue, &EventQueue::dispatch_forever));
}

void XboxUSBGamepadNew::callback_set_configuration(uint8_t configuration) {
    endpoint_add(0x82, 0x20, USB_EP_TYPE_INT, &XboxUSBGamepadNew::sendISR);
    endpoint_add(0x02, 0x20, USB_EP_TYPE_INT, &XboxUSBGamepadNew::recvISR);
    complete_set_configuration(true);
}

void XboxUSBGamepadNew::SendReport(XHIDReport* hidReport) {
    lock();
    memcpy(&xHIDReport, hidReport, sizeof(XHIDReport));
    write_start(0x82, (uint8_t*)&xHIDReport, sizeof(xHIDReport));
    unlock();
}

void debugSend() {
    printf("Sending report...\n");
}

void XboxUSBGamepadNew::sendISR() {
    // Send report to Xbox
    debugQueue.call(debugSend);
    write_finish(0x82);
}

void debugRecv() {
    printf("Receiving report...\n");
}

void XboxUSBGamepadNew::recvISR() {
    // Receive report from Xbox
    debugQueue.call(debugRecv);
}

void debugCallbackRequest(const USBDevice::setup_packet_t *setup) {
    printf("Got a control request:\n");
    printf("bmRequestType: 0x%x\n", setup->bmRequestType.Type);
    printf("bRequest: 0x%x\n", setup->bRequest);
    printf("wValue: 0x%x\n", setup->wValue);
    printf("wLength: 0x%x\n", setup->wLength);
}

void XboxUSBGamepadNew::callback_request(const setup_packet_t *setup) {
    uint8_t *hidDescriptor;
    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;


    debugQueue.call(callback(&debugCallbackRequest, setup));

    // if(setup->bRequest == 0x6) {
    //     if(setup->wValue == 0x4200) {
    //         switch(setup->wLength) {
    //             case 0x50:
    //                 size = 0x30;
    //                 data = desc2;
    //                 result = Send;
    //             break;
    //             case 0x10:
    //                 // GET_DESCRIPTOR?
    //                 size = setup->wLength;
    //                 data = desc3;
    //                 result = Send;
    //             break;
    //             case 0x06:
    //                 size = setup->wLength;
    //                 data = desc1;
    //                 result = Send;
    //             break;
    //             default:
    //             break;
    //         }
    //     }
    // }

    if(setup->bmRequestType.Type == VENDOR_TYPE) {
        if(setup->bRequest == 0x6) {
            if(setup->wValue == 0x4200) {
                //GET_DESCRIPTOR?
                size = setup->wLength;
                data = desc3;
                result = Send;
            } else {
                result = Failure;
            }
        } else if(setup->bRequest == 0x1 && (setup->wValue == 0x200 || setup->wValue == 0x100)) {
            XHIDOutputReport outputReport;
            memset(&outputReport, 0xFF, sizeof(XHIDOutputReport));
            outputReport.length = sizeof(XHIDOutputReport);
            size = outputReport.length;
            data = (uint8_t*)&outputReport;
            result = Send;
        }
    }

    complete_request(result, data, size);
}

#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) \
                               + (1 * INTERFACE_DESCRIPTOR_LENGTH) \
                               + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *XboxUSBGamepadNew::configuration_desc(uint8_t index) {
    static uint8_t configurationDescriptor[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
        CONFIGURATION_DESCRIPTOR,           // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),       // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),       // wTotalLength (MSB)
        0x01,                               // bNumInterfaces
        0x01,                               // bConfigurationValue
        0x00,                               // iConfiguration
        0x80,                               // bmAttributes
        0x32,                               // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH,        // bLength
        INTERFACE_DESCRIPTOR,               // bDescriptorType
        0x00,                               // bInterfaceNumber
        0x00,                               // bAlternateSetting
        0x02,                               // bNumEndpoints
        0x58,                               // bInterfaceClass
        0x42,                               // bInterfaceSubClass
        0x00,                               // bInterfaceProtocol
        0x00,                               // iInterface

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        0x82,                               // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        0x20,                               // wMaxPacketSize (LSB)
        0x00,                               // wMaxPacketSize (MSB)
        4,                                  // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        0x02,                               // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        0x20,                               // wMaxPacketSize (LSB)
        0x00,                               // wMaxPacketSize (MSB)
        4,                                  // bInterval (milliseconds)
    };

    return configurationDescriptor;
}