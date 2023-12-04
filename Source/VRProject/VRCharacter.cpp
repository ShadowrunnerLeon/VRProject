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
#include "Components/SphereComponent.h"
#include "GrabCube.h"

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

	leftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("leftSphere"));
	leftSphere->SetupAttachment(leftMotionController);

	rightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("rightSphere"));
	rightSphere->SetupAttachment(rightMotionController);
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
	UKismetSystemLibrary::PrintString(GetWorld(), L"Grab_Started", true, true, FLinearColor::Green);
	TArray<AActor*> overlappingActors;
	leftSphere->GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		if (actor->IsA(AGrabCube::StaticClass()))
		{
			UGrabComponent* grabComponent = Cast<AGrabCube>(actor)->GetGrabComponent();
			grabComponent->TryGrab(motionController);
		
			if (motionController->GetTrackingMotionSource() == "Left")
			{
				leftGrabComponent = grabComponent;
				rightGrabComponent = nullptr;
			}
			else if (motionController->GetTrackingMotionSource() == "Right")
			{
				rightGrabComponent = grabComponent;
				leftGrabComponent = nullptr;
			}
		}
	}
}

void AVRCharacter::Grab_Completed(const FInputActionValue& value, UMotionControllerComponent* motionController)
{
	UKismetSystemLibrary::PrintString(GetWorld(), L"Grab_Completed", true, true, FLinearColor::Green);
	if ((motionController->GetTrackingMotionSource() == "Left") && IsValid(leftGrabComponent))
	{
		leftGrabComponent->TryRelease();
		leftGrabComponent = nullptr;
	}
	else if ((motionController->GetTrackingMotionSource() == "Right") && IsValid(rightGrabComponent))
	{
		rightGrabComponent->TryRelease();
		rightGrabComponent = nullptr;
	}
}

