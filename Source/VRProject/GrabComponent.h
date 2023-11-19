#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MotionControllerComponent.h"
#include "GrabComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRPROJECT_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

	bool bSimulateOnDrop;
	UHapticFeedbackEffect_Base* hapticEffect;

	void SetShouldSimulateOnDrop();

public:	
	UGrabComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryGrab(UMotionControllerComponent* motionControllerComponent);
	void TryRelease();
};
