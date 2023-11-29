#pragma once

#include "pxr_base/pxr_exports.h"
#include "pxr_base/codec/video_codec_config.h"

#include "pxr_base/stream/packet_config.h"
#include "pxr_base/stream/connection_config.h"

namespace pxr_base
{

    class PXR_API VideoStream {
    public:
        VideoStream() = default;
        VideoStream(const VideoStream& other);

        VideoStream& operator=(const VideoStream& other);

        int32_t GetStreamId_() const;
        void SetStreamId_(int32_t id);

        VideoCodecConfig GetVideoCodecConfig_() const;
        void SetVideoCodecConfig_(const VideoCodecConfig& codec_config);

        ConnectionConfig GetConnectionConfig_() const;
        void SetConnectionConfig_(const ConnectionConfig& conn_config);

        PacketConfig GetPacketConfig_() const;
        void SetPacketConfig_(const PacketConfig& tpt_config);

        int32_t stream_id() const;
        int32_t& stream_id();

        const VideoCodecConfig& video_codec_config() const;
        VideoCodecConfig& video_codec_config();

        const ConnectionConfig& connection_config() const;
        ConnectionConfig& connection_config();

        const PacketConfig& package_config() const;
        PacketConfig& package_config();
    private:
        int32_t stream_id_ = 0;
        VideoCodecConfig video_codec_config_;
        ConnectionConfig connection_config_;
        PacketConfig package_config_;
    };

}


