#pragma once
#include <stdint.h>

#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API TimeUtil
    {
    public:
        static int64_t TimestampNs(void);
        static int64_t TimestampUs(void);
        static int64_t TimestampMs(void);
    };

    class PXR_API ThreadHelper
    {
    public:
        static void sleep(uint64_t ms);
    };
}