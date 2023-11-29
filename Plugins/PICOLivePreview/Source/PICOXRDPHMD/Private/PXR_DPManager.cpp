//Unreal® Engine, Copyright 1998 – 2023, Epic Games, Inc. All rights reserved.


#include "PXR_DPManager.h"
#include <vector>
#include "ClearQuad.h"
#include "CommonRenderResources.h"
#include "D3D11RHIPrivate.h"
#include "GlobalShader.h"
#include "PXR_Log.h"
#include "ScreenRendering.h"
#include "PXR_DPHMD.h"
#include "PXR_DPSettings.h"
#include "Microsoft/COMPointer.h"

/** Utility struct, similar to FScopeLock but allows the lock to be enabled/disabled more easily */
struct FConditionalScopeLock
{
	UE_NONCOPYABLE(FConditionalScopeLock);

	FConditionalScopeLock(FCriticalSection* InSyncObject, bool bShouldLock)
	{
		checkf(InSyncObject != nullptr, TEXT("InSyncObject must point to a valid FCriticalSection"));

		if (bShouldLock)
		{		
			SyncObject = InSyncObject;
			SyncObject->Lock();
		}
		else
		{
			SyncObject = nullptr;
		}
	}

	/** Destructor that performs a release on the synchronization object. */
	~FConditionalScopeLock()
	{
		if (SyncObject != nullptr)
		{
			SyncObject->Unlock();
		}
	}

private:
	FCriticalSection* SyncObject;
};


void StreamingDataReceiveWrapper::NotifyFovUpdated(const pxr_base::DeviceFovInfo& fov_info)
{
	PDCDeviceFovInfo = fov_info;
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyFovUpdated");
	OnFovUpdatedFromServiceEvent.ExecuteIfBound(fov_info);
}

void StreamingDataReceiveWrapper::NotifyIPDUpdated(float ipd)
{
	PXR_LOGV(PxrUnreal, "PXR_LivePreview StreamingDataReceiver NotifyIPDUpdated:%f", ipd);
}

Rotation StreamingDataReceiveWrapper::GetControllerRotation(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftRotation : RightRotation;
}

Vector StreamingDataReceiveWrapper::GetHMDPosition() const
{
	return HMDPosition;
}

Rotation StreamingDataReceiveWrapper::GetHMDRotation() const
{
	return HMDRotation;
}

bool StreamingDataReceiveWrapper::GetDeviceFovInfo(DeviceFovInfo& FovInfo) const
{
	if (PDCDeviceFovInfo.down
		&& PDCDeviceFovInfo.up
		&& PDCDeviceFovInfo.left
		&& PDCDeviceFovInfo.right)
	{
		FovInfo = PDCDeviceFovInfo;
		return true;
	}

	return false;
}

Vector StreamingDataReceiveWrapper::GetControllerPosition(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftPosition : RightPosition;
}

void StreamingDataReceiveWrapper::NotifyDevicePoseUpdated(const pxr_base::DevicePose pose_arr[], size_t arr_size)
{
	for (int index = 0; index < arr_size; index++)
	{
		switch (pose_arr[index].GetPoseType_())
		{
		case pxr_base::PoseType::kUndefinedPoseType: break;
		case pxr_base::PoseType::kHmd:
			{
				FConditionalScopeLock ScopeLock(&StateLock, true);
				PXR_LOGV(PxrUnreal, "PXR_LivePreview NotifyDevicePoseUpdated HMD Index %lld", pose_arr[index].GetIndex_());
				HMDPosition = pose_arr[index].position();
				HMDRotation = pose_arr[index].rotation();

			}
			break;
		case pxr_base::PoseType::kController:
			{
				PXR_LOGV(PxrUnreal, "PXR_LivePreview NotifyDevicePoseUpdated Controller Index %lld", pose_arr[index].GetIndex_());
				if (pose_arr[index].GetIndex_() == 1)
				{
					LeftPosition = pose_arr[index].position();
					LeftRotation = pose_arr[index].rotation();
					bLeftActive = pose_arr[index].active();
				}
				else
				{
					RightPosition = pose_arr[index].position();
					RightRotation = pose_arr[index].rotation();
					bRightActive = pose_arr[index].active();
				}
			}
			break;
		default: ;
		}
	}
}

