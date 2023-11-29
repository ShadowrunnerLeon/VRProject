#pragma once
#include "pxr_base/logger.h"
#define PXR_USB_ERR(M, E) PXR_ERROR_THIS(#M << " error= " << libusb_error_name(E));