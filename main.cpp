#include "mbed.h"
#include "src/include/XboxUSBGamepadNew.h"

XboxUSBGamepadNew gamepad;

int main()
{
    ThisThread::sleep_for(5000);
    while (true) {
        XHIDReport hidReport;
        memset(&hidReport, 0, sizeof(XHIDReport));
        hidReport.LENGTH = sizeof(XHIDReport);
        hidReport.BUTTON_START = 1;
        gamepad.SendReport(&hidReport);
        ThisThread::sleep_for(300);
        hidReport.BUTTON_START = 0;
        gamepad.SendReport(&hidReport);
        ThisThread::sleep_for(5000);
        hidReport.BUTTON_A = 0xFF;
        gamepad.SendReport(&hidReport);
        ThisThread::sleep_for(300);
        hidReport.BUTTON_A = 0x00;
        gamepad.SendReport(&hidReport);
    }
}