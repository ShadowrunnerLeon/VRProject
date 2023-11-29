#pragma once
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/common/rotation.h"
#include "pxr_base/common/vector.h"

namespace pxr_base
{
    class PXR_API DriverSharedImages
    {
    public:
        DriverSharedImages();
        ~DriverSharedImages() = default;

        DriverSharedImages(const DriverSharedImages& other);
        DriverSharedImages& operator=(const DriverSharedImages& other);
        bool operator==(const DriverSharedImages& other);
        bool operator!=(const DriverSharedImages& other);

        void* GetHandle_(int index) const;
        void SetHandle_(int index, void* val);

        Vector GetPostion_() const;
        void SetPostion_(const Vector& postion);
        void SetPostion_(float x, float y, float z);

        Rotation GetRotation_() const;
        void SetRotation_(const Rotation& rotation);
        void SetRotation_(float x, float y, float z, float w);

        void Reset();

        void* shared_handle(int index) const;
        void*& shared_handle(int index);

        const Vector position() const;
        Vector& position();

        const Rotation rotation() const;
        Rotation& rotation();

        uint64_t timestamp() const;
        uint64_t& timestamp();

    private:
        void* shared_handle_[2] = { nullptr };
        Vector postion_;
        Rotation rotation_;
        uint64_t timestamp_ = 0;
    };
}