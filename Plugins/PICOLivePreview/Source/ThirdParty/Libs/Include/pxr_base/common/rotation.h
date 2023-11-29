#pragma once
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API Rotation
    {
    public:
        Rotation() = default;
        ~Rotation() = default;
        Rotation(const Rotation& other);
        Rotation(float w, float x, float y, float z);

        Rotation& operator=(const Rotation& other);
        bool operator==(const Rotation& other);
        bool operator!=(const Rotation& other);

        float GetX_() const;
        void SetX_(float val);

        float GetY_() const;
        void SetY_(float val);

        float GetZ_() const;
        void SetZ_(float val);

        float GetW_() const;
        void SetW_(float val);

        void Set_(float x, float y, float z, float w);
        void Reset();

        float x() const;
        float& x();

        float y() const;
        float& y();

        float z() const;
        float& z();

        float w() const;
        float& w();

    private:
        float w_ = 0.;
        float x_ = 0.;
        float y_ = 0.;
        float z_ = 0.;
    };
}