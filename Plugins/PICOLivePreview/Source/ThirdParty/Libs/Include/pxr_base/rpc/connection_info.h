#pragma once

#include <memory>
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/transport/data_buffer.h"

namespace pxr_base
{
    class PXR_API ConnectionInfo
    {
    public:
        ConnectionInfo();
        ConnectionInfo(const ConnectionInfo& info);

        const char* GetId_() const;
        size_t GetIdLen_() const;
        void SetId_(const char* id);

        const char* GetName_() const;
        size_t GetNameLen_() const;
        void SetName_(const char* name);

        const char* GetModuleVersion_() const;
        size_t GetModuleVersionLen_() const;
        void SetModuleVersion_(const char* module_version);

        const char* GetProtocolVersion_() const;
        size_t GetProtocoleVersionLen_() const;
        void SetProtocolVersion_(const char* protocol_version);

        //const char* id() const;

        ConnectionInfo& operator=(const ConnectionInfo& other);

    private:
        pxr_base::DataBuffer id_;
        pxr_base::DataBuffer name_;
        pxr_base::DataBuffer module_version_;
        pxr_base::DataBuffer protocol_version_;
    };
};

