#pragma once

#include <memory>
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/transport/transport_data.h"
#include "pxr_base/transport/network_address.h"

namespace pxr_base
{
    class PXR_API TransportClientCallback 
    {
    public:
        virtual void NotifyConnectResult(int result) = 0;
        virtual void NotifyDisconnect(int result) = 0;
        virtual void NotifyRececiveData(const TransportData& data) = 0;
    protected:
        virtual ~TransportClientCallback() {}
    };

    class PXR_API TransportClient
    {
    public:
        struct PXR_API Config
        {
            bool enable_packet = false;
            bool enable_channel = false;
            NetworkAddress local_addr;
            NetworkAddress server_addr;
            TransportClientCallback* callback = nullptr;
        };

        typedef std::shared_ptr<TransportClient> Ptr_t;

        static  Ptr_t Create(TransportType type);

        virtual bool Start(const Config& config) = 0;
        virtual void Stop() = 0;
        virtual bool SendData(const TransportData& data) = 0;

        virtual TransportType type() const = 0;
        virtual const Config& config() const = 0;
        virtual bool running() const = 0;

        virtual ~TransportClient() {}
    };
}