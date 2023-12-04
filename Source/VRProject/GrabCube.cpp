#include "GrabCube.h"
#include "GrabComponent.h"
#include "Components/StaticMeshComponent.h"


AGrabCube::AGrabCube()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mesh->SetupAttachment(RootComponent);

	grabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));
	grabComponent->SetupAttachment(mesh);
}

UGrabComponent* AGrabCube::GetGrabComponent()
{
	return grabComponent;
}

void AGrabCube::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrabCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

