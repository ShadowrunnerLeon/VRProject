#pragma once
#include "pxr_base/transport/data_buffer.h"
#include "pxr_base/transport/transport_common.h"
#include "pxr_base/transport/network_address.h"
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API TransportData
    {
    public:
        typedef std::shared_ptr<TransportData> Ptr_t;
        TransportData();
        TransportData(const TransportData& other);
        TransportData(TransportData&& packet);

        DataBuffer& data();
        const DataBuffer& data() const;

        //Note(by Alan Duan): Maybe return InvalidChannelId.
        TransportChannelId channel() const;
        TransportChannelId& channel();

        //Note(by Alan Duan): Maybe return invalid network address.
        NetworkAddress& remote_addr();
        const NetworkAddress& remote_addr() const;
    private:
        DataBuffer data_;
        TransportChannelId channed_id_ = kInvalidChannelId;
        NetworkAddress remote_addr_;
    };
}