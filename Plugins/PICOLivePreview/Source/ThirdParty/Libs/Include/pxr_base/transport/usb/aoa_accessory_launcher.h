#pragma once
#include <string>

#include "pxr_base/transport/usb/aoa_identify_info.h"
#include "pxr_base/usb/usb_controll_request.h"
#include "pxr_base/usb/usb_device.h"

namespace pxr_base
{
    class AoaAccessoryLauncher
    {
    public:
        AoaAccessoryLauncher(const AoaIdentifyInfo& info,
            std::shared_ptr<UsbDevice> device);
        ~AoaAccessoryLauncher();

        bool Run();
        bool IsContinued() const;
        void Stop();

        enum class Request
        {
            kNone = -1,
            kGetProtol = 51,
            kIdentifyInfo = 52,
            kStartAccessory = 53
        };
        typedef AoaIdentifyType IdentifyType;
    private:
        bool CheckProtocol();
        bool SendIdentify();
        bool SendIdentifyInfo(IdentifyType type);
        bool StartAccesory();
        inline IdentifyType GetNextType(IdentifyType type);
    private:
        AoaIdentifyInfo infos_;
        bool launch_success_ = false;
        std::shared_ptr<UsbDevice> device_;
        Request next_request_ = Request::kGetProtol;
        IdentifyType next_identify_index_ = IdentifyType::kNone;
    };
}
