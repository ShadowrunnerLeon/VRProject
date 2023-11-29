#include "VRTeleportVisualizer.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AVRTeleportVisualizer::AVRTeleportVisualizer()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	root->SetupAttachment(RootComponent);

	teleportRing = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_TeleportRing"));
	teleportRing->SetupAttachment(root);

	playAreaBounds = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_PlayAreaBounds"));
	playAreaBounds->SetupAttachment(root);
}

void AVRTeleportVisualizer::BeginPlay()
{
	FVector2D vec2D = UHeadMountedDisplayFunctionLibrary::GetPlayAreaBounds(EHMDTrackingOrigin::Stage);
	playAreaBounds->SetNiagaraVariableVec3(TEXT("User.PlayAreaBounds"), FVector(vec2D, 0.f));
}

void AVRTeleportVisualizer::Tick(float DeltaTime)
{
	FTransform pawnTransform = (UGameplayStatics::GetPlayerPawn(this, 0))->GetActorTransform();
	FVector cameraLocation = (UGameplayStatics::GetPlayerCameraManager(this, 0))->GetCameraLocation();

	FVector invertedVec = UKismetMathLibrary::InverseTransformLocation(pawnTransform, cameraLocation);
	FVector negatedVec = UKismetMathLibrary::NegateVector(invertedVec);
	negatedVec.Z = 0.f;

	playAreaBounds->SetRelativeLocation(negatedVec);

	FRotator pawnRotation = (UGameplayStatics::GetPlayerPawn(this, 0))->GetActorRotation();
	SetActorRotation(pawnRotation);
}

