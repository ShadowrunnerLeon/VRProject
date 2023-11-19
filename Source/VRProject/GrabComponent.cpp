#include "GrabComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bSimulateOnDrop = false;
	hapticEffect = FindObject<UHapticFeedbackEffect_Base>(FTopLevelAssetPath("/Game/VRTemplate/Haptics/GrabHapticEffect"));
}

void UGrabComponent::BeginPlay()
{
	SetShouldSimulateOnDrop();
	UPrimitiveComponent* primComponent = Cast<UPrimitiveComponent>(GetAttachParent());
	primComponent->SetCollisionProfileName(FName("PhysicsActor"));
}

bool UGrabComponent::TryGrab(UMotionControllerComponent* motionControllerComponent)
{
	UPrimitiveComponent* primComponent = Cast<UPrimitiveComponent>(GetAttachParent());
	primComponent->SetSimulatePhysics(false);

	bool isAttached = GetAttachParent()->K2_AttachToComponent
	(
		Cast<USceneComponent>(motionControllerComponent),
		FName("None"),
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true
	);

	if (!isAttached)
	{
		FString errMsg = (TEXT("Attaching %s to %s failed"), UKismetSystemLibrary::GetDisplayName(GetAttachParent()), UKismetSystemLibrary::GetDisplayName(this));
		UKismetSystemLibrary::PrintString(GetWorld(), errMsg);
	}
	else
	{
		APlayerController* palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (motionControllerComponent->MotionSource == FName("Left"))
		{
			palyerController->PlayHapticEffect(hapticEffect, EControllerHand::Left);
		}
		else
		{
			palyerController->PlayHapticEffect(hapticEffect, EControllerHand::Right);
		}
	}

	return isAttached;
}

void UGrabComponent::TryRelease()
{
	if (bSimulateOnDrop)
	{
		UPrimitiveComponent* primComponent = Cast<UPrimitiveComponent>(GetAttachParent());
		primComponent->SetSimulatePhysics(true);
	}
	else
	{
		GetAttachParent()->K2_DetachFromComponent
		(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld
		);
	}
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void UGrabComponent::SetShouldSimulateOnDrop()
{
	UPrimitiveComponent* primComponent = Cast<UPrimitiveComponent>(GetAttachParent());
	bSimulateOnDrop = primComponent->IsAnySimulatingPhysics();
}

