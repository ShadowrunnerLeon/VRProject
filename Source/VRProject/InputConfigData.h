#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputConfigData.generated.h"

UCLASS()
class VRPROJECT_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* turn;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* move;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* grabLeft;
};
