#pragma once
#include <mutex>

#include "pxr_base/transport/transport_server.h"

namespace pxr_base
{
    class TransportServerUsbImpl;
    class TransportServerUsb :public TransportServer
    {
    public:
        TransportServerUsb();
        ~TransportServerUsb();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;

        virtual TransportType type() const override;
        virtual Config config() const override;
        virtual bool running() const override;
    private:
        std::recursive_mutex mutex_;
        Config config_;
        std::shared_ptr<TransportServerUsbImpl> impl_;
    };
}