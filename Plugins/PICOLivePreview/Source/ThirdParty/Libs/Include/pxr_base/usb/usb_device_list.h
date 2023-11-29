#pragma once
#include "pxr_base/usb/libusb_header.h"
#include <memory>

namespace pxr_base
{
    class UsbContext;
    class UsbDeviceList
    {
    public:
        UsbDeviceList(std::shared_ptr<UsbContext> context);
        ~UsbDeviceList();

        size_t DeviceNum();
        size_t Update();
        libusb_device* GetDevice(uint16_t vendor_id, uint16_t product_id);
    private:    
        void InitDeviceList();
        void ResetDeviceList();
    private:
        std::shared_ptr<UsbContext> context_;
        libusb_device** device_list_ = nullptr;
        size_t device_number_ = 0;
    };
}