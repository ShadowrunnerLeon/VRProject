#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    struct PXR_API UsbDeviceDescriptorInfo
    {
        uint16_t vendor_id = 0;
        uint16_t product_id = 0;
        uint8_t interface_num = 0;
        uint8_t interface_class = 0;
        uint8_t interface_sub_class = 0;
        uint8_t interface_protocol = 0;
    };
}