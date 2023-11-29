#pragma once
#include <memory>
#include <mutex>

#include "asio2/asio2.hpp"
#include "pxr_base/transport/package/transport_data_receiver.h"
#include "pxr_base/transport/package/transport_data_sender.h"
#include "pxr_base/transport/transport_session.h"

namespace pxr_base
{
    typedef std::shared_ptr<asio2::tcp_session> Asio2TcpSession;
    typedef asio2::tcp_session::key_type Asio2SessionKey;

    class TransportServerTcpImpl;
    class TransportSessionTcpImpl:
        public TransportDataSender::SenderSink,
        public TransportDataReceiver::ReceiverSink
    {
    public:
        typedef std::shared_ptr<TransportServerTcpImpl> SeverImplPtr;
        typedef TransportSession::Config Config;

        TransportSessionTcpImpl(const Asio2TcpSession& session,
            SeverImplPtr& server);
        ~TransportSessionTcpImpl();

        bool Start(const Config& config);
        void Stop();

        bool running();
        Config config();
        bool SendData(const TransportData& data);
        void NotifyReceiveData(std::string_view data);
        void NotifySessionDisconnect(int res);
    private:
        friend class TransportSessionTcp;
        virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) override;
        virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
            const DataBuffer&data) override;
        void Release();
    private:
        bool running_ = false;
        Asio2TcpSession asio2_session_;
        SeverImplPtr server_;
        TransportDataSender sender_;
        TransportDataReceiver receiver_;
        std::recursive_mutex mutex_;
        Config config_;
    };
}