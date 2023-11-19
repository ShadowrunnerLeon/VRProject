#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRTeleportVisualizer.generated.h"

class UNiagaraComponent;

UCLASS()
class VRPROJECT_API AVRTeleportVisualizer : public AActor
{
	GENERATED_BODY()

	public:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport")
		class UNiagaraComponent* teleportRing;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport")
		class UNiagaraComponent* playAreaBounds;

	public:	
		AVRTeleportVisualizer();

	protected:
		virtual void BeginPlay() override;

	public:	
		virtual void Tick(float DeltaTime) override;

};