void StreamingDataReceiveWrapper::NotifyControllerButtonUpdated(const pxr_base::ControllerButton& button)
{
	if (!button.index())
	{
		LeftJoystickX = button.joystick_x();
		LeftJoystickY = button.joystick_y();
		LeftTriggerValue = button.trigger_value();
		LeftGripValue = button.grip_value();
		LeftState = button.state();
	}
	else
	{
		RightJoystickX = button.joystick_x();
		RightJoystickY = button.joystick_y();
		RightTriggerValue = button.trigger_value();
		RightGripValue = button.grip_value();
		RightState = button.state();
	}
}

void StreamingDataReceiveWrapper::NotifyConnectToService(DriverResultCode result)
{
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyConnectToService reason:%d", result);
	OnConnectFromServiceEvent.ExecuteIfBound(result, 1);
}

void StreamingDataReceiveWrapper::NotifyDisconnectFromService(DriverResultCode reason)
{
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyDisconnectFromService reason:%d", reason);
	OnConnectFromServiceEvent.ExecuteIfBound(reason, 0);
}

void StreamingDataReceiveWrapper::NotifyDeviceConnected(const DeviceInfo& device_info)
{
	PXR_LOGD(PxrUnreal,"PXR_LivePreview DeviceName:%s",UTF8_TO_TCHAR(device_info.device_name));
	OnDeviceConnectedResultsEvent.ExecuteIfBound(device_info, 1);
}

void StreamingDataReceiveWrapper::NotifyDeviceDisconnected(const DeviceInfo& device_info)
{
	OnDeviceConnectedResultsEvent.ExecuteIfBound(device_info, 0);
}

void StreamingDataReceiveWrapper::NotifyStreamingStateUpdated(StreamingState state, const DriverMediaConfig& media_config)
{
	bStreaming = (state == StreamingState::kStreamingReady) ? true : false;
	PDCMediaConfig = media_config;
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyStreamimgStateUpdated state：%d", bStreaming);
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyStreamimgStateUpdated frame_rate：%d", PDCMediaConfig.frame_rate());
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyStreamimgStateUpdated render_height：%d", PDCMediaConfig.render_height());
	PXR_LOGD(PxrUnreal, "PXR_LivePreview NotifyStreamimgStateUpdated render_width：%d", PDCMediaConfig.render_width());
}

void StreamingDataReceiveWrapper::NotifyControllerStateUpdated(const ControllerState controller_states[], size_t size)
{
}

float StreamingDataReceiveWrapper::GetJoystickX(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftJoystickX : RightJoystickX;
}

float StreamingDataReceiveWrapper::GetJoystickY(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftJoystickY : RightJoystickY;
}

float StreamingDataReceiveWrapper::GetTriggerValue(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftTriggerValue : RightTriggerValue;
}

float StreamingDataReceiveWrapper::GetGripValue(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftGripValue : RightGripValue;
}

float StreamingDataReceiveWrapper::GetStateValue(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? LeftState : RightState;
}

bool StreamingDataReceiveWrapper::GetConnectionStatus(EControllerHand DeviceHand) const
{
	return (DeviceHand == EControllerHand::Left) ? bLeftActive : bRightActive;
}

bool StreamingDataReceiveWrapper::IsStreaming() const
{
	return bStreaming;
}

bool StreamingDataReceiveWrapper::GetDriverMediaConfig(DriverMediaConfig& MediaConfig) const
{
	if (PDCMediaConfig.frame_rate() >= 72
		&& PDCMediaConfig.render_height() > 0
		&& PDCMediaConfig.render_width() > 0)
	{
		MediaConfig = PDCMediaConfig;
		return true;
	}
	return false;
}

