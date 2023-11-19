#include "VRCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "NiagaraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "VRTeleportVisualizer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NavigationSystem.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

AVRCharacter::AVRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	camera->SetupAttachment(RootComponent);

	leftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	rightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	leftMotionControllerAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionControllerAim"));
	rightMotionControllerAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionControllerAim"));
	teleportTraceNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportTraceNiagaraSystem"));
}

void AVRCharacter::BeginPlay()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void AVRCharacter::SnapTurn_Started(const FInputActionValue& value)
{
	FVector cameraLocation = camera->GetComponentLocation();
	FTransform cameraTransform = camera->GetRelativeTransform();

	FRotator newRotation(FRotator(0.f, 0.f, value.Get<float>()));
	FTransform newTransform((GetActorRotation() + newRotation), GetActorLocation());

	AddActorWorldRotation(newRotation);
	FVector newLocation = (cameraLocation - (cameraTransform * newTransform).GetLocation()) + GetActorLocation();
	SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);
}

void AVRCharacter::Teleport_Started()
{
	bTeleportTraceActive = true;
	teleportTraceNiagaraSystem->SetVisibility(true);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	spawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;

	vrTeleportVisualizer = GetWorld()->SpawnActor<AVRTeleportVisualizer>
		(
			AVRTeleportVisualizer::StaticClass(),
			FVector(0.f, 0.f, 0.f),
			FRotator(0.f, 0.f, 0.f),
			spawnParams
		);
}

void AVRCharacter::Teleport_Triggered()
{
	FVector startPos = rightMotionControllerAim->GetComponentLocation();
	FVector forwardVec = rightMotionControllerAim->GetForwardVector();

	// seq 1
	FVector convVec = UKismetMathLibrary::Conv_DoubleToVector(localTeleportLaunchSpeed);
	FVector launchVelocity = startPos * convVec;

	FHitResult outHit;
	TArray<FVector> teleportTracePathPositions;
	FVector outLastTraceDestination;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> actorsToIgnore;

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

	FVector projectedLocation;

	bool isValidTelportLocation = IsValidTeleportLocation(outHit, projectedLocation);
	FVector teleportProjectedLocation(projectedLocation.X, projectedLocation.Y, projectedLocation.Z - localNavMeshCellHeight);

	if (isValidTelportLocation)
	{
		USceneComponent* rootComponent = vrTeleportVisualizer->GetRootComponent();
		rootComponent->SetVisibility(true, true);
	}

	// seq 2
	vrTeleportVisualizer->SetActorLocation(projectedLocation);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(teleportTraceNiagaraSystem, FName("User.PointArray"), teleportTracePathPositions);
}

void AVRCharacter::Teleport_Completed()
{
	bTeleportTraceActive = false;

	if (IsValid(vrTeleportVisualizer)) vrTeleportVisualizer->Destroy();
	teleportTraceNiagaraSystem->SetVisibility(false);
}

void AVRCharacter::Grab_Started(UMotionControllerComponent* motionController)
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

void AVRCharacter::Grab_Completed(UMotionControllerComponent* motionController)
{
	if (motionController->MotionSource == FName("Left"))
	{
		leftGrabComponent = nullptr;
	}
	else
	{
		rightGrabComponent = nullptr;
	}
}

bool AVRCharacter::IsValidTeleportLocation(FHitResult& hit, FVector& projectedLocation)
{
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation navLocation;
	bool isValid = navSystem->ProjectPointToNavigation(hit.Location, navLocation);

	projectedLocation = navLocation.Location;
	return isValid;
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
	inputComponent->BindAction(inputActions->snapTurn, ETriggerEvent::Started, this, &AVRCharacter::SnapTurn_Started);
}

