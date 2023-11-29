#pragma once
#include <stdint.h>

namespace pxr_base
{
    static const uint16_t kAoaVendorID = 0x18D1;
    struct AoaProductID
    {
        static const uint16_t kAccessory = 0x2D00;
        static const uint16_t kAccessoryAdb = 0x2D01;
        //Note(by Alan Duan): For AOA version 2,but audio support has been deprecated
        //                   in Android 8.0. https://source.android.com/devices/accessories/aoa2
        static const uint16_t kAudio = 0x2D02;
        static const uint16_t kAudioAdb = 0x2D03;
        static const uint16_t kAccessoryAudio = 0x2D04;
        static const uint16_t kAccessoryAudioAdb = 0x2D05;
    };
}