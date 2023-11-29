#pragma once

namespace pxr_base
{
    enum class UsbTransferDirection
    {
        kHostToDevice = 0,
        kDeviceToHost = 0x80
    };

    enum class UsbTransferType
    {
        kControl = 0,
        kIoschronous = 1,
        kBulk = 2,
        kInterrupt = 3
    };
}