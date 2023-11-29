#pragma once
#include <mutex>
#include <stdint.h>
#include <string>
#include <string_view>

#include "asio2/asio2.hpp"
#include "pxr_base/transport/transport_server.h"
#include "pxr_base/transport/tcp/transport_session_tcp_mgr.h"

namespace pxr_base
{
    class TransportServerTcp;
    class TransportServerTcpImpl
        :public std::enable_shared_from_this<TransportServerTcpImpl>,
        public TransportServer
    {
    public:
        TransportServerTcpImpl();
        ~TransportServerTcpImpl();
        //Called By Server User.
        virtual bool Start(const Config& config) override;
        virtual void Stop() override;

        virtual TransportType type() const override;
        virtual Config config() const override;
        virtual bool running() const override;
    private:
        friend class TransportSessionTcpImpl;
        void InitAsio2Server();

        typedef std::shared_ptr<asio2::tcp_session> Asio2TcpSession;
        void OnServerStarted(asio::error_code ec);
        void OnClientConnected(Asio2TcpSession& asio2_session);
        void OnClientDisconnected(Asio2TcpSession& asio2_session);
        void OnReceivedMessage(Asio2TcpSession& asio2_session, std::string_view data);
        void Reset();
        void ReleaseTransportSession(Asio2SessionKey session_key);
    private:
        asio2::tcp_server server_;
        std::recursive_mutex mutex_;
        bool running_ = false;
        TransportServerCallback* callback_ = nullptr;
        TransportSessionTcpManager session_mgr_;
    };
}