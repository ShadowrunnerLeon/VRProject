#pragma once
#include "pxr_base/transport/transport_session.h"
#include "asio2/asio/asio.hpp"
#include "pxr_base/task_queue/task_queue_boost.h"
#include <string>

namespace pxr_base
{
    //Note(by Alan Duan): We should support option setting for serial port in future. 
    //                    Options:baud_rate, flow_control, parity and stop_bits
    class TransportSessionSerialPort:public TransportSession
    {
    public:
        TransportSessionSerialPort();
        ~TransportSessionSerialPort();
        
        virtual bool Start(TransportSessionCallback* callback) override;
        virtual void Stop() override;
        virtual bool IsStarted() override;
        virtual bool SendData(const DataBuffer& data, TransportChannelId channel_id) override;
        std::string ToString();
    private:
        bool OpenSerialPort(const Config& config);
        bool SetSerialOption();
        void StartAyncRecv();
        void HandleReceiveData(asio::error_code ec, std::size_t bytes_recvd);
        void HandleException(const std::exception& ex);
    private:
        TaskQueueBoost task_queue_;
        asio::serial_port serial_port_;
        Config config_;
        std::recursive_mutex mutex_;
        TransportSessionCallback* callback_ = nullptr;
        //Note(by Alan Duan): 2M for receive buffer;
        static const size_t kReceiveBufferSize = 1024 * 1024 * 2;
        DataBuffer recv_buffer_;
    };
}