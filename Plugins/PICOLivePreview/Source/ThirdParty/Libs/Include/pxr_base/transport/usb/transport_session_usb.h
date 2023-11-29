#pragma once
#include "pxr_base/mutex_lock.h"
#include "pxr_base/task_queue/task_queue.h"
#include "pxr_base/transport/package/transport_data_receiver.h"
#include "pxr_base/transport/package/transport_data_sender.h"
#include "pxr_base/transport/transport_session.h"
#include "pxr_base/usb/usb_device.h"

#include <functional>

namespace pxr_base
{
    class TransportSessionUsb : public TransportSession,
        public TransportDataSender::SenderSink,
        public TransportDataReceiver::ReceiverSink,
        public std::enable_shared_from_this<TransportSessionUsb>
    {
    public:
        TransportSessionUsb(std::shared_ptr<UsbDevice>& device);
        ~TransportSessionUsb();

        virtual bool Start(const Config& config) override;
        virtual void Stop() override;
        virtual bool SendData(const TransportData& data) override;

        virtual TransportType type() override;
        virtual Config config() override;
        virtual bool running() override;
    public:
        void InvokeReadFromDevice();
        void InvokeCloseDevice();
        void InvokeSendData(const DataBuffer& data);
    private:
        static const size_t kSendBufferPacketNum = 128;
        static const size_t kRecvBufferSize = 2 * 1024 * 1024;//5M

        virtual void NotifyDataReadyToSend(DataBuffer::Ptr_t& data) override;
        virtual void NotifyReceivedTransportData(TransportChannelId channel_id,
            const DataBuffer&data) override;
        void StartAyncRecv();
        bool AsyncRecv();
        bool ReceiveFromDevice();
        bool SendByDevice(const DataBuffer& data);
    private:
        typedef TransportSessionCallback* CallbackPtr;
        void SafeCallback(std::function<void(CallbackPtr)> f);
        void HandleSessionError();
        inline bool SetConfig(const Config& config);
    private:
        std::atomic_bool running_;
        std::recursive_mutex mutex_;
        Config config_;

        std::shared_mutex dev_mutex_;
        std::shared_ptr<UsbDevice> device_;

        std::shared_ptr<TaskQueue> send_queue_;
        TransportDataSender sender_;

        TransportDataReceiver receiver_;
        std::shared_ptr<TaskQueue> recv_queue_;
        std::shared_ptr<DataBuffer> recv_buffer_;
    };
}