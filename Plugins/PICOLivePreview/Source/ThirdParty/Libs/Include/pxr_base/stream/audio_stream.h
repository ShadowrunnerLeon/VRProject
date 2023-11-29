#pragma once

#include "pxr_base/pxr_exports.h"
#include "pxr_base/codec/audio_codec_config.h"

#include "pxr_base/stream/packet_config.h"
#include "pxr_base/stream/connection_config.h"

namespace pxr_base
{
    class PXR_API AudioStream {
    public:
        AudioStream() = default;
        AudioStream(const AudioStream& other);

        AudioStream& operator=(const AudioStream& other);

        int32_t GetStreamId_() const;
        void SetStreamId_(int32_t id);

        AudioCodecConfig GetAudioCodecConfig_() const;
        void SetAudioCodecConfig_(const AudioCodecConfig& codec_config);

        ConnectionConfig GetConnectionConfig_() const;
        void SetConnectionConfig_(const ConnectionConfig& conn_config);

        PacketConfig GetTransportConfig_() const;
        void SetTransportConfig_(const PacketConfig& tpt_config);


        int32_t stream_id() const;
        int32_t& stream_id();

        const AudioCodecConfig& audio_codec_config() const;
        AudioCodecConfig& audio_codec_config();

        const ConnectionConfig& connection_config() const;
        ConnectionConfig& connection_config();

        const PacketConfig& packet_config() const;
        PacketConfig& packet_config();

    private:
        int32_t stream_id_ = 0;
        AudioCodecConfig audio_codec_config_;
        ConnectionConfig connection_config_;
        PacketConfig packet_config_;
    };
}


