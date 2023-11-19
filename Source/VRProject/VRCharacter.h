// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputConfigData.h"
#include "GrabComponent.h"
#include "VRCharacter.generated.h"

class UCameraComponent;
class UMotionControllerComponent;
class UNiagaraComponent;
class UInputMappingContext;
class AVRTeleportVisualizer;

UCLASS()
class VRPROJECT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

	const float localTeleportLaunchSpeed = 650.f;
	const float localTeleportProjectileRadius = 3.6;
	const float localNavMeshCellHeight = 8.f;
	const float grabRadiusFromGripPosition = 6.f;

	float localNearestComponentDistance = 9999999.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* camera;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* inputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputConfigData* inputActions;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	bool bTeleportTraceActive;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	AVRTeleportVisualizer* vrTeleportVisualizer;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	UGrabComponent* leftGrabComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	UGrabComponent* rightGrabComponent;

	public:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class UMotionControllerComponent* leftMotionController;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class UMotionControllerComponent* rightMotionController;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class UMotionControllerComponent* leftMotionControllerAim;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class UMotionControllerComponent* rightMotionControllerAim;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport")
		class UNiagaraComponent* teleportTraceNiagaraSystem;

	public:
		AVRCharacter();

	protected:
		virtual void BeginPlay() override;

		void SnapTurn_Started(const FInputActionValue& value);
		void Teleport_Started();
		void Teleport_Triggered();
		void Teleport_Completed();
		void Grab_Started(UMotionControllerComponent* motionController);
		void Grab_Completed(UMotionControllerComponent* motionController);
		bool IsValidTeleportLocation(FHitResult& hit, FVector& projectedLocation);
		UGrabComponent* GetGrabComponentNearMotionController(UMotionControllerComponent* motionController);

	public:	
		virtual void Tick(float DeltaTime) override;
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
