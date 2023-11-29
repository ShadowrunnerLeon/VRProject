#pragma once
#include <mutex>
#include <stdint.h>
#include <string>
#include <string_view>

#include "asio/asio.hpp"
#include "pxr_base/task_queue/task_queue_boost.h"
#include "pxr_base/transport/transport_server.h"

namespace pxr_base
{
    class TransportServerTcp2:public TransportServer
    {
    public:
        TransportServerTcp2();
        ~TransportServerTcp2();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;

        virtual TransportType type() const override;
        virtual Config config() const override;
        virtual bool running() const override;
    private:
        bool StartAccept();
        bool CreateTransportSession(std::shared_ptr<TaskQueueBoost> task_queue,
            std::shared_ptr<asio::ip::tcp::socket> socket);
    private:
        std::recursive_mutex mutex_;
        std::atomic_bool running_ = false;
        Config config_;
        asio::ip::tcp::endpoint local_endpoint_;
        TaskQueueBoost task_queue_;
        asio::ip::tcp::acceptor acceptor_;
    };
}