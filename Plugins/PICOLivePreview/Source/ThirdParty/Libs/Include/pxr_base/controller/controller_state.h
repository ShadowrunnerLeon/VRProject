#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    enum class ControllerType
    {
        kControllerUndef = -1,
        kControllerPicoOculus = 0,
        kControllerOculus = 1,
        kControllerHTCVive = 2,
        kControllerPreview = 3
    };

    class PXR_API ControllerState
    {
    public:
        ControllerState() = default;
        ~ControllerState() = default;

        ControllerState(const ControllerState& other);

        bool GetActive_() const;
        void SetActive_(bool val);

        int64_t GetIndex_() const;
        void SetIndex_(int64_t val);

        uint8_t GetBatteryPercent_() const;
        void SetBatteryPercent_(uint8_t val);

        void Set_(bool active, int64_t index, uint8_t battery);
        void Reset();


        bool active() const;
        bool& active();

        int64_t index() const;
        int64_t& index();

        uint8_t battery_percent() const;
        uint8_t& battery_percent();

        ControllerState& operator=(const ControllerState& other);

    private:
        bool active_ = false;
        int64_t index_ = 0;
        uint8_t battery_percent_ = 0;
    };
}