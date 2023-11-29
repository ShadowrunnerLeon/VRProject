#pragma once

#include <string>

#include "pxr_base/pxr_exports.h"

namespace pxr_base
{
    class PXR_API Random
    {
    public:
        //
        //brief: Generate random string.
        //param: [in] length     - Random string length, less than 256.
        //       [in] containNum - If true, random string will contain number, 
        //                         Otherwise only contain letters.
        static std::string RandomString(size_t length, bool hasNum = true);
        static int64_t RandomNumber(int64_t min, int64_t max);
    };
}