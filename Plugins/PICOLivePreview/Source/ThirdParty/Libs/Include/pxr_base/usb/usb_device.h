#pragma once

#include "pxr_base/transport/data_buffer.h"
#include "pxr_base/usb/libusb_header.h"
#include "pxr_base/usb/usb_controll_request.h"

namespace pxr_base
{
    class UsbContext;
    class UsbDevice
    {
    public:
        UsbDevice(std::shared_ptr<UsbContext> context);
        ~UsbDevice();

        bool Open(libusb_device* device, bool bulk_flag);
        bool Open(uint16_t vendor_id, uint16_t product_id, bool bulk_flag);
        void Close();
        bool ControlTransfer(UsbControllRequest& request, uint32_t timeout);
        bool SendBulk(const DataBuffer& data, uint32_t timeout);
        bool RecvBulk(DataBuffer& data, uint32_t timeout);
    private:
        bool ClaimDeviceInterface(uint8_t interface_num);
        void ReleaseDeviceInterface();
        //Note(by Alan Duan): Detach not used, call SetAutoDetachKernelDriver instead.
        bool DetachKernelDriverActive();
        void SetAutoDetachKernelDriver();
        bool InitBulkTransfer(libusb_device* device);
    private:
        std::shared_ptr<UsbContext> context_;
        libusb_device_handle* device_handle_ = nullptr;

        bool claim_flag_ = false;
        int interface_number_ = 0;
        uint8_t bulk_out_endpoint_ = 0;
        uint8_t bulk_input_endpoint_ = 0;
    };
}