bool FPICOXRDPManager::bConnectToServiceSucceed = false;
StreamingDriverInterface::Ptr_t FPICOXRDPManager::StreamingDriverInterfacePtr = nullptr;
TSharedPtr<StreamingDataReceiveWrapper, ESPMode::ThreadSafe> FPICOXRDPManager::StreamingDataWrapperPtr = nullptr;
TArray<TRefCountPtr<ID3D11Texture2D>> FPICOXRDPManager::LeftDstSwapChainTextures;
TArray<TRefCountPtr<ID3D11Texture2D>> FPICOXRDPManager::RightDstSwapChainTextures;
TArray<void*> FPICOXRDPManager::LeftDstTextureHandles;
TArray<void*> FPICOXRDPManager::RightDstTextureHandles;
TArray<FTextureRHIRef> FPICOXRDPManager::LeftRHITextureSwapChain;
TArray<FTextureRHIRef> FPICOXRDPManager::RightRHITextureSwapChain;
FTextureRHIRef FPICOXRDPManager::LeftBindingTexture;
FTextureRHIRef FPICOXRDPManager::RightBindingTexture;
DriverMediaConfig FPICOXRDPManager::MediaConfig;
TComPtr<IDXGIKeyedMutex> FPICOXRDPManager::LeftKeyedMutex;
TComPtr<IDXGIKeyedMutex> FPICOXRDPManager::RightKeyedMutex;

bool FPICOXRDPManager::OnBeginPlayStartStreaming()
{
	DriverMediaConfig TempMediaConfig;
	if (StreamingDataWrapperPtr->GetDriverMediaConfig(TempMediaConfig))
	{
		MediaConfig = TempMediaConfig;
	}

	if (StreamingDriverInterfacePtr
		&& StreamingDriverInterfacePtr->IsConnected()
		&& StreamingDriverInterfacePtr->StartStreaming(MediaConfig) != DriverResultCode::kOk)
	{
		PXR_LOGD(PxrUnreal, "PXR_LivePreview StartStreaming Failed!");
		return false;
	}

	PXR_LOGD(PxrUnreal, "PXR_LivePreview ConnectServer Successed!");

	return true;
}

bool FPICOXRDPManager::OnEndPlayStopStreaming()
{
	if (StreamingDriverInterfacePtr
		&& StreamingDriverInterfacePtr->IsConnected()
		&& StreamingDriverInterfacePtr->StopStreaming() != DriverResultCode::kOk)
	{
		PXR_LOGD(PxrUnreal, "PXR_LivePreview StopStreaming Failed!");
		return false;
	}

	return true;
}

bool FPICOXRDPManager::InitializeLivePreview()
{
	if (!CreateStreamingInterface())
	{
		return false;
	}

	if (!CreateSharedTexture2D())
	{
		PXR_LOGD(PxrUnreal, "PXR_LivePreview CreateSharedTexture2D Failed!");
		return false;
	}
	PXR_LOGD(PxrUnreal, "PXR_LivePreview CreateSharedTexture2D Successed!");

	MediaConfig.frame_rate() = 72;
	MediaConfig.render_width() = 1920;
	MediaConfig.render_height() = 1920;

	return true;
}

void FPICOXRDPManager::ShutDownLivePreview()
{
	DisConnectStreamingServer();
}

bool FPICOXRDPManager::IsStreaming()
{
	return StreamingDriverInterfacePtr
		&& StreamingDriverInterfacePtr->IsConnected()
		&& StreamingDataWrapperPtr
		&& StreamingDataWrapperPtr->IsStreaming();
}

void FPICOXRDPManager::OnDeviceConnectionChanged(const DeviceInfo& device_info, int32 State)
{
	PXR_LOGD(PxrUnreal, "PXR_LivePreview OnDeviceConnectionChanged State:%d!", State);
	bConnectToServiceSucceed = State ? true : false;
}

bool FPICOXRDPManager::CreateStreamingInterface()
{
	if (!StreamingDataWrapperPtr.IsValid())
	{
		StreamingDataWrapperPtr = MakeShareable(new StreamingDataReceiveWrapper());
		StreamingDataWrapperPtr->OnDeviceConnectedResultsEvent.BindStatic(&OnDeviceConnectionChanged);
	}

	if (!StreamingDriverInterfacePtr)
	{
		StreamingDriverInterfacePtr = StreamingDriverInterface::Create(StreamingDataWrapperPtr.Get());
	}

	return true;
}

