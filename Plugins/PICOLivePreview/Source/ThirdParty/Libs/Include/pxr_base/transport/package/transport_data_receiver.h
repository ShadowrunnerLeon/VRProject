#pragma once
#include "pxr_base/transport/transport_data.h"
#include "pxr_base/transport/transport_common.h"
#include "pxr_base/transport/data_buffer_reader.h"

namespace pxr_base 
{
    class TransportDataReceiver
    {
    public:
        class ReceiverSink 
        {
        public:
            virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
                const DataBuffer& data) = 0;
            virtual ~ReceiverSink() = default;
        };

        TransportDataReceiver();
        ~TransportDataReceiver();

        static const size_t kPayloadBufferSize = 5 * 1024 * 1024;
        void UpdateConfig(bool enable_channel, bool enable_packet_len, ReceiverSink* sink);
        bool ProcessReceivedData(uint8_t* data, size_t length);
    private:
        void ReadChannelId(DataBufferReader& reader);
        size_t ReadPacketLength(DataBufferReader& reader);
        void SubmitBuffer();
    private:
        bool enable_channel_ = false;
        bool enable_packet_len_ = false;
        ReceiverSink* sink_ = nullptr;
        DataBuffer packet_length_buffer_;
        TransportChannelId channel_id_ = kInvalidChannelId;
        DataBuffer buffer_;
    };
}