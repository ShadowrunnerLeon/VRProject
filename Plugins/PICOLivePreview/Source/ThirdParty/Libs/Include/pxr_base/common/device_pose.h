#pragma once

#include <stdint.h>

#include "pxr_base/common/motion_param.h"
#include "pxr_base/common/rotation.h"

namespace pxr_base
{
    enum class PoseType
    {
        kUndefinedPoseType = 0,
        kHmd,
        kController
    };

    class PXR_API DevicePose
    {
    public:
        DevicePose() = default;
        DevicePose(const DevicePose& other);
        ~DevicePose() = default;

        int64_t GetIndex_() const;
        void SetIndex_(int64_t);

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

        PoseType GetPoseType_() const;
        void SetPoseType_(PoseType type);

        void Reset();


        int64_t index() const;
        int64_t& index();

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

        PoseType pose_type() const;
        PoseType& pose_type();

        DevicePose& operator=(const DevicePose& other);

    private:
        int64_t index_;
        bool active_ = false;
        Vector position_;
        Rotation rotation_;
        MotionParam linear_;
        MotionParam angular_;
        PoseType pose_type_;
    };
}



