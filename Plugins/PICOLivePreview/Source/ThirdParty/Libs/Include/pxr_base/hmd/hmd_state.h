#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API  HmdState
    {
    public:
        HmdState() = default;
        ~HmdState() = default;
        HmdState(const HmdState& other);

        uint8_t GetBatteryPercent_() const;
        void SetBatteryPercent_(uint8_t val);

        void Reset();

        uint8_t battery_percent() const;
        uint8_t& battery_percent();

        HmdState& operator=(const HmdState& other);

    private:
        uint8_t battery_percent_ = 0;
    };
}