// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VRCharacter.generated.h"

class UMotionControllerComponent;
class UCameraComponent;
class UInputConfigData;
class UGrabComponent;

UCLASS()
class VRPROJECT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

	float localNearestComponentDistance = 9999999.f;
	const float grabRadiusFromGripPosition = 6.f;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* camera;

	UPROPERTY(EditDefaultsOnly, Category = "Motion")
	class UMotionControllerComponent* leftMotionController;

	UPROPERTY(EditDefaultsOnly, Category = "Motion")
	class UMotionControllerComponent* rightMotionController;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* inputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputConfigData* inputActions;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	UGrabComponent* leftGrabComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	UGrabComponent* rightGrabComponent;

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Turn_Started(const FInputActionValue& value);
	void Move_Triggered(const FInputActionValue& value);

	void Grab_Started(const FInputActionValue& value, UMotionControllerComponent* motionController);
	void Grab_Completed(const FInputActionValue& value, UMotionControllerComponent* motionController);
	UGrabComponent* GetGrabComponentNearMotionController(UMotionControllerComponent* motionController);

};
