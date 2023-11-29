//Unreal® Engine, Copyright 1998 – 2023, Epic Games, Inc. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#if PLATFORM_WINDOWS
#include "D3D11RHIPrivate.h"
#include "streamingDriverInterface/streaming_driver_interface.h"
#endif

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include "Microsoft/COMPointer.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

using namespace pxr;
using namespace pxr_base;

DECLARE_DELEGATE_TwoParams(FOnDeviceConnectedResultsEvent, const DeviceInfo&,int32);
DECLARE_DELEGATE_TwoParams(FOnConnectFromServiceEvent, const DriverResultCode&,int32);
DECLARE_DELEGATE_OneParam(FOnFovUpdatedFromServiceEvent,const DeviceFovInfo&);

class StreamingDataReceiveWrapper : public StreamingDriverInterfaceCallback
{
public:
	
	StreamingDataReceiveWrapper() = default;
	virtual void NotifyFovUpdated(const pxr_base::DeviceFovInfo& fov_info) override;
	virtual void NotifyIPDUpdated(float ipd) override;
	virtual void NotifyDevicePoseUpdated(const DevicePose pose_arr[], size_t arr_size) override;
	virtual void NotifyControllerButtonUpdated(const ControllerButton& button) override;
	
	virtual void NotifyConnectToService(DriverResultCode result) override;
	virtual void NotifyDisconnectFromService(DriverResultCode reason) override;

	virtual void NotifyDeviceConnected(const DeviceInfo& device_info) override;
	virtual void NotifyDeviceDisconnected(const DeviceInfo& device_info) override;

	virtual void NotifyStreamingStateUpdated(StreamingState state, const DriverMediaConfig& media_config) override;
	virtual void NotifyControllerStateUpdated(const ControllerState controller_states[], size_t size) override;
	
	Vector GetControllerPosition(EControllerHand DeviceHand) const;
	Rotation GetControllerRotation(EControllerHand DeviceHand) const;
	Vector GetHMDPosition() const;
	Rotation GetHMDRotation() const;

	bool GetDeviceFovInfo(DeviceFovInfo& FovInfo) const;
	float GetJoystickX(EControllerHand DeviceHand) const;
	float GetJoystickY(EControllerHand DeviceHand) const;
	float GetTriggerValue(EControllerHand DeviceHand) const;
	float GetGripValue(EControllerHand DeviceHand) const;
	float GetStateValue(EControllerHand DeviceHand) const;
	bool GetConnectionStatus(EControllerHand DeviceHand) const;
	bool IsStreaming() const;
	bool GetDriverMediaConfig(DriverMediaConfig& MediaConfig) const;
	
	FOnDeviceConnectedResultsEvent OnDeviceConnectedResultsEvent;
	FOnConnectFromServiceEvent OnConnectFromServiceEvent;
	FOnFovUpdatedFromServiceEvent OnFovUpdatedFromServiceEvent;
	FCriticalSection StateLock;
	
private:

	//DeviceInfo Device_Info;
	bool bLeftActive;
	bool bRightActive;
	bool bStreaming;
	DeviceFovInfo PDCDeviceFovInfo;
	DriverMediaConfig PDCMediaConfig;
	
	Vector LeftPosition;
	Rotation LeftRotation;
	Vector RightPosition;
	Rotation RightRotation;
	Vector HMDPosition;
	Rotation HMDRotation;

	float LeftJoystickX;
	float LeftJoystickY;
	float LeftTriggerValue;
	float LeftGripValue;
	int32 LeftState;

	float RightJoystickX;
	float RightJoystickY;
	float RightTriggerValue;
	float RightGripValue;
	int32 RightState;

};

struct FPICOXRFrustumDP;

class PICOXRDPHMD_API FPICOXRDPManager
{
public:
	FPICOXRDPManager()=default;
	~FPICOXRDPManager()=default;
	static bool IsStreaming();
	
	static bool OnBeginPlayStartStreaming();
	static bool OnEndPlayStopStreaming();
	static bool InitializeLivePreview();
	static void ShutDownLivePreview();
	static bool SendMessage(int SwapChainIndex);
	static void GetHMDPositionAndRotation(FVector &OutPostion,FQuat &OutQuat);
	static void GetControllerPositionAndRotation(EControllerHand DeviceHand,float WorldScale, FVector& OutPostion, FRotator& OutQuat);
	static bool GetControllerConnectionStatus(EControllerHand DeviceHand);
	static int32 GetControllerButtonStatus(EControllerHand DeviceHand);
	static void GetControllerAxisValue(EControllerHand DeviceHand,double &JoyStickX,double& JoyStickY,float &TriggerValue,float &GripValue);
	static bool IsConnectToServiceSucceed();
	static bool GetPICOXRFrustumDP(FPICOXRFrustumDP& PICOXRFrustumDP);

	static bool ConnectStreamingServer();
	static bool DisConnectStreamingServer();
	static bool SetDisconnectFromServiceEvent(const FOnConnectFromServiceEvent &OnConnectFromServiceEvent);
	static bool SetFovUpdatedFromServiceEvent(const FOnFovUpdatedFromServiceEvent &OnFovUpdatedFromServiceEvent);

	static FTextureRHIRef GetLeftBindingTexture();
	static FTextureRHIRef GetRightBindingTexture();
	
	static TArray<FTextureRHIRef> GetLeftRHITextureSwapChain();
	static TArray<FTextureRHIRef> GetRightRHITextureSwapChain();


	static TComPtr<IDXGIKeyedMutex> GetLeftKeyedMutex();
	static TComPtr<IDXGIKeyedMutex> GetRightKeyedMutex();

	static bool LockKeyedMutex(UINT64 Key =0,DWORD TimeOut_ms = 10);
	static void UnlockKeyedMutex(UINT64 Key =0);

	
private:
	static void OnDeviceConnectionChanged(const DeviceInfo& device_info,int32 State);

	static bool CreateStreamingInterface();
	static bool CreateSharedTexture2D();
	static bool CreateRHITexture(ID3D11Texture2D* OpenedSharedResource, EPixelFormat Format, FTexture2DRHIRef& OutTexture);


	static FTextureRHIRef LeftBindingTexture;
	static FTextureRHIRef RightBindingTexture;

	static TArray<FTextureRHIRef> LeftRHITextureSwapChain;
	static TArray<FTextureRHIRef> RightRHITextureSwapChain;
	
	static bool bConnectToServiceSucceed;
	static StreamingDriverInterface::Ptr_t StreamingDriverInterfacePtr;
	static TSharedPtr<StreamingDataReceiveWrapper,ESPMode::ThreadSafe> StreamingDataWrapperPtr;

	static uint32 SwapChainIndex_RHIThread;
	
	static TArray<TRefCountPtr<ID3D11Texture2D>> LeftDstSwapChainTextures;
	static TArray<TRefCountPtr<ID3D11Texture2D>> RightDstSwapChainTextures;

	static TArray<void*> LeftDstTextureHandles;
	static TArray<void*> RightDstTextureHandles;

	static FTextureRHIRef LeftRHITexture;
	static FTextureRHIRef RightRHITexture;

    static DriverMediaConfig MediaConfig;
	
	static TComPtr<IDXGIKeyedMutex> LeftKeyedMutex;
	static TComPtr<IDXGIKeyedMutex> RightKeyedMutex;
};
