#ifndef XBOXUSBGAMEPAD_H
#define XBOXUSBGAMEPAD_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice.h"

#include "USBHID_Types.h"
#include "OperationList.h"

#define XBOX_CONTROLLER_VENDOR_ID 0x045E
#define XBOX_CONTROLLER_PRODUCT_ID 0x0202

#define XID_GET_DESCRIPTOR 6
#define XID_GET_REPORT 1
#define XID_GET_CAPABILITIES 
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdXid;
    uint8_t bType;
    uint8_t bSubType;
    uint8_t bMaxInputReportSize;
    uint8_t bMaxOutputReportSize;
    uint16_t wAlternateProductIds[4];
} XIDDescriptor;

typedef struct {
	uint8_t unk1;
	uint8_t reportSize;
	uint8_t digital;
	uint8_t unk2;
    uint8_t a;
    uint8_t b;
    uint8_t x;
    uint8_t y;
    uint8_t black;
    uint8_t white;
    uint8_t lTrigger;
    uint8_t rTrigger;
    int16_t lX;
    int16_t lY;
    int16_t rX;
    int16_t rY;
} XboxControllerReport;

/**
 * \defgroup drivers_USBHID USBHID class
 * \ingroup drivers-public-api-usb
 * @{
 */

/**
 * USBHID example
 * @code
 * #include "mbed.h"
 * #include "USBHID.h"
 *
 * USBHID hid;
 * HID_REPORT recv;
 * BusOut leds(LED1,LED2,LED3,LED4);
 *
 * int main(void) {
 *    while (1) {
 *        hid.read(&recv);
 *        leds = recv.data[0];
 *    }
 * }
 * @endcode
 */

class XboxUSBGamepad: public USBDevice {
public:

    /**
    * Basic constructor
    *
    * Construct this object optionally connecting and blocking until it is ready.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param output_report_length Maximum length of a sent report (up to 64 bytes)
    * @param input_report_length Maximum length of a received report (up to 64 bytes)
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    XboxUSBGamepad(bool connect_blocking = true, uint8_t output_report_length = 64, uint8_t input_report_length = 64, uint16_t vendor_id = XBOX_CONTROLLER_VENDOR_ID, uint16_t product_id = XBOX_CONTROLLER_PRODUCT_ID, uint16_t product_release = 0x0001);

    /**
    * Fully featured constructor
    *
    * Construct this object with the supplied USBPhy and parameters. The user
    * this object is responsible for calling connect() or init().
    *
    * @note Derived classes must use this constructor and call init() or
    * connect() themselves. Derived classes should also call deinit() in
    * their destructor. This ensures that no interrupts can occur when the
    * object is partially constructed or destroyed.
    *
    * @param phy USB phy to use
    * @param output_report_length Maximum length of a sent report (up to 64 bytes)
    * @param input_report_length Maximum length of a received report (up to 64 bytes)
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    XboxUSBGamepad(USBPhy *phy, uint8_t output_report_length, uint8_t input_report_length, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~XboxUSBGamepad();

    /**
     * Check if this class is ready
     *
     * @return true if the device is in the configured state
     */
    bool ready();

    /**
     * Block until this HID device is in the configured state
     */
    void wait_ready();

    /**
    * Send a Report. warning: blocking
    *
    * @param report Report which will be sent (a report is defined by all data and the length)
    * @returns true if successful
    */
    bool send(const HID_REPORT *report);


    /**
    * Send a Report. warning: non blocking
    *
    * @param report Report which will be sent (a report is defined by all data and the length)
    * @returns true if successful
    */
    bool send_nb(const HID_REPORT *report);

    /**
    * Read a report: blocking
    *
    * @param report pointer to the report to fill
    * @returns true if successful
    */
    bool read(HID_REPORT *report);

    /**
    * Read a report: non blocking
    *
    * @param report pointer to the report to fill
    * @returns true if successful
    */
    bool read_nb(HID_REPORT *report);

    void sendTestCommand();

protected:
    uint16_t reportLength;
    uint8_t reportDescriptor[27];

    /*
    * Get the Report descriptor
    *
    * @returns pointer to the report descriptor
    */
    virtual const uint8_t *report_desc();

    /*
    * Get the length of the report descriptor
    *
    * @returns the length of the report descriptor
    */
    virtual uint16_t report_desc_length();

    /*
    * Get string product descriptor
    *
    * @returns pointer to the string product descriptor
    */
    virtual const uint8_t *string_iproduct_desc();

    /*
    * Get string interface descriptor
    *
    * @returns pointer to the string interface descriptor
    */
    virtual const uint8_t *string_iinterface_desc();

    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t *configuration_desc(uint8_t index);


    /*
    * HID Report received by SET_REPORT request. Warning: Called in ISR context
    * First byte of data will be the report ID
    *
    * @param report Data and length received
    */
    virtual void HID_callbackSetReport(HID_REPORT *report) {};

    /**
    * Called when USB changes state
    *
    * @param new_state The new state of the USBDevice
    *
    * Warning: Called in ISR context
    */
    virtual void callback_state_change(DeviceState new_state);

    /*
    * This is used to handle extensions to standard requests
    * and class specific requests
    */
    virtual void callback_request(const setup_packet_t *setup);

    /*
    * This is used to handle extensions to standard requests
    * and class specific requests with a data phase
    */
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);


    /*
    * Called by USBDevice layer. Set configuration of the device.
    * For instance, you can add all endpoints that you need on this function.
    *
    * @param configuration Number of the configuration
    * @returns true if class handles this request
    */
    virtual void callback_set_configuration(uint8_t configuration);

    /*
    * Called by USBDevice layer in response to set_interface.
    *
    * Upon reception of this command endpoints of any previous interface
    * if any must be removed with endpoint_remove and new endpoint added with
    * endpoint_add.
    *
    * @param configuration Number of the configuration
    *
    * Warning: Called in ISR context
    */
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);

    /*
     * Called when there is a hid report that can be read
     */
    virtual void report_rx() {}

    /*
     * Called when there is space to send a hid report
     */
    virtual void report_tx() {}

protected:
    usb_ep_t _int_in;
    usb_ep_t _int_out;

private:
    void _init(uint8_t output_report_length, uint8_t input_report_length);
    void _send_isr();
    void _read_isr();

    class AsyncSend;
    class AsyncRead;
    class AsyncWait;

    OperationList<AsyncWait> _connect_list;
    OperationList<AsyncSend> _send_list;
    bool _send_idle;
    OperationList<AsyncRead> _read_list;
    bool _read_idle;

    uint8_t _configuration_descriptor[32];
    HID_REPORT _input_report;
    HID_REPORT _output_report;
    uint8_t _output_length;
    uint8_t _input_length;
};


#endif