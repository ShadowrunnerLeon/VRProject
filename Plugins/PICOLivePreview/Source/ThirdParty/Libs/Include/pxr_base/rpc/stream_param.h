#pragma once
#include <memory>
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/codec/video_codec_config.h"
#include "pxr_base/codec/audio_codec_config.h"
#include "pxr_base/transport/transport_common.h"
#include "pxr_base/rpc/connection_config.h"

namespace pxr_base
{
    struct RtpParam {

        uint8_t payload_type = 255;
        uint32_t ssrc = 0;
    };

    enum class PackageType {
        kUndefined = 0,
        kRtp,
        kRaw,
        kDynamic,
    };

    struct PackageConfig {
        PackageType package_type = PackageType::kUndefined;
        RtpParam rtp_param;
    };

    class PXR_API VideoStream {
    public:
        VideoStream() = default;
        VideoStream(const VideoStream& other);

        VideoStream& operator=(const VideoStream& other);

        int32_t GetStreamId_() const;
        void SetStreamId_(int32_t id);

        pxr_base::VideoCodecConfig GetVideoCodecConfig_() const;
        void SetVideoCodecConfig_(const pxr_base::VideoCodecConfig& codec_config);

        ConnectionConfig GetConnectionConfig_() const;
        void SetConnectionConfig_(const ConnectionConfig& conn_config);

        PackageConfig GetPackageConfig_() const;
        void SetPackageConfig_(const PackageConfig& tpt_config);


        int32_t stream_id() const;
        int32_t& stream_id();

        const pxr_base::VideoCodecConfig& video_codec_config() const;
        pxr_base::VideoCodecConfig& video_codec_config();

        const ConnectionConfig& connection_config() const;
        ConnectionConfig& connection_config();

        const PackageConfig& package_config() const;
        PackageConfig& package_config();

    private:
        int32_t stream_id_ = 0;
        pxr_base::VideoCodecConfig video_codec_config_;
        ConnectionConfig connection_config_;
        PackageConfig transport_config_;
    };

    class PXR_API AudioStream {
    public:
        AudioStream() = default;
        AudioStream(const AudioStream& other);

        AudioStream& operator=(const AudioStream& other);

        int32_t GetStreamId_() const;
        void SetStreamId_(int32_t id);

        pxr_base::AudioCodecConfig GetAudioCodecConfig_() const;
        void SetAudioCodecConfig_(const pxr_base::AudioCodecConfig& codec_config);

        ConnectionConfig GetConnectionConfig_() const;
        void SetConnectionConfig_(const ConnectionConfig& conn_config);

        PackageConfig GetTransportConfig_() const;
        void SetTransportConfig_(const PackageConfig& tpt_config);


        int32_t stream_id() const;
        int32_t& stream_id();

        const pxr_base::AudioCodecConfig& audio_codec_config() const;
        pxr_base::AudioCodecConfig& audio_codec_config();

        const ConnectionConfig& connection_config() const;
        ConnectionConfig& connection_config();

        const PackageConfig& packet_config() const;
        PackageConfig& packet_config();

    private:
        int32_t stream_id_ = 0;
        pxr_base::AudioCodecConfig audio_codec_config_;
        ConnectionConfig connection_config_;
        PackageConfig packet_config_;
    };


    class PXR_API StreamParam
    {
    public:
        StreamParam();
        StreamParam(const StreamParam& param);

        //StreamParam(StreamParam&& param) noexcept;
        explicit StreamParam(size_t vsize, size_t vcapacity,
            size_t asize, size_t acapacity);
        ~StreamParam();
        StreamParam& operator=(const StreamParam& other);

        const VideoStream* video_stream(size_t index) const;
        VideoStream* video_stream(size_t index);
        const AudioStream* audio_stream(size_t index) const;
        AudioStream* audio_stream(size_t index);

        size_t vsize() const;
        size_t asize() const;

        void add_video_stream(const VideoStream& video_stream);
        void add_audio_stream(const AudioStream& audio_stream);

    private:
        void EnsureCapacityAdequate(size_t vcapacity, size_t acapacity);

    private:
        std::unique_ptr<VideoStream[]> video_streams_;
        size_t vsize_ = 0;
        size_t vcapacity_ = 0;

        std::unique_ptr<AudioStream[]> audio_streams_;
        size_t asize_ = 0;
        size_t acapacity_ = 0;
    };
};

