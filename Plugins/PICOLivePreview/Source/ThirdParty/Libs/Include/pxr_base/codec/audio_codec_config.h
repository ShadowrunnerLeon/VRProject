#pragma once

#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base {

    enum class AudioCodecType {

        kUndefinedAudioCodecType = 0,
        kPcm,
    };

    class PXR_API AudioCodecConfig
    {
    public:
        AudioCodecConfig() = default;
        ~AudioCodecConfig() = default;
        AudioCodecConfig(const AudioCodecConfig& other);

        AudioCodecType GetCodecType_() const;
        void SetCodecType_(AudioCodecType type);

        uint32_t GetChannels_() const;
        void SetChannels_(uint32_t channels);

        uint32_t GetSampleDepth_() const;
        void SetSampleDepth_(uint32_t depth);

        uint32_t GetSampleRate_() const;
        void SetSampleRate_(uint32_t rate);

        AudioCodecType codec_type() const;
        AudioCodecType& codec_type();

        uint32_t channels() const;
        uint32_t& channels();

        uint32_t sample_depth() const;
        uint32_t& sample_depth();

        uint32_t sample_rate() const;
        uint32_t& sample_rate();

        AudioCodecConfig& operator=(const AudioCodecConfig& other);
        bool operator==(const AudioCodecConfig& other);
        bool operator!=(const AudioCodecConfig& other);
    private:
        AudioCodecType codec_type_ = AudioCodecType::kUndefinedAudioCodecType;
        uint32_t channels_ = 0;
        uint32_t sample_depth_ = 0;
        uint32_t sample_rate_ = 0;
    };
}


