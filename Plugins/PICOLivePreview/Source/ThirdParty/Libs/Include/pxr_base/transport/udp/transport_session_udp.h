#pragma once
#include <memory>
#include <mutex>
#include <thread>

#include "asio2/asio/asio.hpp"
#include "pxr_base/transport/transport_session.h"
#include "pxr_base/task_queue/task_queue_boost.h"

namespace pxr_base
{
    class TransportSessionUdp :public TransportSession
    {
    public:
        TransportSessionUdp();
        ~TransportSessionUdp();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() override;
        virtual Config config() override;
        virtual bool running() override;
    private:
        void StartAyncRecv();
        void HandleReceiveData(std::shared_ptr<TransportData> data);
    private:
        std::recursive_mutex mutex_;
        TaskQueueBoost task_queue_;
        asio::ip::udp::socket socket_;
        Config config_;
        asio::ip::udp::endpoint source_endpoint_;
    };
}