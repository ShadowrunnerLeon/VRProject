#pragma once
#include <memory>
#include "pxr_base/usb/libusb_header.h"

namespace pxr_base
{
    class UsbDeviceList;
    class UsbContext:public std::enable_shared_from_this<UsbContext>
    {
    public:
        UsbContext();
        ~UsbContext();

        static std::shared_ptr<UsbContext> Create();
        std::shared_ptr<UsbDeviceList> GetDeviceList();
        libusb_context* NativeContext();
    private:
        void InitContext();
        void ResetContext();
        void InitLibusbLog();
        static void LIBUSB_CALL LibusbLogCallback(libusb_context* ctx, enum libusb_log_level level,
            const char* str);
    private:
        libusb_context* context_ = nullptr;
    };
}