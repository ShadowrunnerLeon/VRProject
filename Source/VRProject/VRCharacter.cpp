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
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "VRTeleportVisualizer.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

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

	teleportTraceNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("teleportTraceNiagara"));
	teleportTraceNiagaraSystem->SetupAttachment(RootComponent);
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

	inputComponent->BindAction(inputActions->teleport, ETriggerEvent::Started, this, &AVRCharacter::Teleport_Started);
	inputComponent->BindAction(inputActions->teleport, ETriggerEvent::Triggered, this, &AVRCharacter::Teleport_Triggered);
	inputComponent->BindAction(inputActions->teleport, ETriggerEvent::Completed, this, &AVRCharacter::Teleport_Completed);
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
	//UKismetSystemLibrary::PrintString(GetWorld(), L"Grab_Started", true, true, FLinearColor::Green);
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
	//UKismetSystemLibrary::PrintString(GetWorld(), L"Grab_Completed", true, true, FLinearColor::Green);
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

void AVRCharacter::Teleport_Started(const FInputActionValue& value)
{
	UKismetSystemLibrary::PrintString(GetWorld(), L"Teleport_Started", true, true, FLinearColor::Green);
	teleportTraceNiagaraSystem->SetVisibility(true);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	spawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;

	vrTeleportVisualizerRef = GetWorld()->SpawnActor<AVRTeleportVisualizer>
		(
			vrTeleportVisualizer,
			FVector(0.f, 0.f, 0.f),
			FRotator(0.f, 0.f, 0.f),
			spawnParams
		);
}

void AVRCharacter::Teleport_Triggered(const FInputActionValue& value)
{
	UKismetSystemLibrary::PrintString(GetWorld(), L"Teleport_Triggered", true, true, FLinearColor::Green);
	FVector startPos = leftMotionController->GetComponentLocation();
	FVector forwardVec = leftMotionController->GetForwardVector();

	// seq 1
	FVector convVec = UKismetMathLibrary::Conv_DoubleToVector(localTeleportLaunchSpeed);
	FVector launchVelocity = startPos * convVec;

	FHitResult outHit;
	TArray<FVector> teleportTracePathPositions;
	FVector outLastTraceDestination;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> actorsToIgnore;

	objectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECollisionChannel::ECC_WorldStatic));

	UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType
	(
		GetWorld(),
		outHit,
		teleportTracePathPositions,
		outLastTraceDestination,
		startPos,
		launchVelocity,
		true,
		localTeleportProjectileRadius,
		objectTypes,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		0.f
	);

	teleportTracePathPositions.Insert(startPos, 0);

	isValidTeleportLocation = IsValidTeleportLocation(outHit);
	FVector teleportProjectedLocation(projectedLocation.X, projectedLocation.Y, projectedLocation.Z - localNavMeshCellHeight);

	if (isValidTeleportLocation)
	{
		USceneComponent* rootComponent = vrTeleportVisualizerRef->GetRootComponent();
		rootComponent->SetVisibility(true, true);
	}

	// seq 2
	vrTeleportVisualizerRef->SetActorLocation(projectedLocation);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(teleportTraceNiagaraSystem, FName("User.PointArray"), teleportTracePathPositions);
}

void AVRCharacter::Teleport_Completed(const FInputActionValue& value)
{
	UKismetSystemLibrary::PrintString(GetWorld(), L"Teleport_Completed", true, true, FLinearColor::Green);

	if (IsValid(vrTeleportVisualizer)) vrTeleportVisualizerRef->Destroy();
	teleportTraceNiagaraSystem->SetVisibility(false);

	USceneComponent* rootComponent = vrTeleportVisualizerRef->GetRootComponent();
	rootComponent->SetVisibility(false, true);

	TryTeleport();
}

bool AVRCharacter::IsValidTeleportLocation(FHitResult& hit)
{
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	bool isValid = navSystem->K2_ProjectPointToNavigation(GetWorld(), hit.Location, projectedLocation, nullptr, nullptr);
	return isValid;
}

void AVRCharacter::TryTeleport()
{
	if (!isValidTeleportLocation) return;

	FVector cameraLocation = camera->GetRelativeLocation();
	cameraLocation.Z = 0;

	FRotator actorRotation = GetActorRotation();
	FRotator rot(0.f, 0.f, actorRotation.Yaw);
	FVector rotatedVector = rot.RotateVector(cameraLocation);

	FVector destLocation = projectedLocation - rotatedVector;
	TeleportTo(destLocation, FRotator(0.f, 0.f, actorRotation.Yaw));
}