bool FPICOXRDPManager::ConnectStreamingServer()
{
	if (!StreamingDriverInterfacePtr)
	{
		PXR_LOGD(PxrUnreal, "PXR_LivePreview ConnectStreamingServer Create Failed!");
		return false;
	}

	if (StreamingDriverInterfacePtr->ConnectToService() != DriverResultCode::kOk)
	{
		PXR_LOGD(PxrUnreal, "PXR_LivePreview ConnectStreamingServer ConnectToService Failed!");
		return false;
	}

	bConnectToServiceSucceed = true;

	return bConnectToServiceSucceed;
}

bool FPICOXRDPManager::DisConnectStreamingServer()
{
	if (!bConnectToServiceSucceed
		|| !StreamingDriverInterfacePtr
		|| StreamingDriverInterfacePtr->DisconnectFromService() != DriverResultCode::kOk)
	{
		return false;
	}
	return true;
}

bool FPICOXRDPManager::SetDisconnectFromServiceEvent(const FOnConnectFromServiceEvent& OnConnectFromServiceEvent)
{
	if (StreamingDataWrapperPtr)
	{
		StreamingDataWrapperPtr->OnConnectFromServiceEvent = OnConnectFromServiceEvent;
		return true;
	}
	return false;
}

bool FPICOXRDPManager::SetFovUpdatedFromServiceEvent(const FOnFovUpdatedFromServiceEvent& OnFovUpdatedFromServiceEvent)
{
	if (StreamingDataWrapperPtr)
	{
		StreamingDataWrapperPtr->OnFovUpdatedFromServiceEvent = OnFovUpdatedFromServiceEvent;
		return true;
	}
	return false;
}

FTextureRHIRef FPICOXRDPManager::GetLeftBindingTexture()
{
	return LeftBindingTexture;
}

FTextureRHIRef FPICOXRDPManager::GetRightBindingTexture()
{
	return RightBindingTexture;
}

TArray<FTextureRHIRef> FPICOXRDPManager::GetLeftRHITextureSwapChain()
{
	return LeftRHITextureSwapChain;
}

TArray<FTextureRHIRef> FPICOXRDPManager::GetRightRHITextureSwapChain()
{
	return RightRHITextureSwapChain;
}

static const uint32 DPSwapChainLength = 1;
bool FPICOXRDPManager::SendMessage(int SwapChainIndex)
{
	pxr_base::DriverSharedImages submit_message;
	submit_message.shared_handle(0) = LeftDstTextureHandles[SwapChainIndex];
	submit_message.shared_handle(1) = RightDstTextureHandles[SwapChainIndex];
	FConditionalScopeLock ScopeLock(&StreamingDataWrapperPtr->StateLock, true);
	submit_message.SetPostion_(StreamingDataWrapperPtr->GetHMDPosition());
	submit_message.SetRotation_(StreamingDataWrapperPtr->GetHMDRotation());

	PXR_LOGV(PxrUnreal, "PXR_LivePreview Left Shared Handle ID:%lu", HandleToULong(LeftDstTextureHandles[SwapChainIndex]));
	PXR_LOGV(PxrUnreal, "PXR_LivePreview Right Shared Handle ID:%lu", HandleToULong(RightDstTextureHandles[SwapChainIndex]));
	if (StreamingDriverInterfacePtr->SubmitLayerImage(submit_message) == DriverResultCode::kOk)
	{
		return true;
	}
	return false;
}

int ColorRGBA(int alpha, int red, int green, int blue)
{
	return (alpha << 24 | red << 16 | green << 8 | blue);
}

