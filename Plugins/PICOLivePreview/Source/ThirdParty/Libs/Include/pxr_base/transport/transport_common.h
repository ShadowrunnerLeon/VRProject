#pragma once

#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    enum class TransportType
    {
        kUndefined = -1,
        kTcp = 0,
        kUdp = 1,
        kAoa = 2,
        kFileDescriptor = 3,
        kUsb = 4
    };

    typedef uint8_t TransportChannelId;
    static const TransportChannelId kInvalidChannelId = 0xFF;

    const char* ToString(TransportType type);
}