#pragma once
#pragma once

#include "pxr_base/transport/transport_session.h"
#include "asio2/asio/asio.hpp"
#include "pxr_base/task_queue/task_queue_boost.h"
#include "pxr_base/transport/package/transport_data_receiver.h"
#include "pxr_base/transport/package/transport_data_sender.h"
#include <string>

namespace pxr_base
{
    class TransportSessionFileDescriptor :public TransportSession,
        public TransportDataSender::SenderSink,
        public TransportDataReceiver::ReceiverSink
    {
    public:
        TransportSessionFileDescriptor();
        ~TransportSessionFileDescriptor();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() override;
        virtual Config config() override;
        virtual bool running() override;
    private:
        static const size_t kSendBufferPacketNum = 128;
        //Note(by Alan Duan): 2M for receive buffer;
        static const size_t kRecvBufferSize = 2 * 1024 * 1024;

        virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) override;
        virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
            const DataBuffer& data) override;

        bool OpenFileDescriptor(const Config& config);
        void StartAyncRecv();
        bool AsyncReceive();
        void AsyncSend(const DataBuffer& data);
    private:
        typedef TransportSessionCallback* CallbackPtr;
        void SafeCallback(std::function<void(CallbackPtr)> f);
        void HandleSessionError();
        inline bool SetConfig(const Config& config);
    private:
        std::atomic_bool running_;
        Config config_;
        std::recursive_mutex mutex_;
        int file_descriptor_ = -1;
        //Note(by Alan Duan): Sender running in user thread.
        TransportDataSender sender_;
        std::shared_ptr<TaskQueue> send_queue_;
        //Note(by Alan Duan): Receiver running in receive queue thread.
        TransportDataReceiver receiver_;
        std::shared_ptr<TaskQueue> recv_queue_;
        std::shared_ptr<DataBuffer> recv_buffer_;
    };
}