#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base {
    enum class VideoCodecType 
    {
        kUndefined = 0,
        kH264,
        kH265,
    };

    class PXR_API VideoCodecConfig
    {
    public:
        VideoCodecConfig() = default;
        ~VideoCodecConfig() = default;
        VideoCodecConfig(const VideoCodecConfig& other);

        VideoCodecType GetCodecType_() const;
        void SetCodecType_(VideoCodecType type);

        int32_t GetWidth_() const;
        void SetWidth_(int32_t width);

        int32_t GetHeight_() const;
        void SetHeight_(int32_t height);

        int32_t GetBitrate_() const;
        void SetBitrate_(int32_t bitrate);

        int32_t GetFps_() const;
        void SetFps_(int32_t fps);


        VideoCodecType codec_type() const;
        VideoCodecType& codec_type();

        int32_t width() const;
        int32_t& width();

        int32_t height() const;
        int32_t& height();

        int32_t bitrate() const;
        int32_t& bitrate();

        int32_t fps() const;
        int32_t& fps();

        VideoCodecConfig& operator=(const VideoCodecConfig& other);

    private:
        VideoCodecType codec_type_ = VideoCodecType::kUndefined;
        int32_t width_ = 0;
        int32_t height_ = 0;
        int32_t bitrate_ = 0;
        int32_t fps_ = 0;
    };
}


