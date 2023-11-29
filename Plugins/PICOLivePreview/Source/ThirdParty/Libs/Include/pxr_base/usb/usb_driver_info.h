#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    struct PXR_API UsbDriverInfo
    {
        uint16_t vendor_id = 0;
        uint16_t product_id = 0;
    };
}