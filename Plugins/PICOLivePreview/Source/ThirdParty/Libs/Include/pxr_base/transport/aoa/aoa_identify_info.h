#pragma once
#include "pxr_base/transport/data_buffer.h"
#include "pxr_base/pxr_exports.h"

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
}