#pragma once
#include <stdint.h>
#include "pxr_base/controller/controller_state.h"
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API DriverMediaConfig
    {
    public:
        DriverMediaConfig() = default;
        ~DriverMediaConfig() = default;

        DriverMediaConfig(const DriverMediaConfig& other);
        DriverMediaConfig& operator=(const DriverMediaConfig& other);

        int32_t GetRenderWidth_() const;
        void SetRenderWidth_(int32_t val);

        int32_t GetRenderHeight_() const;
        void SetRenderHeight_(int32_t val);

        int32_t GetFrameRate_() const;
        void SetFrameRate_(int32_t val);

        void Set_(int32_t width, int32_t height, int32_t frame_rate);
        void Reset();

        int32_t render_width() const;
        int32_t& render_width();

        int32_t render_height() const;
        int32_t& render_height();

        int32_t frame_rate() const;
        int32_t& frame_rate();
    private:
        int32_t render_width_ = 0;
        int32_t render_height_ = 0;
        int32_t frame_rate_ = 0;
    };
}