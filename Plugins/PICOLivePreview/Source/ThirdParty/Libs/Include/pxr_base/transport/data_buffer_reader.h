#pragma once
#include <memory>
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/transport/data_buffer.h"

namespace pxr_base
{
    class PXR_API DataBufferReader
    {
    public:
        DataBufferReader(uint8_t* buffer, size_t length, size_t beg = 0);

        size_t Read(uint8_t* buffer, size_t buffer_size, bool change_offset = true);
        size_t Read(DataBuffer& buffer, bool change_offset = true);
        size_t Read(DataBuffer& buffer, size_t size, bool change_offset = true);
        size_t Offset_() const;
        size_t Length_() const;
        const uint8_t* ReadPtr_() const;
        size_t RemainingLength_() const;
        bool Valid_() const;
        void Reset();
    private:
        bool IsConsistent() const;
        void Clear();
    private:
        uint8_t* buffer_ = nullptr;
        size_t beg_ = 0;
        size_t length_ = 0;
        size_t offset_ = 0;
    };
}