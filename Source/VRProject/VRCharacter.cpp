#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "InputConfigData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GrabComponent.h"

AVRCharacter::AVRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	camera->SetupAttachment(RootComponent);

	leftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	leftMotionController->SetupAttachment(RootComponent);
	leftMotionController->SetTrackingMotionSource("Left");

	rightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	rightMotionController->SetupAttachment(RootComponent);
	rightMotionController->SetTrackingMotionSource("Right");
}

void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVRCharacter::Tick(float DeltaTime)
{
}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(inputMapping, 0);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	inputComponent->BindAction(inputActions->turn, ETriggerEvent::Started, this, &AVRCharacter::Turn_Started);
	inputComponent->BindAction(inputActions->move, ETriggerEvent::Triggered, this, &AVRCharacter::Move_Triggered);

	inputComponent->BindAction(inputActions->grabLeft, ETriggerEvent::Started, this, &AVRCharacter::Grab_Started, leftMotionController);
	inputComponent->BindAction(inputActions->grabLeft, ETriggerEvent::Completed, this, &AVRCharacter::Grab_Completed, leftMotionController);
	inputComponent->BindAction(inputActions->grabRight, ETriggerEvent::Started, this, &AVRCharacter::Grab_Started, rightMotionController);
	inputComponent->BindAction(inputActions->grabRight, ETriggerEvent::Completed, this, &AVRCharacter::Grab_Completed, rightMotionController);
}

void AVRCharacter::Turn_Started(const FInputActionValue& value)
{
	float angle = (value.Get<float>() > 0.f) ? 45.f : -45.f;
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FRotator rot = GetControlRotation();
	rot.Yaw += angle;
	controller->SetControlRotation(rot);
}

void AVRCharacter::Move_Triggered(const FInputActionValue& value)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), L"Move_Triggered", true, true, FLinearColor::Green);
	AddMovementInput(camera->GetForwardVector(), value.Get<float>());
}

void AVRCharacter::Grab_Started(const FInputActionValue& value, UMotionControllerComponent* motionController)
{
	UGrabComponent* grabComponent = GetGrabComponentNearMotionController(motionController);
	if (!IsValid(grabComponent)) return;

	bool isAttached = grabComponent->TryGrab(motionController);
	if (!isAttached) return;

	if (motionController->MotionSource == FName("Left"))
	{
		leftGrabComponent = grabComponent;
		rightGrabComponent = (leftGrabComponent == rightGrabComponent) ? nullptr : rightGrabComponent;
	}
	else
	{
		rightGrabComponent = grabComponent;
		leftGrabComponent = (leftGrabComponent == rightGrabComponent) ? nullptr : leftGrabComponent;
	}
}

void AVRCharacter::Grab_Completed(const FInputActionValue& value, UMotionControllerComponent* motionController)
{
	if (motionController->MotionSource == FName("Left"))
	{
		leftGrabComponent->TryRelease();
		leftGrabComponent = nullptr;
	}
	else
	{
		rightGrabComponent->TryRelease();
		rightGrabComponent = nullptr;
	}
}

UGrabComponent* AVRCharacter::GetGrabComponentNearMotionController(UMotionControllerComponent* motionController)
{
	UGrabComponent* localNearestGrabComponent = nullptr;
	FVector localGripPosition = motionController->GetComponentLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjects;
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;

	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),
		localGripPosition,
		localGripPosition,
		grabRadiusFromGripPosition,
		traceObjects,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		outHit,
		true);

	TArray<UGrabComponent*> grabComponents;
	outHit.GetActor()->GetComponents(grabComponents);

	for (size_t index = 0; index < grabComponents.Num(); ++index)
	{
		FVector grabComponentLocation = grabComponents[index]->GetComponentLocation();
		float squaredLength = UKismetMathLibrary::VSizeSquared(grabComponentLocation - localGripPosition);

		if (squaredLength < localNearestComponentDistance)
		{
			localNearestComponentDistance = squaredLength;
			localNearestGrabComponent = grabComponents[index];
		}
	}

	return localNearestGrabComponent;
}

