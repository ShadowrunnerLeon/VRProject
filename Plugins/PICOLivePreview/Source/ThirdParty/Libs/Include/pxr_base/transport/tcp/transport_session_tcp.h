#pragma once
#include <memory>
#include <mutex>

#include "pxr_base/transport/transport_session.h"

namespace pxr_base
{
    class TransportSessionTcpImpl;

    class TransportSessionTcp :public TransportSession
    {
    public:
        typedef std::shared_ptr<TransportSessionTcpImpl> ImplPtr;

        TransportSessionTcp(ImplPtr& impl);
        ~TransportSessionTcp();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() override;
        virtual Config config() override;
        virtual bool running() override;
    private:
        std::recursive_mutex mutex_;
        ImplPtr impl_;
    };
}