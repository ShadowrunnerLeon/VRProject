#pragma once
#include "pxr_base/common/vector.h"

namespace pxr_base
{
    class PXR_API  MotionParam
    {
    public:
        MotionParam() = default;
        ~MotionParam() = default;

        MotionParam(const MotionParam& other);

        Vector GetVelocity_() const;
        void SetVelocity_(const Vector& val);
        void SetVelocity_(float x, float y, float z);

        Vector GetAcceleration_() const;
        void SetAcceleration_(const Vector& val); 
        void SetAcceleration_(float x, float y, float z);

        void Reset();


        const Vector velocity() const;
        Vector& velocity();

        const Vector acceleration() const;
        Vector& acceleration();

        MotionParam& operator=(const MotionParam& other);

    private:
        Vector velocity_;
        Vector acceleration_;
    };
}