bool FPICOXRDPManager::CreateSharedTexture2D()
{
	UPICOXRDPSettings* HMDSettings = GetMutableDefault<UPICOXRDPSettings>();

	auto device = static_cast<ID3D11Device*>(GDynamicRHI->RHIGetNativeDevice());
	if (device == nullptr)
	{
		return false;
	}
	if (LeftBindingTexture && RightBindingTexture)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC sharedTexture2Ddesc;
	ZeroMemory(&sharedTexture2Ddesc, sizeof(sharedTexture2Ddesc));
	sharedTexture2Ddesc.Height = 1920; //m_lastSize.Height;
	sharedTexture2Ddesc.Width = 1920; // m_lastSize.Width;
	sharedTexture2Ddesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	sharedTexture2Ddesc.ArraySize = 1;
	sharedTexture2Ddesc.MipLevels = 1;
	sharedTexture2Ddesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexture2Ddesc.SampleDesc.Count = 1;
	sharedTexture2Ddesc.CPUAccessFlags = 0;
	sharedTexture2Ddesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	sharedTexture2Ddesc.MiscFlags = HMDSettings->GraphicQuality==EGraphicQuality::High?D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX:D3D11_RESOURCE_MISC_SHARED;

	uint32_t m_color = ColorRGBA(255, 108, 108, 255);

	std::vector<uint32_t> textureMap(1920 * 1920, m_color);
	uint32_t* pData = textureMap.data();

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pData;
	sd.SysMemPitch = 1920 * sizeof(uint32_t);
	sd.SysMemSlicePitch = 1920 * 1920 * sizeof(uint32_t);

	for (uint32 SwapChainIter = 0; SwapChainIter < DPSwapChainLength; ++SwapChainIter)
	{
		TRefCountPtr<ID3D11Texture2D> LeftDstTexture;
		TRefCountPtr<ID3D11Texture2D> RightDstTexture;

		void* LeftDstTextureHandle;
		void* RightDstTextureHandle;

		TRefCountPtr<IDXGIResource> LeftDXGIResource;
		TRefCountPtr<IDXGIResource> RightDXGIResource;

		FTexture2DRHIRef LeftTempTexture;
		FTexture2DRHIRef RightTempTexture;

		if (device->CreateTexture2D(&sharedTexture2Ddesc, &sd, LeftDstTexture.GetInitReference()) != S_OK
			|| device->CreateTexture2D(&sharedTexture2Ddesc, &sd, RightDstTexture.GetInitReference()) != S_OK
		)
		{
			PXR_LOGD(PxrUnreal, "PXR_LivePreview DstSwapChainTextures:%d Created faild!", SwapChainIter);
			return false;
		}
		
		LeftDstTexture->QueryInterface(_uuidof(IDXGIKeyedMutex), (void**)&LeftKeyedMutex);
		RightDstTexture->QueryInterface(_uuidof(IDXGIKeyedMutex), (void**)&RightKeyedMutex);

		LeftDstSwapChainTextures.Add(LeftDstTexture);
		RightDstSwapChainTextures.Add(RightDstTexture);

		if (LeftDstSwapChainTextures[SwapChainIter] && RightDstSwapChainTextures[SwapChainIter])
		{
			PXR_LOGD(PxrUnreal, "PXR_LivePreview DstSwapChainTextures:%d Created faild!");

			LeftDstSwapChainTextures[SwapChainIter]->QueryInterface(LeftDXGIResource.GetInitReference());
			RightDstSwapChainTextures[SwapChainIter]->QueryInterface(RightDXGIResource.GetInitReference());

			const HRESULT LeftSharedHandleResult = LeftDXGIResource->GetSharedHandle(&LeftDstTextureHandle);
			const HRESULT RightSharedHandleResult = RightDXGIResource->GetSharedHandle(&RightDstTextureHandle);

			LeftDstTextureHandles.Add(LeftDstTextureHandle);
			RightDstTextureHandles.Add(RightDstTextureHandle);

			if (FAILED(LeftSharedHandleResult) || FAILED(RightSharedHandleResult))
			{
				PXR_LOGD(PxrUnreal, "PXR_LivePreview GetSharedHandle faild!");
				return false;
			}
			else
			{
				PXR_LOGD(PxrUnreal, "PXR_LivePreview Left Shared Handle ID:%lu", HandleToULong(LeftDstTextureHandle));
				PXR_LOGD(PxrUnreal, "PXR_LivePreview Right Shared Handle ID:%lu", HandleToULong(RightDstTextureHandle));

				CreateRHITexture(LeftDstSwapChainTextures[SwapChainIter], PF_R8G8B8A8, LeftTempTexture);
				CreateRHITexture(RightDstSwapChainTextures[SwapChainIter], PF_R8G8B8A8, RightTempTexture);
				LeftRHITextureSwapChain.Add((FTextureRHIRef&)LeftTempTexture);
				RightRHITextureSwapChain.Add((FTextureRHIRef&)RightTempTexture);
				if (LeftRHITextureSwapChain[SwapChainIter]->IsValid() || RightRHITextureSwapChain[SwapChainIter]->IsValid())
				{
					PXR_LOGD(PxrUnreal, "PXR_LivePreview SwapChainTextures OK");
				}
			}

			if (LeftBindingTexture == nullptr)
			{
				LeftBindingTexture = GDynamicRHI->RHICreateAliasedTexture((FTextureRHIRef&)LeftTempTexture);
			}
			if (RightBindingTexture == nullptr)
			{
				RightBindingTexture = GDynamicRHI->RHICreateAliasedTexture((FTextureRHIRef&)RightTempTexture);
			}
		}
		else
		{
			PXR_LOGD(PxrUnreal, "PXR_LivePreview Shared Texture Created faild!");
			return false;
		}
	}

	return true;
}

