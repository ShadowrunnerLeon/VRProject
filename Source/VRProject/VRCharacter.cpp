#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "InputConfigData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
	Super::Tick(DeltaTime);

}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(inputMapping, 0);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	inputComponent->BindAction(inputActions->turn, ETriggerEvent::Started, this, &AVRCharacter::Turn_Started);
	inputComponent->BindAction(inputActions->move, ETriggerEvent::Started, this, &AVRCharacter::Move_Started);
}

void AVRCharacter::Turn_Started(const FInputActionValue& value)
{
	float angle = (value.Get<float>() > 0.f) ? 45.f : -45.f;
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FRotator rot = GetControlRotation();
	rot.Yaw += angle;
	controller->SetControlRotation(rot);
}

void AVRCharacter::Move_Started(const FInputActionValue& value)
{
	UKismetSystemLibrary::PrintString(GetWorld(), L"Move_Started", true, true, FLinearColor::Green);
	AddMovementInput(rightMotionController->GetForwardVector(), value.Get<float>());
}

