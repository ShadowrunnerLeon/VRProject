#pragma once
#include <stdint.h>
#include <string>
#include <string_view>

#include "asio/asio.hpp"
#include "pxr_base/transport/package/transport_data_receiver.h"
#include "pxr_base/transport/package/transport_data_sender.h"
#include "pxr_base/transport/transport_client.h"
#include "pxr_base/task_queue/task_queue_boost.h"

namespace pxr_base
{
    class TransportClientTcp2 :public TransportClient,
        public TransportDataSender::SenderSink,
        public TransportDataReceiver::ReceiverSink
    {
    public:
        TransportClientTcp2();
        ~TransportClientTcp2();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() const override;
        virtual const Config& config() const override;
        virtual bool running() const override;
    private:
        void StartAyncRecv();
        void HandleReceiveData(size_t bytes_recved);
        void HandleConnect(asio::error_code err);
        virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) override;
        virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
            const DataBuffer& data) override;
        void HandleAsyncError(asio::error_code ec);
    private:
        static const size_t kDefaultReceiveBufferSize = 1024 * 1024;
        TaskQueueBoost task_queue_;
        asio::ip::tcp::socket socket_;
        std::recursive_mutex mutex_;
        std::atomic_bool connected_;
        Config config_;
        TransportDataSender sender_;        
        TransportDataReceiver receiver_;
        DataBuffer recv_buffer_;
    };
}