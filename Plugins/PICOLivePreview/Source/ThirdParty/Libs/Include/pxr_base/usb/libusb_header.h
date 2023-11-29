#pragma once
#ifdef  _MSC_VER
//Note(by Alan Duan): VS Compiler not support zero array.
#pragma warning(disable : 4200)
#endif //  _MSC_VER
#include "thirdparty/libusb/include/libusb-1.0/libusb.h"
#define PXR_USB_DEBUG 0