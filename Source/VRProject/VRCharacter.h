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
class UNiagaraComponent;
class AVRTeleportVisualizer;

UCLASS()
class VRPROJECT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

	const float localTeleportLaunchSpeed = 650.f;
	const float localTeleportProjectileRadius = 3.6;
	const float localNavMeshCellHeight = 8.f;

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

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	class UNiagaraComponent* teleportTraceNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	FVector projectedLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	bool isValidTeleportLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	TSubclassOf<AVRTeleportVisualizer> vrTeleportVisualizer;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	class AVRTeleportVisualizer* vrTeleportVisualizerRef;

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

	void Teleport_Started(const FInputActionValue& value);
	void Teleport_Triggered(const FInputActionValue& value);
	void Teleport_Completed(const FInputActionValue& value);
	bool IsValidTeleportLocation(FHitResult& hit);
	void TryTeleport();
};
