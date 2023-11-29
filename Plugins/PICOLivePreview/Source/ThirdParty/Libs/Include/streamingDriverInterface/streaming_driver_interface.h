#pragma once

#include "pxr_base/pxr_exports.h"
#include "pxr_base/common/driver_media_config.h"
#include "pxr_base/controller/controller_button.h"
#include "pxr_base/controller/controller_state.h"
#include "pxr_base/controller/controller_vibration.h"
#include "pxr_base/common/device_fov_info.h"
#include "pxr_base/common/device_pose.h"
#include "pxr_base/common/device_tracking_origin.h"
#include "pxr_base/video/driver_shared_images.h"

#include <memory>

namespace pxr
{
    enum class DriverResultCode
    {
        kUndefined = -1,
        kOk = 0,
        kInternalError = 10001,
        kInvalidParam = 10002,
		kCallError = 10003,
        kServiceError = 20001,
    };

    enum class StreamingState
    {
        kUndefinedStreamingState = 0,
        kStreamingLaunching,
        kStreamingReady,
        kStreamingFinished,
    };

    struct PXR_API DeviceInfo
    {
        char device_id[256] = {0};
        char device_name[256] = { 0 };
    };

    class PXR_API StreamingDriverInterfaceCallback
    {
    public:
        virtual void NotifyConnectToService(DriverResultCode result) = 0;
        virtual void NotifyDisconnectFromService(DriverResultCode reason) = 0;

        virtual void NotifyDeviceConnected(const DeviceInfo& device_info) = 0;
        virtual void NotifyDeviceDisconnected(const DeviceInfo& device_info) = 0;

        virtual void NotifyStreamingStateUpdated(StreamingState state, 
            const pxr_base::DriverMediaConfig& media_config) = 0;

        virtual void NotifyIPDUpdated(float ipd) = 0;
        virtual void NotifyFovUpdated(const pxr_base::DeviceFovInfo& fov_info) = 0;
        virtual void NotifyDevicePoseUpdated(const pxr_base::DevicePose pose_arr[],
            size_t arr_size) = 0;
        virtual void NotifyControllerButtonUpdated(const pxr_base::ControllerButton& button) = 0;
        virtual void NotifyControllerStateUpdated(
            const pxr_base::ControllerState controller_states[], size_t size) = 0;

        virtual ~StreamingDriverInterfaceCallback() = default;
    };

    class PXR_API StreamingDriverInterface
    {
    public:
        typedef std::shared_ptr<StreamingDriverInterface> Ptr_t;
        static Ptr_t Create(StreamingDriverInterfaceCallback* callback);

        virtual DriverResultCode ConnectToService() = 0;
        virtual DriverResultCode DisconnectFromService() = 0;

        virtual DriverResultCode StartStreaming(
            const pxr_base::DriverMediaConfig& media_config) = 0;
        virtual DriverResultCode StopStreaming() = 0;

        virtual DriverResultCode SubmitLayerImage(const pxr_base::DriverSharedImages& layer) = 0;
        virtual DriverResultCode VibrateController(
            const pxr_base::ControllerVibration& vibration) = 0;
        virtual DriverResultCode SetTrackingOrigin(pxr_base::DeviceTrackingOrigin origin) = 0;

        virtual bool IsConnected() = 0;
        virtual StreamingState GetStreamingState() = 0;
        virtual ~StreamingDriverInterface() = default;
    };
}