#pragma once
#include <memory>
#include <mutex>

#include "pxr_base/transport/transport_server.h"

namespace pxr_base
{
    class TransportSession;
    class TransportServerTcpImpl;

    class TransportServerTcp :public TransportServer
    {
    public:
        TransportServerTcp();
        ~TransportServerTcp();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;

        virtual TransportType type() const override;
        virtual Config config() const override;
        virtual bool running() const override;
    private:
        std::shared_ptr<TransportServerTcpImpl> impl_;
        std::recursive_mutex mutex_;
        Config config_;
        std::atomic_bool running_;
    };
}