#pragma once
#include <memory>
#include <mutex>

#include "asio/asio.hpp"
#include "pxr_base/transport/package/transport_data_receiver.h"
#include "pxr_base/transport/package/transport_data_sender.h"
#include "pxr_base/transport/transport_session.h"
#include "pxr_base/task_queue/task_queue_boost.h"

namespace pxr_base
{
    class TransportSessionTcp2 :public TransportSession,
        public TransportDataSender::SenderSink,
        public TransportDataReceiver::ReceiverSink
    {
    public:
        TransportSessionTcp2(std::shared_ptr<TaskQueueBoost> task_queue,
            std::shared_ptr<asio::ip::tcp::socket> socket);
        ~TransportSessionTcp2();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() override;
        virtual Config config() override;
        virtual bool running() override;
    private:
        virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) override;
        virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
            const DataBuffer& data) override;
        bool StartAyncRecv();
        void HandleReceiveData(size_t bytes_recved);
        void HandleAsyncError(asio::error_code ec);
        void ConvertAddress(const asio::ip::tcp::endpoint& source, NetworkAddress& addr);
    private:
        static const size_t kDefaultReceiveBufferSize = 1024 * 1024;
        std::shared_ptr<TaskQueueBoost> task_queue_;
        std::shared_ptr<asio::ip::tcp::socket> socket_;
        std::recursive_mutex mutex_;
        Config config_;
        std::atomic_bool running_ = false;
        TransportDataSender sender_;
        TransportDataReceiver receiver_;
        DataBuffer recv_buffer_;
    };
}