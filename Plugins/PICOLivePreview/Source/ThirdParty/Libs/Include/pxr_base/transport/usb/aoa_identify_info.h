#pragma once
#include "pxr_base/transport/data_buffer.h"
#include "pxr_base/pxr_exports.h"
#include "pxr_base/usb/usb_driver_info.h"

namespace pxr_base
{
    enum class AoaIdentifyType
    {
        kNone = -1,
        kManufacturerName = 0,
        kModelName = 1,
        kDescription = 2,
        kVersion = 3,
        kURI = 4,
        kSerialNumber = 5
    };

    class PXR_API AoaIdentifyInfo
    {
    public:
        void Set_(AoaIdentifyType type, const char* val);
        const DataBuffer& Get_(AoaIdentifyType type) const;
        bool IsValid(AoaIdentifyType type) const;
    private:
        DataBuffer& GetMultable_(AoaIdentifyType type);
    private:
        DataBuffer none_;
        DataBuffer manufacturer_name_;
        DataBuffer model_name_;
        DataBuffer description_;
        DataBuffer version_;
        DataBuffer uri_;
        DataBuffer serial_number_;
    };

    struct PXR_API AoaConfig
    {
        static const size_t kMaxListeningDeviceNum = 16;
        size_t driver_info_num = 0;
        UsbDriverInfo driver_infos[kMaxListeningDeviceNum];
        AoaIdentifyInfo identify;
    };
}