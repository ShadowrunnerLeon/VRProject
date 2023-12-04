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
class USphereComponent;

UCLASS()
class VRPROJECT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

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
	class UGrabComponent* leftGrabComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* rightGrabComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class USphereComponent* leftSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class USphereComponent* rightSphere;

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

	void GrabLeft_Started(const FInputActionValue& value);
	void GrabLeft_Completed(const FInputActionValue& value);
};
