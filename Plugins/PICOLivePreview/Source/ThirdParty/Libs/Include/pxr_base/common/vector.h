#pragma once
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API Vector
    {
    public:
        Vector() = default;
        ~Vector() = default;
        Vector(float x, float y, float z);
        Vector(const Vector& other);

        Vector& operator=(const Vector& other);
        bool operator==(const Vector& other);
        bool operator!=(const Vector& other);

        float GetX_() const;
        void SetX_(float v);

        float GetY_() const;
        void SetY_(float v);

        float GetZ_() const;
        void SetZ_(float v);

        void Set_(float x, float y, float z);
        void Reset();


        float x() const;
        float& x();

        float y() const;
        float& y();

        float z() const;
        float& z();

    private:
        float x_ = 0.;
        float y_ = 0.;
        float z_ = 0.;
    };
}