TComPtr<IDXGIKeyedMutex> FPICOXRDPManager::GetLeftKeyedMutex()
{
	return LeftKeyedMutex;
}

TComPtr<IDXGIKeyedMutex> FPICOXRDPManager::GetRightKeyedMutex()
{
	return RightKeyedMutex;
}


bool FPICOXRDPManager::LockKeyedMutex(UINT64 Key,DWORD TimeOut_ms)
{
	if((!LeftKeyedMutex||LeftKeyedMutex->AcquireSync(Key,TimeOut_ms)!= S_OK)
	||(!RightKeyedMutex||RightKeyedMutex->AcquireSync(Key,TimeOut_ms)!= S_OK))
	{
		PXR_LOGD(PxrUnreal, "PXR_LivePreview IDXGIKeyedMutex::AcquireSync Failed!");
		return false;
	}
	return true;
}

void FPICOXRDPManager::UnlockKeyedMutex(UINT64 Key)
{
	if (LeftKeyedMutex&&RightKeyedMutex)
	{
		LeftKeyedMutex->ReleaseSync(Key);
		RightKeyedMutex->ReleaseSync(Key);
	}
}


bool FPICOXRDPManager::CreateRHITexture(ID3D11Texture2D* OpenedSharedResource, EPixelFormat Format, FTexture2DRHIRef& OutTexture)
{
	FD3D11DynamicRHI* DynamicRHI = static_cast<FD3D11DynamicRHI*>(GDynamicRHI);
	OutTexture = DynamicRHI->RHICreateTexture2DFromResource(
		Format, TexCreate_RenderTargetable | TexCreate_ShaderResource, FClearValueBinding::None, OpenedSharedResource).GetReference();
	return OutTexture.IsValid();
}

void FPICOXRDPManager::GetHMDPositionAndRotation(FVector& OutPostion, FQuat& OutQuat)
{
	if (StreamingDataWrapperPtr)
	{
		const pxr_base::Vector position = StreamingDataWrapperPtr->GetHMDPosition();
		const pxr_base::Rotation rotation = StreamingDataWrapperPtr->GetHMDRotation();
		OutPostion.X = position.x();
		OutPostion.Y = position.y();
		OutPostion.Z = position.z();
		OutQuat.X = rotation.x();
		OutQuat.Y = rotation.y();
		OutQuat.Z = rotation.z();
		OutQuat.W = rotation.w();
	}
	else
	{
		OutPostion = FVector::ZeroVector;
		OutQuat = FQuat::Identity;
	}
}

