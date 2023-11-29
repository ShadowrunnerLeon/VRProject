#pragma once
#include "pxr_base/transport/transport_common.h"
#include "pxr_base/transport/transport_data.h"

namespace pxr_base 
{

    class TransportDataSender
    {
    public:
        class SenderSink
        {
        public:    
            virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) = 0;
            virtual ~SenderSink() = default;
        };

        TransportDataSender();
        ~TransportDataSender();

        void UpdateConfig(bool enable_channel, bool enable_packet_len, SenderSink* sink);
        bool SendData(const TransportData& data);
    private:
        bool enable_channel_ = false;
        bool enable_packet_len_ = false;
        SenderSink* sink_ = nullptr;
    };
}