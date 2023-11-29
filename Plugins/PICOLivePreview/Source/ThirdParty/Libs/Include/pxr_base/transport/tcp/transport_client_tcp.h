#pragma once

#include <stdint.h>
#include <string>
#include <string_view>

#include "asio2/asio2.hpp"
#include "pxr_base/transport/package/transport_data_receiver.h"
#include "pxr_base/transport/package/transport_data_sender.h"
#include "pxr_base/transport/transport_client.h"

namespace pxr_base
{
    class TransportClientTcp:public TransportClient,
        public TransportDataSender::SenderSink,
        public TransportDataReceiver::ReceiverSink
    {
    public:
        TransportClientTcp();
        ~TransportClientTcp();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() const override;
        virtual const Config& config() const override;
        virtual bool running() const override;
    private:
        void InitAsio2Client();
        void OnConnect(asio::error_code ec);
        void OnReceivedMessage(std::string_view data);
        void OnDisconnect(asio::error_code ec);
        void SetCallback(TransportClientCallback* val);

        virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) override;
        virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
            const DataBuffer& data) override;
    private:
        Config config_;
        asio2::tcp_client client_;
        TransportDataSender sender_;
        TransportDataReceiver receiver_;
        std::recursive_mutex mutex_;
        TransportClientCallback* callback_ = nullptr;
    };
}

