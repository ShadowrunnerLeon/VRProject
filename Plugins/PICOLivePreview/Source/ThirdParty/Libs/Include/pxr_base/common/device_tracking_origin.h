#pragma once
namespace pxr_base
{
    enum class DeviceTrackingOrigin
    {
        kUndefined = -1,
        kEyeLevel = 0,
        kFloorLevel = 1,
        kStageLevel = 2
    };
}