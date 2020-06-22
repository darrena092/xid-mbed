#ifndef XBOXUSBGAMEPADNEW_H
#define XBOXUSBGAMEPADNEW_H

#include "USBDevice.h"
#include "USBDescriptor.h"
#include "mbed.h"

typedef struct {
    uint8_t UNUSED_0;
    uint8_t LENGTH;  // always 0x14

    uint8_t DPAD_UP : 1;
    uint8_t DPAD_DOWN : 1;
    uint8_t DPAD_LEFT : 1;
    uint8_t DPAD_RIGHT : 1;

    uint8_t BUTTON_START : 1;
    uint8_t BUTTON_BACK : 1;
    uint8_t BUTTON_L3 : 1;
    uint8_t BUTTON_R3 : 1;

    uint8_t UNUSED_1;

    uint8_t BUTTON_A;
    uint8_t BUTTON_B;
    uint8_t BUTTON_X;
    uint8_t BUTTON_Y;

    uint8_t BUTTON_BLACK;
    uint8_t BUTTON_WHITE;

    uint8_t TRIGGER_LEFT;
    uint8_t TRIGGER_RIGHT;

    int16_t LEFT_STICK_X;
    int16_t LEFT_STICK_Y;
    int16_t RIGHT_STICK_X;
    int16_t RIGHT_STICK_Y;

} __attribute__((packed)) XHIDReport;

typedef struct {
    uint8_t report_id;
    uint8_t length;
    uint16_t left_actuator_strength;
    uint16_t right_actuator_strength;
} __attribute__((packed)) XHIDOutputReport;

class XboxUSBGamepadNew : public USBDevice {
public:
    XboxUSBGamepadNew();
    void SendReport(XHIDReport* hidReport);
protected:
    virtual const uint8_t *configuration_desc(uint8_t index) override;
    virtual void callback_state_change(DeviceState new_state) override {}
    virtual void callback_request(const setup_packet_t *setup) override;
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted) override {
        complete_request_xfer_done(true);
    };
    virtual void callback_set_configuration(uint8_t configuration) override;
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate) override {};
private:
    void sendISR();
    void recvISR();
    events::EventQueue debugQueue;
    Thread debugThread;
    XHIDReport xHIDReport;
};

#endif