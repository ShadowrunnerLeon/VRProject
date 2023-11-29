#pragma once

#include <memory>
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/transport/network_address.h"

namespace pxr_base
{
    enum class ConnectionType {

        kUndefinedConnectionType = 0,
        kUdp,
        kTcp,
        kAoa,
    };

    enum class Direction {

        kUndefinedDirection = 0,
        kSend,
        kReceive,
        kSendAndReceive
    };

    enum class SetupRole {
        kClient = 0,
        kServer = 1,
        kBoth = 2
    };

    class PXR_API ConnectionConfig {
    public:
        ConnectionConfig();
        ConnectionConfig(const ConnectionConfig& config);

        ConnectionType GetConnectionType_() const;
        void SetConnectionType_(ConnectionType type);

        pxr_base::NetworkAddress GetAddress_() const;
        void SetAddress_(const pxr_base::NetworkAddress& addr);

        Direction GetDirection_() const;
        void SetDirection_(Direction drt);


        ConnectionType connection_type() const;
        ConnectionType& connection_type();

        const NetworkAddress& addr() const;
        NetworkAddress& addr();

        Direction direction() const;
        Direction& direction();

        SetupRole role() const;
        SetupRole& role();

        uint32_t channel() const;
        uint32_t& channel();

        ConnectionConfig& operator=(const ConnectionConfig& other);
    private:
        ConnectionType connection_type_;
        pxr_base::NetworkAddress addr_;
        Direction direction_;
        SetupRole role_ = SetupRole::kBoth;
        uint32_t channel_ = 0;
    };
};

