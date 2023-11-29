#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API ControllerButton
    {
    public:
        ControllerButton() = default;
        ~ControllerButton() = default;

        ControllerButton(const ControllerButton& other);

        int64_t GetIndex_() const;
        void SetIndex_(int64_t val);

        int32_t GetState_() const;
        void SetState_(int32_t val);

        float GetJoystickX_() const;
        void SetJoystickX_(float val);

        float GetJoystickY_() const;
        void SetJoystickY_(float val);

        float GetTriggerValue_() const;
        void SetTriggerValue_(float val);

        float GetGripValue_() const;
        void SetGripValue_(float val);

        void Reset();


        int64_t index() const;
        int64_t& index();

        int32_t state() const;
        int32_t& state();

        float joystick_x() const;
        float& joystick_x();

        float joystick_y() const;
        float& joystick_y();

        float trigger_value() const;
        float& trigger_value();

        float grip_value() const;
        float& grip_value();

        ControllerButton& operator=(const ControllerButton& other);

    private:
        int64_t index_ = 0;
        int32_t state_ = 0;
        float joystick_x_ = 128;
        float joystick_y_ = 128;
        float trigger_value_ = 128;
        float grip_value_ = 128;
    };
}