void FPICOXRDPManager::GetControllerPositionAndRotation(EControllerHand DeviceHand, float WorldScale, FVector& OutPostion, FRotator& OutQuat)
{
	if (StreamingDataWrapperPtr)
	{
		OutPostion.X = -StreamingDataWrapperPtr->GetControllerPosition(DeviceHand).z();
		OutPostion.Y = StreamingDataWrapperPtr->GetControllerPosition(DeviceHand).x();
		OutPostion.Z = StreamingDataWrapperPtr->GetControllerPosition(DeviceHand).y();
		OutPostion = OutPostion * WorldScale;
		OutQuat = FQuat(StreamingDataWrapperPtr->GetControllerRotation(DeviceHand).z(), -StreamingDataWrapperPtr->GetControllerRotation(DeviceHand).x(), -StreamingDataWrapperPtr->GetControllerRotation(DeviceHand).y(), StreamingDataWrapperPtr->GetControllerRotation(DeviceHand).w()).Rotator();
	}
	else
	{
		OutPostion = FVector::ZeroVector;
		OutQuat = FRotator::ZeroRotator;
	}
}

bool FPICOXRDPManager::GetControllerConnectionStatus(EControllerHand DeviceHand)
{
	return StreamingDataWrapperPtr ? StreamingDataWrapperPtr->GetConnectionStatus(DeviceHand) : false;
}

int32 FPICOXRDPManager::GetControllerButtonStatus(EControllerHand DeviceHand)
{
	return StreamingDataWrapperPtr ? StreamingDataWrapperPtr->GetStateValue(DeviceHand) : 0;
}

void FPICOXRDPManager::GetControllerAxisValue(EControllerHand DeviceHand, double& JoyStickX, double& JoyStickY, float& TriggerValue, float& GripValue)
{
	if (StreamingDataWrapperPtr)
	{
		JoyStickX = StreamingDataWrapperPtr->GetJoystickX(DeviceHand);
		JoyStickY = StreamingDataWrapperPtr->GetJoystickY(DeviceHand);
		TriggerValue = StreamingDataWrapperPtr->GetTriggerValue(DeviceHand);
		GripValue = StreamingDataWrapperPtr->GetGripValue(DeviceHand);
	}
}

bool FPICOXRDPManager::IsConnectToServiceSucceed()
{
	if (bConnectToServiceSucceed && StreamingDriverInterfacePtr)
	{
		return StreamingDriverInterfacePtr->IsConnected();
	}
	return false;
}

bool FPICOXRDPManager::GetPICOXRFrustumDP(FPICOXRFrustumDP& PICOXRFrustumDP)
{
	DeviceFovInfo TempDeviceFovInfo;
	if (StreamingDataWrapperPtr
		&& StreamingDataWrapperPtr->GetDeviceFovInfo(TempDeviceFovInfo))
	{
		PICOXRFrustumDP.FovUp = TempDeviceFovInfo.up;
		PICOXRFrustumDP.FovDown = TempDeviceFovInfo.down;
		PICOXRFrustumDP.FovLeft = TempDeviceFovInfo.left;
		PICOXRFrustumDP.FovRight = TempDeviceFovInfo.right;
		PICOXRFrustumDP.Type = static_cast<EEyeType>(TempDeviceFovInfo.eye_type);

		PXR_LOGD(PxrUnreal, "PXR_LivePreview PICOXRFrustumDP.FovUp:%f", PICOXRFrustumDP.FovUp);
		PXR_LOGD(PxrUnreal, "PXR_LivePreview PICOXRFrustumDP.FovDown:%f", PICOXRFrustumDP.FovDown);
		PXR_LOGD(PxrUnreal, "PXR_LivePreview PICOXRFrustumDP.FovLeft:%f", PICOXRFrustumDP.FovLeft);
		PXR_LOGD(PxrUnreal, "PXR_LivePreview PICOXRFrustumDP.FovRight:%f", PICOXRFrustumDP.FovRight);
		PXR_LOGD(PxrUnreal, "PXR_LivePreview PICOXRFrustumDP.Type:%d", PICOXRFrustumDP.Type);

		return true;
	}

	return false;
}


