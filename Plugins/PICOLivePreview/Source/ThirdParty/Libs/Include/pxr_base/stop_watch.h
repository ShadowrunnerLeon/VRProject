#pragma once
#include "pxr_base/pxr_exports.h"

#include <stdint.h>
#include <memory>

namespace pxr_base
{
    class PXR_API StopWatch
    {
    public:
        //Start when created, and high resolution for microseconds, other use milliseconds.
        static std::shared_ptr<StopWatch> Create(bool high_resolution = false);
        //Reset start time point.
        virtual void Reset() = 0;
        //Return duration since last pause or start time point.
        virtual uint64_t Pause() = 0;
        //Return duration since start time point.
        virtual uint64_t Stop() = 0;
        virtual ~StopWatch() = default;
    };
}