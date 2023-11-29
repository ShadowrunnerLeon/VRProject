#pragma once
#include "pxr_base/common/motion_param.h"
#include "pxr_base/common/rotation.h"

namespace pxr_base
{
    class PXR_API HmdPose
    {
    public:
        HmdPose() = default;
        ~HmdPose() = default;
        HmdPose(const HmdPose& other);

        bool GetActive_() const;
        void SetActive_(bool val);

        Vector GetPostion_() const;
        void SetPostion_(const Vector& val);
        void SetPostion_(float x, float y, float z);

        Rotation GetRotation_() const;
        void SetRotation_(const Rotation& val);
        void SetRotation_(float x, float y, float z, float w);

        MotionParam GetLinear_() const;
        void SetLinear_(const MotionParam& val);
        void SetLinear_(const Vector& vel, const Vector& acce);

        MotionParam GetAngular_() const;
        void SetAngular_(const MotionParam& val);
        void SetAngular_(const Vector& vel, const Vector& acce);

        void Reset();


        bool active() const;
        bool& active();

        const Vector& position() const;
        Vector& position();

        const Rotation& rotation() const;
        Rotation& rotation();

        const MotionParam& linear() const;
        MotionParam& linear();

        const MotionParam& angular() const;
        MotionParam& angular();

        HmdPose& operator=(const HmdPose& other);

    private:
        bool active_ = false;
        Vector position_;
        Rotation rotation_;
        MotionParam linear_;
        MotionParam angular_;
    };
}