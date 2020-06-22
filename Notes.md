# Xbox Controller USB Descriptors

Information for device USB\Vendor_FFFF_Product_FFFF:

Connection Information:
------------------------------
Device current bus speed: FullSpeed
Device supports USB 1.1 specification
Device address: 0x0003
Current configuration value: 0x00
Number of open pipes: 0

Device Descriptor:
------------------------------
0x12	bLength
0x01	bDescriptorType
0x0110	bcdUSB
0x00	bDeviceClass      
0x00	bDeviceSubClass   
0x00	bDeviceProtocol   
0x40	bMaxPacketSize0   (64 bytes)
0xFFFF	idVendor
0xFFFF	idProduct
0x0100	bcdDevice
0x00	iManufacturer
0x00	iProduct     
0x00	iSerialNumber
0x01	bNumConfigurations

Configuration Descriptor:
------------------------------
0x09	bLength
0x02	bDescriptorType
0x0020	wTotalLength   (32 bytes)
0x01	bNumInterfaces
0x01	bConfigurationValue
0x00	iConfiguration
0x80	bmAttributes   (Bus-powered Device)
0x32	bMaxPower      (100 mA)

Interface Descriptor:
------------------------------
0x09	bLength
0x04	bDescriptorType
0x00	bInterfaceNumber
0x00	bAlternateSetting
0x02	bNumEndPoints
0x58	bInterfaceClass      
0x42	bInterfaceSubClass   
0x00	bInterfaceProtocol   
0x00	iInterface

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x82	bEndpointAddress  (IN endpoint 2)
0x03	bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
0x0020	wMaxPacketSize    (1 x 32 bytes)
0x04	bInterval         (4 frames)

Endpoint Descriptor:
------------------------------
0x07	bLength
0x05	bDescriptorType
0x02	bEndpointAddress  (OUT endpoint 2)
0x03	bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
0x0020	wMaxPacketSize    (1 x 32 bytes)
0x04	bInterval         (4 frames)

Microsoft OS Descriptor is not available. Error code: 0x0000001F

String Descriptor Table
--------------------------------
Index  LANGID  String
0x00   0x0000  

------------------------------

Connection path for device: 
USB xHCI Compliant Host Controller
Root Hub
Generic USB Hub
USB\Vendor_FFFF_Product_FFFF Port: 1