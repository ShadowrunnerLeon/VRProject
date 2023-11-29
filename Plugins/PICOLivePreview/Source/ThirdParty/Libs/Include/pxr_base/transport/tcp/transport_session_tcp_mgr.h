#pragma once
#include <memory>

#include "pxr_base/transport/tcp/transport_session_tcp_impl.h"

namespace pxr_base
{
    class TransportServerTcpImpl;
    class TransportSessionTcpManager
    {
    public:
        typedef TransportSessionTcpImpl Session;
        typedef std::shared_ptr<Session> SessionPtr;
        typedef std::shared_ptr<TransportServerTcpImpl> ServerPtr;

        SessionPtr Add(Asio2TcpSession& asio2_session, ServerPtr server);
        SessionPtr Get(Asio2SessionKey key);
        void Release(Asio2SessionKey key);
        void Reset();
    private:
        std::unordered_map<Asio2SessionKey, SessionPtr> session_map_;
    };
}