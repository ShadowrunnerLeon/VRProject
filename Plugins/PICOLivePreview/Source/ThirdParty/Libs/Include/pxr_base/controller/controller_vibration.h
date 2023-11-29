#pragma once
#include <stdint.h>
#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API ControllerVibration
    {
    public:
        ControllerVibration() = default;
        ~ControllerVibration() = default;
        ControllerVibration(const ControllerVibration& other);
        ControllerVibration(int64_t index, float ampl, float dura, float freq);

        int64_t GetIndex_() const;
        void SetIndex_(int64_t index);

        float GetAmplitude_() const;
        void SetAmplitude_(float index);

        float GetDuration_() const;
        void SetDuration_(float index);

        float GetFrequency_() const;
        void SetFrequency_(float index);

        void Set_(int64_t index, float ampl, float dura, float freq);
        void Reset();

        int64_t index() const;
        int64_t& index();

        float amplitude() const;
        float& amplitude();

        float duration() const;
        float& duration();

        float frequency() const;
        float& frequency();

        ControllerVibration& operator=(const ControllerVibration& other);

    private:
        int64_t index_ = 0;
        float amplitude_ = 0.;
        float duration_ = 0.;
        float frequency_ = 0.;
    };
}