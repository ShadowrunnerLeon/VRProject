#pragma once

#include<stdint.h>

namespace pxr_base
{
    struct RtpParam {

        uint8_t payload_type = 255;
        uint32_t ssrc = 0;
    };

    enum class PacketType {
        kUndefined = 0,
        kRtp,
        kRaw,
        kDynamic,
    };

    struct PacketConfig {
        PacketType package_type = PacketType::kUndefined;
        RtpParam rtp_param;
    };
}