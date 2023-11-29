#pragma once

#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base {
    class PXR_API EncodeStatisticsData
    {
    public:

        int64_t GetEyeIndex_() const;
        void SetEyeIndex_(int64_t eye_index);

        float GetEyeTimedelay_(int64_t index) const;
        void SetEyeTimedelay_(int64_t index, float eye_timedelay);

        void Reset();

    private:
        int64_t eye_index_;
        float eye_timedelay_[2] = { 0, 0 };
    };
}



