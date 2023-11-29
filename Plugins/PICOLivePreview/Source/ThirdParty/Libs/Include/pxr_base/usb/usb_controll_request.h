#pragma once
#include <stdint.h>

#include "pxr_base/usb/usb_common_define.h"
#include "pxr_base/transport/data_buffer.h"

namespace pxr_base
{
    class UsbControllRequest
    {
    public:
        enum class RequestType
        {
            kStandard = 0x00,
            kClass = 0x01,
            kVendor = 0x10,
            kUndef = 0x11
        };

        enum class Recipient
        {
            kDevice = 0,
            kInterface = 1,
            kEndpoint = 2,
            kOther = 3
        };

        virtual uint8_t bmRequestType() const;

        UsbTransferDirection direction_ = UsbTransferDirection::kHostToDevice;
        RequestType type_ = RequestType::kVendor;
        Recipient recipient_ = Recipient::kDevice;

        uint8_t request_ = 0;
        uint16_t value_ = 0;
        uint16_t index_ = 0;
        DataBuffer data_;
    };
}