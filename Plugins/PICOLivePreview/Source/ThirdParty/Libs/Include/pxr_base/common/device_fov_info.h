#pragma once
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    enum class EyeType
    {
        kEyeLeft = 0,
        kEyeRight = 1,
        kEyeBoth = 2,
    };

    struct PXR_API DeviceFovInfo
    {
        EyeType eye_type = EyeType::kEyeBoth;
        float left = 0.;
        float right = 0.;
        float up = 0.;
        float down = 0.;
    };
}