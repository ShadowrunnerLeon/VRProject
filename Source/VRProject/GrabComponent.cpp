#include "GrabComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

void UGrabComponent::BeginPlay()
{
}

bool UGrabComponent::TryGrab(UMotionControllerComponent* motionControllerComponent)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), L"Grab", true, true, FLinearColor::Green);
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

	return isAttached;
}

void UGrabComponent::TryRelease()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), L"Release", true, true, FLinearColor::Green);
	UPrimitiveComponent* primComponent = Cast<UPrimitiveComponent>(GetAttachParent());
	primComponent->SetSimulatePhysics(true);

	GetAttachParent()->K2_DetachFromComponent
	(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld
	);
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}


