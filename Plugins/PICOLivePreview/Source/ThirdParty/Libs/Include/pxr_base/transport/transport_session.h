#pragma once

#include "pxr_base/pxr_exports.h"
#include "pxr_base/transport/network_address.h"
#include "pxr_base/transport/transport_common.h"
#include "pxr_base/transport/transport_data.h"

namespace pxr_base
{
    class PXR_API TransportSessionCallback
    {
    public:
        virtual void NotifySessionStopped(int res) = 0;
        virtual void NotifyReceiveData(const TransportData& data) = 0;
    protected:
        virtual ~TransportSessionCallback() {}
    };

    class PXR_API TransportSession
    {
    public:
        typedef std::shared_ptr<TransportSession> Ptr_t;
        struct PXR_API Config
        {
            TransportSessionCallback* callback = nullptr;

            // Add a header before data sent when theses flag opened. 
            // Supported for UDP, TCP, File Descriptor and AOA transport.
            //  
            //  |  channel_id(1 bytes) | packet length(4 bytes)  |
            // 
            bool enable_packet = false;
            // User can get or set the channel id in TransportData.
            bool enable_channel = false;

            // Open the udp broadcast, setting the destination port in remote_addr.
            bool udp_broadcast = false;
            // For TCP: [out] Get the client and server address from config() function.
            // For UDP: [in]  Bind the ip and port in local addr. If the remote_addr is valid,
            //               it will be used to send data.
            //          [out] Get the local and remote address from config() function.
            NetworkAddress local_addr;
            NetworkAddress remote_addr;
            //For file descriptor transport.
            DataBuffer file_path;
        };

        virtual bool Start(const Config& config) = 0;
        virtual void Stop() = 0;

        virtual bool SendData(const TransportData& data) = 0;

        virtual TransportType type() = 0;
        virtual Config config() = 0;
        virtual bool running() = 0;

        virtual ~TransportSession() {}
    };

    class PXR_API TransportSessionFactory
    {
    public:
        struct PXR_API Config
        {
            TransportType type = TransportType::kUndefined;
        };
        //Note(by Alan Duan): Now we only support udp and file descriptor session.
        static TransportSession::Ptr_t CreateSession(const Config& config);
    };
}