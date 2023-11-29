#pragma once
#include <stdint.h>

#include "pxr_base/pxr_exports.h"
#include "pxr_base/common/rotation.h"
#include "pxr_base/common/vector.h"
#include "pxr_base/common/motion_param.h"

namespace pxr_base
{
    class PXR_API ControllerPose
    {
    public:
        ControllerPose() = default;
        ControllerPose(const ControllerPose& other);

        bool GetActive_() const;
        void SetActive_(bool val);

        int64_t GetIndex_() const;
        void SetIndex_(int64_t index);

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

        int64_t index() const;
        int64_t& index();

        const Vector& position() const;
        Vector& position();

        const Rotation& rotation() const;
        Rotation& rotation();

        const MotionParam& linear() const;
        MotionParam& linear();

        const MotionParam& angular() const;
        MotionParam& angular();

        ControllerPose& operator=(const ControllerPose& other);

    private:
        int64_t index_ = 0;
        bool active_ = false;
        Vector position_;
        Rotation rotation_;
        MotionParam linear_;
        MotionParam angular_;
    };
}